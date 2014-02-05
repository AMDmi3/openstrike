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

#include <SDL2/SDL_stdinc.h> // XXX <- this should be in SDL_pixels.h
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include <dat/datgraphics.hh>

#include <graphics/spritemanager.hh>

const int SpriteManager::atlas_page_width_ = 512;
const int SpriteManager::atlas_page_height_ = 512;

SpriteManager::SpriteManager(SDL2pp::Renderer& renderer) : renderer_(renderer), rect_packer_(atlas_page_width_, atlas_page_width_) {
}

SpriteManager::~SpriteManager() {
}

unsigned int SpriteManager::Add(const DatGraphics& graphics, unsigned int first, unsigned int count) {
	assert(first + count <= graphics.GetNumSprites());
	int retid = sprites_.size();

	// XXX: not exception-safe
	for (unsigned int i = 0; i < count; i++) {
		SpriteInfo sprite;

		sprite.width = graphics.GetWidth(first + i);
		sprite.height = graphics.GetHeight(first + i);
		sprite.xoffset = graphics.GetXOffset(first + i);
		sprite.yoffset = graphics.GetYOffset(first + i);
		sprite.framewidth = graphics.GetFrameWidth(first + i);
		sprite.frameheight = graphics.GetFrameHeight(first + i);

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
		std::vector<unsigned char> pixels = graphics.GetPixels(first + i);
		atlas_pages_[sprite.atlaspage].Update(SDL2pp::Rect(sprite.atlasx, sprite.atlasy, sprite.width, sprite.height), pixels.data(), sprite.width * 4);

		// Done
		sprites_.emplace_back(sprite);
	}

	return retid;
}

void SpriteManager::Render(unsigned int id, int x, int y, int flags) {
	const SpriteInfo& sprite = sprites_[id];

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
				SDL2pp::Point::Null(),
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
