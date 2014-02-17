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

#include <math/pi.hh>

#include <graphics/spritemanager.hh>

SpriteManager::SingleSprite::SingleSprite(SpriteManager& manager, const std::string& name, int frame, int flags)
	: manager_(manager),
	  id_(manager.Add(name, frame)),
	  flags_(flags) {
}

void SpriteManager::SingleSprite::Render(int x, int y) {
	manager_.Render(id_, x, y, flags_);
}

SpriteManager::DirectionalSprite::DirectionalSprite(SpriteManager& manager, const std::string& name, int startframe, int nframes, int flags)
	: manager_(manager),
	  flags_(flags) {
	for(int i = 0; i < nframes; i++)
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

	assert(phase >= 0 && phase < ids_.size());

	manager_.Render(ids_[phase], x, y, flags_ ^ flipflag);
}

SpriteManager::LoopAnimation::LoopAnimation(SpriteManager& manager, const std::string& name, int startframe, int nframes, float fps, int flags)
	: manager_(manager),
	  frame_time_ms_(1000.0/fps),
	  own_time_(0),
	  flags_(flags) {
	if (frame_time_ms_ == 0)
		frame_time_ms_ = 1;
	for(int i = 0; i < nframes; i++)
		ids_.emplace_back(manager.Add(name, startframe + i));
}

void SpriteManager::LoopAnimation::Render(int x, int y) {
	int frame = own_time_ / frame_time_ms_;

	manager_.Render(ids_[frame], x, y, flags_);
}

void SpriteManager::LoopAnimation::Update(unsigned int deltams) {
	own_time_ = (own_time_ + deltams) % (frame_time_ms_ * ids_.size());
}
