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
#include <gameobjects/building.hh>

#include <gameobjects/bullet.hh>

class CollisionVisitor : public Visitor {
private:
	Vector3f pos_;
	bool had_collision_;

public:
	CollisionVisitor(const Vector3f& pos) : pos_(pos), had_collision_(false) {
	}

	void Visit(Building& building) {
		for (auto& bbox : building.GetBBoxes()) {
			if (bbox.Contains(pos_)) {
				had_collision_ = true;
				break;
			}
		}
	}

	bool HadCollision() const {
		return had_collision_;
	}
};

Bullet::Bullet(Game& game, Vector3f pos, Vector3f vel, Direction3f direction)
	: GameObject(game),
	  pos_(pos),
	  vel_(vel + direction * Constants::Speed()) {
}

void Bullet::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

void Bullet::Update(unsigned int deltams) {
	float delta_sec = deltams / 1000.0f;

	// XXX: bullet actually moves along a parabola, should take into account
	// rockets, otoh, move  along a straight line
	pos_ += vel_ * delta_sec;

	CollisionVisitor cv(pos_);
	game_.Accept(cv);

	// object hit
	if (cv.HadCollision()) {
		// XXX: calculate exact collision?

		RemoveLater();
		game_.Spawn<Explosion>(pos_, Explosion::GUN_OBJECT);
		return;
	}

	// ground hit
	if (pos_.z <= 0) {
		// calculate exact collision point
		float penetration = pos_.z / vel_.z;

		pos_ -= vel_ * delta_sec * penetration;

		RemoveLater();

		game_.Spawn<Explosion>(pos_, Explosion::GUN_GROUND);
		// XXX: cause damage
		return;
	}

	// g-force effect
	vel_.z -= Constants::GForce() * delta_sec;

	// XXX: limit lifetime if it doesn't hit the ground
}
