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

#include <game/gameobject.hh>

#include <game/game.hh>

Game::Game(float width, float height) : width_(width), height_(height) {
}

Game::~Game() {
}

/**
 * these should be just:
 *
Game::Game(Game&& other) noexcept = default
Game& Game::operator=(Game&& other) noexcept = default;
 *
 * however, modern compilers still lack full c++11 support and won't
 * deduce default move constructor types correctly (usually missing
 * noexcept specifier)
 * only clang 3.4+ will compile this at the time of writing
 * (gcc 4.9 still won't)
**/

Game::Game(Game&& other) noexcept
	: width_(other.width_),
	  height_(other.height_),
	  objects_(std::move(other.objects_)),
	  for_removal_(std::move(other.for_removal_)) {
}

Game& Game::operator=(Game&& other) noexcept {
	width_ = other.width_;
	height_ = other.height_;
	objects_ = std::move(other.objects_);
	for_removal_ = std::move(other.for_removal_);
	return *this;
}

void Game::Accept(Visitor& visitor) {
	// remove objects that were scheduled from outside
	RemoveScheduledObjects();

	for (ObjectList::iterator object = objects_.begin(); object != objects_.end(); object++)
		(*object)->Accept(visitor);
}

void Game::Update(unsigned int deltams) {
	// remove objects that were scheduled from outside
	RemoveScheduledObjects();

	for (ObjectList::iterator object = objects_.begin(); object != objects_.end(); object++)
		(*object)->Update(deltams);

	// remove objects that were scheduled during update
	RemoveScheduledObjects();
}

void Game::RemoveLater(const GameObject* victim) {
	// note: only inserts once, e.g. no object will be removed twice
	for_removal_.insert(victim);
}

void Game::RemoveScheduledObjects() {
	if (for_removal_.empty())
		return;

	for (ObjectList::iterator object = objects_.begin(); object != objects_.end(); ) {
		ObjectList::iterator current = object++;
		RemovedObjectsSet::iterator victim = for_removal_.find(current->get());
		if (victim != for_removal_.end()) {
			for_removal_.erase(victim);
			objects_.erase(current);
		}
	}
	assert(for_removal_.empty());
}

float Game::GetWidth() const {
	return width_;
}

float Game::GetHeight() const {
	return height_;
}
