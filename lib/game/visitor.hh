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

#ifndef VISITOR_HH
#define VISITOR_HH

class GameObject;

class Bullet;
class Explosion;
class Heli;
class Rocket;

class Visitor {
public:
	virtual void Visit(GameObject&) {}

	virtual void Visit(Bullet& obj) { Visit((GameObject&)obj); }
	virtual void Visit(Explosion& obj) { Visit((GameObject&)obj); }
	virtual void Visit(Heli& obj) { Visit((GameObject&)obj); }
	virtual void Visit(Rocket& obj) { Visit((GameObject&)obj); }
};

#endif // VISITOR_HH
