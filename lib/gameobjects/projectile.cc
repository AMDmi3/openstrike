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

#include <gameobjects/projectile.hh>

class CollisionVisitor : public Visitor {
public:
	typedef std::function<void(Building&)> CollisionHandler;

private:
	Vector3f pos_;
	const CollisionHandler collision_handler_;

public:
	CollisionVisitor(const Vector3f& pos, CollisionHandler&& collision_handler) : pos_(pos), collision_handler_(collision_handler) {
	}

	void Visit(Building& building) {
		building.ForeachBBox([this, &building](const BBoxf& bbox) {
			if (bbox.Contains(pos_))
				collision_handler_(building);
		});
	}
};

Projectile::Projectile(Game& game, const Vector3f& pos, const Vector3f& vel, const Direction3f& direction, Type type)
	: GameObject(game),
	  pos_(pos),
	  vel_(vel + direction * Constants::Speed()),
	  dir_(direction),
	  type_(type) {
}

void Projectile::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

void Projectile::Update(unsigned int deltams) {
	float delta_sec = deltams / 1000.0f;

	// XXX: bullet actually moves along a parabola, should take into account
	// rockets, otoh, move  along a straight line
	pos_ += vel_ * delta_sec;

	bool had_collision = false;
	CollisionVisitor cv(pos_, [&had_collision, this](Building& building) {
		switch (type_) {
		case BULLET:   building.Damage(3); break;
		case HYDRA:    building.Damage(25); break;
		case HELLFIRE: building.Damage(100); break;
		}
		had_collision = true;
	});
	game_.Accept(cv);

	// object hit
	if (had_collision) {
		// XXX: calculate exact collision point?

		RemoveLater();

		switch (type_) {
		case BULLET:   game_.Spawn<Explosion>(pos_, Explosion::GUN_OBJECT); break;
		case HYDRA:    game_.Spawn<Explosion>(pos_, Explosion::HYDRA); break;
		case HELLFIRE: game_.Spawn<Explosion>(pos_, Explosion::HELLFIRE); break;
		}

		return;
	}

	// ground hit
	if (pos_.z <= 0) {
		// calculate exact collision point
		float penetration = pos_.z / vel_.z;

		pos_ -= vel_ * delta_sec * penetration;

		RemoveLater();

		switch (type_) {
		case BULLET:   game_.Spawn<Explosion>(pos_, Explosion::GUN_GROUND); break;
		case HYDRA:    game_.Spawn<Explosion>(pos_, Explosion::HYDRA); break;
		case HELLFIRE: game_.Spawn<Explosion>(pos_, Explosion::HELLFIRE); break;
		}

		return;
	}

	// g-force effect
	if (type_ == BULLET)
		vel_.z -= Constants::GForce() * delta_sec;

	// XXX: limit lifetime if it doesn't hit the ground
}
