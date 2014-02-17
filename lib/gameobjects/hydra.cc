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

#include <gameobjects/explosion.hh>

#include <gameobjects/hydra.hh>

Hydra::Hydra(Game& game, Vector3f pos, Direction3f direction) : GameObject(game) {
	pos_ = pos;
	dir_ = direction;
}

void Hydra::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

void Hydra::Update(unsigned int deltams) {
	float delta_sec = deltams / 1000.0f;

	Vector3f vel = dir_ * speed_;

	pos_ += vel * delta_sec;

	if (pos_.z <= 0) {
		// calculate exact collision point
		float penetration = pos_.z / vel.z;

		pos_ -= vel * delta_sec * penetration;

		RemoveLater();

		game_.Spawn<Explosion>(pos_, Explosion::SMALL);
		// XXX: cause damage
	}

	// XXX: limit lifetime if it doesn't hit the ground
}
