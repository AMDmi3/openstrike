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

#ifndef GAMEOBJECT_HH
#define GAMEOBJECT_HH

class Game;
class Visitor;

class GameObject {
protected:
	Game& game_;

public:
	GameObject(Game& game);
	virtual ~GameObject();

	virtual void Accept(Visitor& visitor) = 0;
	virtual void Update(unsigned int deltams) = 0;

	void RemoveLater();
};

#endif // GAMEOBJECT_HH
