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

#ifndef BUILDING_HH
#define BUILDING_HH

#include <vector>

#include <math/geom.hh>
#include <math/bbox.hh>

#include <game/gameobject.hh>

#include <dat/datlevel.hh>

class Game;
class Visitor;

class Building : public GameObject {
protected:
	Vector3f pos_;
	unsigned short type_;
	Vector3f dead_pos_;
	unsigned short dead_type_;

	std::vector<BBoxf> bboxes_;
	std::vector<BBoxf> dead_bboxes_;

	int health_;

public:
	Building(Game& game, const Vector3f& pos, unsigned short type);
	Building(Game& game, const Vector3f& pos, unsigned short type, const Vector3f& dead_pos, unsigned short dead_type);

	virtual void Accept(Visitor& visitor);
	virtual void Update(unsigned int deltams);

	void Damage(int amount);
	void Die();

	Vector3f GetPos() const {
		return pos_;
	}

	unsigned short GetType() const {
		return type_;
	}

	void AddBBox(const BBoxf& bbox) {
		bboxes_.emplace_back(bbox);
	}

	void AddDeadBBox(const BBoxf& bbox) {
		dead_bboxes_.emplace_back(bbox);
	}

	void ForeachBBox(const std::function<void(const BBoxf&)> bbox_processor) const {
		for (auto bbox : bboxes_)
			bbox_processor(bbox);
	}
};

#endif // BUILDING_HH
