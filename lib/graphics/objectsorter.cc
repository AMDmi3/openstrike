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

#include <gameobjects/heli.hh>
#include <gameobjects/bullet.hh>
#include <gameobjects/rocket.hh>
#include <gameobjects/explosion.hh>

#include <graphics/objectsorter.hh>

ObjectSorter::ObjectSorter() {
}

void ObjectSorter::AddSorted(const Vector3f& pos, GameObject& obj) {
	float sortkey = pos.z - pos.y / 2;
	sorted_objects_.insert(std::make_pair(sortkey, &obj));
}

void ObjectSorter::Visit(GameObject& obj) {
	other_objects_.push_back(&obj);
}

void ObjectSorter::Visit(Heli& heli) {
	AddSorted(heli.GetPos(), heli);
}

void ObjectSorter::Visit(Bullet& bullet) {
	AddSorted(bullet.GetPos(), bullet);
}

void ObjectSorter::Visit(Rocket& rocket) {
	AddSorted(rocket.GetPos(), rocket);
}

void ObjectSorter::Visit(Explosion& explosion) {
	AddSorted(explosion.GetPos(), explosion);
}

void ObjectSorter::Accept(Visitor& visitor) {
	for (auto& object : sorted_objects_)
		object.second->Accept(visitor);
	for (auto& object : other_objects_)
		object->Accept(visitor);
}
