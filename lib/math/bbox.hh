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

#ifndef BBOX_HH
#define BBOX_HH

#include <algorithm>

#include <math/geom.hh>

template<typename T>
struct BBox {
	Vector3<T> pos;
	Direction2<T> direction;
	const T left, front, right, back, bottom, top;

	BBox() : left(0), front(0), right(0), back(0), bottom(0), top(0) {}

	BBox(Vector3<T> pos, T left, T front, T right, T back, T bottom, T top, Direction2<T> direction = Direction2<T>())
		: pos(pos),
		  direction(direction),
		  left(std::min(left, right)),
		  front(std::min(front, back)),
		  right(std::max(left, right)),
		  back(std::max(front, back)),
		  bottom(std::min(bottom, top)),
		  top(std::max(bottom, top)) {
	}

	bool Contains(const Vector3<T>& point) const {
		if (point.z < pos.z + bottom || point.z > pos.z + top)
			return false;

		Vector3<T> toPoint = (point - pos) * Direction2<T>(-direction.yaw);

		return !(toPoint.x < left || toPoint.x > right || toPoint.y < front || toPoint.y > back);
	}

	template<class Fn>
	void ForEachEdge(const Fn& fn) const {
		Vector2<T> p0 = Vector2<T>(pos) + Vector2<T>(left, front) * direction;
		Vector2<T> p1 = Vector2<T>(pos) + Vector2<T>(right, front) * direction;
		Vector2<T> p2 = Vector2<T>(pos) + Vector2<T>(right, back) * direction;
		Vector2<T> p3 = Vector2<T>(pos) + Vector2<T>(left, back) * direction;

		fn(Vector3<T>(p0, pos.z + bottom), Vector3<T>(p1, pos.z + bottom));
		fn(Vector3<T>(p1, pos.z + bottom), Vector3<T>(p2, pos.z + bottom));
		fn(Vector3<T>(p2, pos.z + bottom), Vector3<T>(p3, pos.z + bottom));
		fn(Vector3<T>(p3, pos.z + bottom), Vector3<T>(p0, pos.z + bottom));

		fn(Vector3<T>(p0, pos.z + top), Vector3<T>(p1, pos.z + top));
		fn(Vector3<T>(p1, pos.z + top), Vector3<T>(p2, pos.z + top));
		fn(Vector3<T>(p2, pos.z + top), Vector3<T>(p3, pos.z + top));
		fn(Vector3<T>(p3, pos.z + top), Vector3<T>(p0, pos.z + top));

		fn(Vector3<T>(p0, pos.z + bottom), Vector3<T>(p0, pos.z + top));
		fn(Vector3<T>(p1, pos.z + bottom), Vector3<T>(p1, pos.z + top));
		fn(Vector3<T>(p2, pos.z + bottom), Vector3<T>(p2, pos.z + top));
		fn(Vector3<T>(p3, pos.z + bottom), Vector3<T>(p3, pos.z + top));
	}
};

typedef BBox<float> BBoxf;

#endif // BBOX_HH
