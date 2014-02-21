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

#ifndef ROCKET_HH
#define ROCKET_HH

#include <math/geom.hh>

#include <game/gameobject.hh>

class Game;
class Visitor;

class Rocket : public GameObject {
public:
	enum Type {
		HYDRA,
		HELLFIRE
	};

protected:
	struct Constants {
		static constexpr float Speed() { return 400.0; } // XXX: from bullet, fix
	};

protected:
	Vector3f pos_;
	Vector3f vel_;
	Direction3f dir_;
	Type type_;

public:
	Rocket(Game& game, Vector3f pos, Vector3f vel, Direction3f direction, Type type);

	virtual void Accept(Visitor& visitor);
	virtual void Update(unsigned int deltams);

	Vector3f GetPos() const {
		return pos_;
	}

	Direction3f GetDirection() const {
		return dir_;
	}

	Type GetType() const {
		return type_;
	}
};

#endif // HYDRA_HH
