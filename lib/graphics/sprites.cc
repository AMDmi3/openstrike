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
		// a horizontal flip flag is encoded in higher byte of block id
		for (auto& blockid : blockids)
			ids_.emplace_back(std::make_pair(manager_.Add(name, blockid & 0xff), blockid >> 8));
	}
}

void SpriteManager::BlockMap::Render(int x, int y) {
#if !defined DEBUG_RENDERING
	if (ids_.empty()) {
#endif
		// fallback if no resource was specified: just render a frame
		SDL2pp::Renderer& renderer = manager_.GetRenderer();
		renderer.SetDrawColor(0, 255, 0, 64);
		renderer.DrawRect(SDL2pp::Rect(x, y, width_, height_));
#if !defined DEBUG_RENDERING
		return;
	}
#endif

	static const int blockwidth = 16, blockheight = 16;
	int xpos = 0, ypos = 0;
	for (auto& id : ids_) {
		int flipflag = id.second ? SpriteManager::HFLIP : 0;
		manager_.Render(id.first, x + xpos, y + ypos, flags_ ^ flipflag);
		if ((xpos += blockwidth) >= width_) {
			xpos = 0;
			ypos += blockheight;
		}
	}
}

SpriteManager::TextMap::TextMap(SpriteManager& manager, const std::string& name, char firstchar, int firstframe, int nframes)
	: manager_(manager),
	  first_char_(firstchar),
	  space_width_(-1),
	  ascent_(-1),
	  median_(-1),
	  baseline_(-1),
	  descent_(-1) {
	for (int nframe = firstframe; nframe < firstchar + nframes; nframe++)
		ids_.emplace_back(manager_.Add(name, nframe));
}

bool SpriteManager::TextMap::HasChar(char ch) const {
	return ch >= first_char_ && ch < (int)(first_char_ + ids_.size());
}

SpriteManager::sprite_id_t SpriteManager::TextMap::GetChar(char ch) const {
	return ids_[ch - first_char_];
}

void SpriteManager::TextMap::UpdateDimensions() const {
	if (space_width_ != -1)
		return;

	if (HasChar('r'))
		space_width_ = manager_.GetSpriteInfo(GetChar('r')).width;

	if (HasChar('W'))
		ascent_ = manager_.GetSpriteInfo(GetChar('W')).yoffset;

	if (HasChar('x')) {
		median_ = manager_.GetSpriteInfo(GetChar('x')).yoffset;
		baseline_ = manager_.GetSpriteInfo(GetChar('x')).yoffset + manager_.GetSpriteInfo(GetChar('x')).height;
	}

	if (HasChar('p'))
		descent_ = manager_.GetSpriteInfo(GetChar('p')).yoffset + manager_.GetSpriteInfo(GetChar('p')).height;
}

int SpriteManager::TextMap::GetWidth(const std::string& text) const {
	UpdateDimensions();

	int width = 0, pos = 0;
	for (auto ch : text) {
		if (pos++)
			width++;
		if (ch == ' ') {
			width += space_width_;
			continue;
		}
		if (!HasChar(ch))
			ch = '?';
		if (!HasChar(ch)) {
			width += space_width_;
			continue;
		}

		width += manager_.GetSpriteInfo(GetChar(ch)).width;
	}
	return width;
}

void SpriteManager::TextMap::Render(int x, int y, const std::string& text, int align) {
	UpdateDimensions();

	int xpos = x, ypos = y;

	if (align & (HALIGN_CENTER|HALIGN_RIGHT)) {
		if (align & HALIGN_CENTER)
			xpos -= GetWidth(text)/2;
		else if (align & HALIGN_RIGHT)
			xpos -= GetWidth(text) - 1;
	}

	if (align & VALIGN_TOP)
		ypos -= ascent_;
	else if (align & VALIGN_MEDIAN)
		ypos -= median_;
	else if (align & VALIGN_BASELINE)
		ypos -= baseline_ - 1;
	else if (align & VALIGN_BOTTOM)
		ypos -= descent_ - 1;

	int pos = 0;
	for (auto ch : text) {
		if (pos++)
			xpos++;
		if (ch == ' ') {
			xpos += space_width_;
			continue;
		}
		if (!HasChar(ch))
			ch = '?';
		if (!HasChar(ch)) {
			xpos += space_width_;
			continue;
		}

		manager_.Render(GetChar(ch), xpos, ypos, PIVOT_FRAMECORNER);
		xpos += manager_.GetSpriteInfo(GetChar(ch)).width;
	}
}
