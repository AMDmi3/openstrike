/*
 * Copyright (C) 2013-2014 Dmitry Marakasov
 *
 * This file is part of openstrike.
 *
 * openstrike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * openstrike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with openstrike.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <set>

#include <SDL2/SDL_stdinc.h> // XXX <- this should be in SDL_pixels.h
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include <dat/datgraphics.hh>
#include <dat/datfile.hh>

#include <graphics/spritemanager.hh>

const int SpriteManager::atlas_page_width_ = 512;
const int SpriteManager::atlas_page_height_ = 512;

SpriteManager::SpriteManager(SDL2pp::Renderer& renderer, DatFile& datfile) : renderer_(renderer), datfile_(datfile), rect_packer_(atlas_page_width_, atlas_page_width_) {
}

SpriteManager::~SpriteManager() {
}

void SpriteManager::LoadAll(const LoadingStatusCallback& statuscb) {
	int numloaded = 0, numtoload = 0;

	// gather sprites to load, grouped by resource
	std::map<std::string, std::set<sprite_id_t>> toload;
	sprite_id_t current_id = 0;
	for (auto& sprite : sprites_) {
		if (!sprite.loaded) {
			numtoload++;
			toload[sprite.resource].insert(current_id);
		}
		current_id++;
	}

	if (statuscb)
		statuscb(0, numtoload);

	// load
	for (auto& resource : toload) {
		Buffer data = datfile_.GetData(resource.first);
		DatGraphics gfx(data);

		for (auto& sprite : resource.second) {
			Load(sprite, gfx);
			if (statuscb)
				statuscb(++numloaded, numtoload);
		}
	}
}

SpriteManager::sprite_id_t SpriteManager::Add(const std::string& resource, unsigned int frame, bool load_immediately) {
	SpriteMap::iterator known_sprite = known_sprites_.find(std::make_pair(resource, frame));

	sprite_id_t id;

	if (known_sprite == known_sprites_.end()) {
		id = sprites_.size();
		sprites_.emplace_back(resource, frame);
		known_sprites_[std::make_pair(resource, frame)] = id;
	} else {
		id = known_sprite->second;
	}

	if (load_immediately && !sprites_[id].loaded)
		Load(id);

	return id;
}

void SpriteManager::Render(sprite_id_t id, int x, int y, int flags) {
	const SpriteInfo& sprite = sprites_[id];
	assert(sprite.loaded);

	if (sprite.width == 0 && sprite.height == 0)
		return;

	int xoffset = 0, yoffset = 0;

	if (flags & PIVOT_USEFRAME) {
		xoffset += sprite.xoffset;
		yoffset += sprite.yoffset;

		if (flags & HFLIP_FRAME)
			xoffset += sprite.framewidth - 2 * sprite.xoffset - sprite.width;
	}

	if (flags & PIVOT_USECENTER) {
		if (flags & PIVOT_USEFRAME) {
			xoffset -= sprite.framewidth / 2;
			yoffset -= sprite.frameheight / 2;
		} else {
			xoffset -= sprite.width / 2;
			yoffset -= sprite.width / 2;
		}
	}

	if (flags & HFLIP_SPRITE) {
		renderer_.Copy(
				atlas_pages_[sprite.atlaspage],
				SDL2pp::Rect(sprite.atlasx, sprite.atlasy, sprite.width, sprite.height),
				SDL2pp::Rect(x + xoffset, y + yoffset, sprite.width, sprite.height),
				0.0,
				SDL2pp::NullOpt,
				SDL_FLIP_HORIZONTAL
			);
	} else {
		renderer_.Copy(
				atlas_pages_[sprite.atlaspage],
				SDL2pp::Rect(sprite.atlasx, sprite.atlasy, sprite.width, sprite.height),
				SDL2pp::Rect(x + xoffset, y + yoffset, sprite.width, sprite.height)
			);
	}
}

const SpriteManager::SpriteInfo& SpriteManager::GetSpriteInfo(sprite_id_t id) const {
	return sprites_[id];
}

void SpriteManager::Load(SpriteManager::sprite_id_t id, const DatGraphics& graphics) {
	SpriteInfo& sprite = sprites_[id];

	if (sprite.loaded)
		return;

	unsigned int nframe = sprite.frame;

	sprite.width = graphics.GetWidth(nframe);
	sprite.height = graphics.GetHeight(nframe);
	sprite.xoffset = graphics.GetXOffset(nframe);
	sprite.yoffset = graphics.GetYOffset(nframe);
	sprite.framewidth = graphics.GetFrameWidth(nframe);
	sprite.frameheight = graphics.GetFrameHeight(nframe);

	if (sprite.width == 0 && sprite.height == 0) {
		sprite.loaded = true;
		return;
	}

	// place sprite in atlas
	// XXX: padding is required when SDL_RenderSetLogicalSize is used
	// XXX: otherwise parts of adjacent sprites are occasionally shown; investigate
	const RectPacker::Rect& placed = rect_packer_.Place(sprite.width, sprite.height, 1);
	sprite.atlaspage = placed.page;
	sprite.atlasx = placed.x;
	sprite.atlasy = placed.y;

	// Create missing atlas textures
	while (sprite.atlaspage >= atlas_pages_.size()) {
		atlas_pages_.emplace_back(renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, atlas_page_width_, atlas_page_height_);
		atlas_pages_.back().SetBlendMode(SDL_BLENDMODE_BLEND);
	}

	// Write pixels to texture
	std::vector<unsigned char> pixels = graphics.GetPixels(nframe);
	atlas_pages_[sprite.atlaspage].Update(SDL2pp::Rect(sprite.atlasx, sprite.atlasy, sprite.width, sprite.height), pixels.data(), sprite.width * 4);

	// Done
	sprite.loaded = true;
}

void SpriteManager::Load(SpriteManager::sprite_id_t id) {
	SpriteInfo& sprite = sprites_[id];

	if (sprite.loaded)
		return;

	Buffer data = datfile_.GetData(sprite.resource);
	DatGraphics gfx(data);

	Load(id, gfx);
}

SDL2pp::Renderer& SpriteManager::GetRenderer() {
	return renderer_;
}
