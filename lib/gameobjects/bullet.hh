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

#ifndef BULLET_HH
#define BULLET_HH

#include <game/gameobject.hh>

class Game;
class Visitor;

class Bullet : public GameObject {
protected:
	static constexpr float speed_ = 400.0;

protected:
	float x_pos_;
	float y_pos_;
	float height_;

	float direction_;
	float pitch_;

public:
	Bullet(Game& game, float x, float y, float height, float direction, float pitch);

	virtual void Accept(Visitor& visitor) const;
	virtual void Update(unsigned int deltams);

	float GetXPos() const {
		return x_pos_;
	}

	float GetYPos() const {
		return y_pos_;
	}

	float GetHeight() const {
		return height_;
	}
};

#endif // BULLET_HH
