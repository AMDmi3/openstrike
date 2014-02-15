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

#ifndef GEOM_HH
#define GEOM_HH

template<typename T>
struct Vector3 {
	T x, y, z;

	Vector3(T xx = 0, T yy = 0, T zz = 0) : x(xx), y(yy), z(zz) {}

	static Vector3<T> FromYawPitch(T yaw, T pitch) {
		return Vector3<T>(
				sin(yaw) * cos(pitch),
				cos(yaw) * cos(pitch),
				sin(pitch)
			);
	}

	Vector3<T> operator+(const Vector3<T>& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
	Vector3<T> operator-(const Vector3<T>& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
	Vector3<T> operator*(T v) const { return Vector3<T>(x * v, y * v, z * v); }
	Vector3<T> operator/(T v) const { return Vector3<T>(x / v, y / v, z / v); }

	Vector3<T>& operator+=(const Vector3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3<T>& operator-=(const Vector3<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3<T>& operator*=(T v) { x *= v; y *= v; z *= v; return *this; }
	Vector3<T>& operator/=(T v) { x /= v; y /= v; z /= v; return *this; }

};

typedef Vector3<float> Vector3f;

#endif // GEOM_HH
