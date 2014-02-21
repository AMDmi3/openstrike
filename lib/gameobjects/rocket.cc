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

#include <gameobjects/rocket.hh>

Rocket::Rocket(Game& game, Vector3f pos, Vector3f vel, Direction3f direction, Rocket::Type type)
	: GameObject(game),
	  pos_(pos),
	  vel_(vel + direction * Constants::Speed()),
	  dir_(direction),
	  type_(type) {
}

void Rocket::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

void Rocket::Update(unsigned int deltams) {
	float delta_sec = deltams / 1000.0f;

	pos_ += vel_ * delta_sec;

	if (pos_.z <= 0) {
		// calculate exact collision point
		float penetration = pos_.z / vel_.z;

		pos_ -= vel_ * delta_sec * penetration;

		RemoveLater();

		switch (type_) {
		case HYDRA:    game_.Spawn<Explosion>(pos_, Explosion::HYDRA); break;
		case HELLFIRE: game_.Spawn<Explosion>(pos_, Explosion::HELLFIRE); break;
		}
		// XXX: cause damage
	}

	// XXX: limit lifetime if it doesn't hit the ground
}
