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
#include <algorithm>

#include <math/pi.hh>

#include <graphics/spritemanager.hh>

SpriteManager::SingleSprite::SingleSprite(SpriteManager& manager, const std::string& name, unsigned int frame, int flags)
	: manager_(manager),
	  id_(manager.Add(name, frame)),
	  flags_(flags) {
}

void SpriteManager::SingleSprite::Render(int x, int y) {
	manager_.Render(id_, x, y, flags_);
}

SpriteManager::DirectionalSprite::DirectionalSprite(SpriteManager& manager, const std::string& name, unsigned int startframe, unsigned int nframes, int flags)
	: manager_(manager),
	  flags_(flags) {
	for (unsigned int i = 0; i < nframes; i++)
		ids_.emplace_back(manager.Add(name, startframe + i));
}

void SpriteManager::DirectionalSprite::Render(int x, int y, float angle) {
	unsigned int nphases = (ids_.size() - 1) * 2;
	unsigned int phase = (unsigned int)((angle / (2.0 * pi) * nphases) + 0.5) % nphases;

	int flipflag = 0;
	if (phase > nphases / 2) {
		phase = nphases - phase;
		flipflag = SpriteManager::HFLIP;
	}

	assert(phase < ids_.size());

	manager_.Render(ids_[phase], x, y, flags_ ^ flipflag);
}

SpriteManager::Animation::Animation(SpriteManager& manager, int flags)
	: manager_(manager),
	  flags_(flags) {
}

SpriteManager::Animation::Animation(SpriteManager& manager, const std::string& name, unsigned int startframe, unsigned int nframes, int flags)
	: manager_(manager),
	  flags_(flags) {
	for (unsigned int i = 0; i < nframes; i++)
		ids_.emplace_back(manager.Add(name, startframe + i));
}

SpriteManager::Animation::Animation(SpriteManager& manager, const std::string& name, const std::vector<unsigned int>& frames, int flags)
	: manager_(manager),
	  flags_(flags) {
	for (auto& frame : frames)
		ids_.emplace_back(manager.Add(name, frame));
}

void SpriteManager::Animation::AddFrames(const std::string& name, unsigned int startframe, unsigned int nframes) {
	for (unsigned int i = 0; i < nframes; i++)
		ids_.emplace_back(manager_.Add(name, startframe + i));
}

void SpriteManager::Animation::AddFrames(const std::string& name, const std::vector<unsigned int>& frames) {
	for (auto& frame : frames)
		ids_.emplace_back(manager_.Add(name, frame));
}

void SpriteManager::Animation::Render(int x, int y, unsigned int nframe) {
	manager_.Render(ids_[nframe % ids_.size()], x, y, flags_);
}

unsigned int SpriteManager::Animation::GetNumFrames() const {
	return ids_.size();
}

SpriteManager::BlockMap::BlockMap(SpriteManager& manager, const std::string& name, const std::vector<unsigned short>& blockids, int width, int height, int flags)
	: manager_(manager),
	  flags_(flags),
	  width_(width),
	  height_(height) {
	if (!name.empty()) {
		for (auto& blockid : blockids)
			ids_.emplace_back(manager_.Add(name, blockid));
	}
}

void SpriteManager::BlockMap::Render(int x, int y) {
	static const int blockwidth = 16, blockheight = 16;
	int xpos = 0, ypos = 0;
	for (auto& id : ids_) {
		manager_.Render(id, x + xpos, y + ypos, flags_);
		if ((xpos += blockwidth) >= width_) {
			xpos = 0;
			ypos += blockheight;
		}
	}
	// fallback if no resource was specified: just render a frame
	//if (ids_.empty()) {
		SDL2pp::Renderer& renderer = manager_.GetRenderer();
		renderer.SetDrawColor(0, 255, 0);
		renderer.DrawRect(SDL2pp::Rect(x, y, width_, height_));
	//}
}
