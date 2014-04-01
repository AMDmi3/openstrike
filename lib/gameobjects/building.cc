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

#include <game/visitor.hh>
#include <game/game.hh>

#include <game/game.hh>

#include <gameobjects/explosion.hh>

#include <gameobjects/building.hh>

Building::Building(Game& game, const Vector3f& pos, unsigned short type, const Vector3f& sprite_offset)
	: GameObject(game),
	  pos_(pos),
	  type_(type),
	  sprite_offset_(sprite_offset),
	  dead_type_(type),
	  dead_sprite_offset_(sprite_offset),
	  health_(1) { // XXX: implement real health
}

Building::Building(Game& game, const Vector3f& pos, unsigned short type, const Vector3f& sprite_offset, unsigned short dead_type, const Vector3f& dead_sprite_offset)
	: GameObject(game),
	  pos_(pos),
	  type_(type),
	  sprite_offset_(sprite_offset),
	  dead_type_(dead_type),
	  dead_sprite_offset_(dead_sprite_offset),
	  health_(1) { // XXX: implement real health
}

void Building::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

void Building::Update(unsigned int) {
	// does nothing
}

void Building::Damage(int amount) {
	if (type_ == dead_type_)
		return;

	health_ -= amount;
	if (health_ <= 0)
		Die();
}

void Building::Die() {
	game_.Spawn<Explosion>(pos_, Explosion::BOOM);
	type_ = dead_type_;
	sprite_offset_ = dead_sprite_offset_;
	bboxes_.swap(dead_bboxes_);
}
