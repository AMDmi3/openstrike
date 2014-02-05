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

#ifndef HELI_HH
#define HELI_HH

#include <game/gameobject.hh>

class Game;
class Visitor;

class Heli : public GameObject {
protected:
	float direction_;
	unsigned int rotor_phase_;

public:
	Heli(Game& game);

	virtual void Accept(Visitor& visitor) const;
	virtual void Update(unsigned int deltams);

	float GetDirection() const {
		return direction_;
	}

	unsigned int GetRotorPhase() const {
		return rotor_phase_;
	}

	void Left() {
		direction_ -= 15.0f;
		if (direction_ < 0.0f)
			direction_ += 360.0f;
	}

	void Right() {
		direction_ += 15.0f;
		if (direction_ > 360.0f)
			direction_ -= 360.0f;
	}
};

#endif // HELI_HH
