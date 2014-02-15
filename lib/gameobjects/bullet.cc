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

#include <cmath>

#include <game/visitor.hh>

#include <gameobjects/bullet.hh>

Bullet::Bullet(Game& game, float x, float y, float height, float direction, float pitch) : GameObject(game) {
	x_pos_ = x;
	y_pos_ = y;
	height_ = height;
	direction_ = direction;
	pitch_ = pitch;
}

void Bullet::Accept(Visitor& visitor) const {
	visitor.Visit(*this);
}

void Bullet::Update(unsigned int deltams) {
	float delta_sec = deltams / 1000.0f;

	float dx = sin(direction_) * cos(pitch_);
	float dy = cos(direction_) * cos(pitch_);
	float dh = sin(pitch_);

	x_pos_ += dx * speed_ * delta_sec;
	y_pos_ += dy * speed_ * delta_sec;

	// XXX: bullet actually moves along a parabola, should take into account
	// rockets, otoh, move  along a straight line
	height_ += dh * speed_ * delta_sec;

	if (height_ <= 0) {
		// calculate exact collision point
		float penetration = 1.0f + height_ / dh;

		x_pos_ -= dx * speed_ * delta_sec * penetration;
		y_pos_ -= dy * speed_ * delta_sec * penetration;
		height_ = 0.0f;

		RemoveLater();

		// XXX: spawn explosion object
		// XXX: cause damage
	}

	// XXX: limit lifetime if it doesn't hit the ground
}
