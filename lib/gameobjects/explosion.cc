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

#include <gameobjects/explosion.hh>

Explosion::Explosion(Game& game, Vector3f pos, Explosion::Type type) : GameObject(game), pos_(pos), type_(type), age_(0) {
}

void Explosion::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

unsigned int Explosion::GetLifetime() const {
	return 1000; // XXX: tweak for different explosion types
}

void Explosion::Update(unsigned int deltams) {
	age_ += deltams;
	if (age_ > GetLifetime())
		RemoveLater();
}
