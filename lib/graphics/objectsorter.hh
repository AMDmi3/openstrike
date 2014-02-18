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

#ifndef OBJECTSORTER_HH
#define OBJECTSORTER_HH

#include <map>
#include <vector>

#include <math/geom.hh>

#include <game/visitor.hh>

class ObjectSorter : public Visitor {
protected:
	std::multimap<float, GameObject*> sorted_objects_;
	std::vector<GameObject*> other_objects_;

protected:
	void AddSorted(const Vector3f& pos, GameObject& obj);

public:
	ObjectSorter();

	virtual void Visit(GameObject& obj);

	virtual void Visit(Heli& heli);
	virtual void Visit(Bullet& bullet);
	virtual void Visit(Rocket& rocket);
	virtual void Visit(Explosion& explosion);

	void Accept(Visitor& visitor);
};

#endif // OBJECTSORTER_HH
