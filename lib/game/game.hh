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

#ifndef GAME_HH
#define GAME_HH

#include <memory>
#include <list>
#include <set>
#include <utility>

class Visitor;
class GameObject;

class Game {
protected:
	typedef std::list<std::unique_ptr<GameObject>> ObjectList;
	typedef std::set<const GameObject*> RemovedObjectsSet;

protected:
	float width_;
	float height_;
	ObjectList objects_;
	RemovedObjectsSet for_removal_;

protected:
	void RemoveScheduledObjects();

public:
	Game(float width, float height);

	template<class T, class... Args>
	T* Spawn(Args... args) {
		T* ptr;
		std::unique_ptr<GameObject> obj(ptr = new T(*this, std::forward<Args>(args)...));
		objects_.push_back(std::move(obj));
		return ptr;
	}

	void Accept(Visitor& visitor);
	void Update(unsigned int deltams);

	void RemoveLater(const GameObject* victim);

	float GetWidth() const;
	float GetHeight() const;
};

#endif // GAME_HH
