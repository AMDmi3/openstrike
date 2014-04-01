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

#include <math/pi.hh>

#include <cmath>

template<typename T>
struct Vector2;

template<typename T>
struct Vector3;

template<typename T>
struct Direction2 {
	T yaw;

	constexpr Direction2(T y = 0) : yaw(NormalizeYaw(y)) {}

	constexpr Vector2<T> ToVector(T length = 1) const { return Vector2<T>(sin(yaw), -cos(yaw)) * length; }

	constexpr Direction2<T> RotatedCW(T angle) const { return Direction2<T>(yaw + angle); }
	constexpr Direction2<T> RotatedCCW(T angle) const { return Direction2<T>(yaw - angle); }
	constexpr Vector2<T> operator*(T m) const { return ToVector(m); }
	constexpr Vector2<T> operator/(T d) const { return ToVector(1/d); }

	Direction2<T>& RotateCW(T angle) { yaw = NormalizeYaw(yaw + angle); return *this; }
	Direction2<T>& RotateCCW(T angle) { yaw = NormalizeYaw(yaw - angle); return *this; }

	constexpr static T NormalizeYaw(T angle) {
		return (angle < 0) ? std::fmod(angle, 2*pi) + 2*pi : ((angle >= 2*pi) ? std::fmod(angle, 2*pi) : angle);
	}
};

template<typename T>
struct Direction3 {
	T yaw, pitch;

	constexpr Direction3(Direction2<T> dir, T p = 0) : yaw(NormalizeYaw(dir.yaw)), pitch(p) {}
	constexpr Direction3(T y = 0, T p = 0) : yaw(NormalizeYaw(y)), pitch(p) {}

	constexpr Vector3<T> ToVector(T length = 1) const { return Vector3<T>(sin(yaw) * cos(pitch), -cos(yaw) * cos(pitch), sin(pitch)) * length; }

	constexpr operator Direction2<T>() const { return Direction2<T>(yaw); }

	constexpr Direction3<T> RotatedCW(T angle) const { return Direction3<T>(yaw + angle, pitch); }
	constexpr Direction3<T> RotatedCCW(T angle) const { return Direction3<T>(yaw - angle, pitch); }
	constexpr Direction3<T> RotatedUp(T angle) const { return Direction3<T>(yaw, pitch + angle); }
	constexpr Direction3<T> RotatedDown(T angle) const { return Direction3<T>(yaw, pitch - angle); }
	constexpr Vector3<T> operator*(T m) const { return ToVector(m); }
	constexpr Vector3<T> operator/(T d) const { return ToVector(1/d); }

	Direction2<T>& RotateCW(T angle) { yaw = NormalizeYaw(yaw + angle); return *this; }
	Direction2<T>& RotateCCW(T angle) { yaw = NormalizeYaw(yaw - angle); return *this; }
	Direction3<T>& RotateUp(T angle) { pitch += angle; return *this; }
	Direction3<T>& RotateDown(T angle) { pitch -= angle; return *this; }

	constexpr static T NormalizeYaw(T angle) {
		return Direction2<T>::NormalizeYaw(angle);
	}
};

template<typename T>
struct Vector2 {
	T x, y;

	constexpr Vector2(T xx = 0, T yy = 0) : x(xx), y(yy) {}

	constexpr Vector2<T> operator+(const Vector2<T>& v) const { return Vector2<T>(x + v.x, y + v.y); }
	constexpr Vector2<T> operator-(const Vector2<T>& v) const { return Vector2<T>(x - v.x, y - v.y); }
	constexpr Vector2<T> operator*(T m) const { return Vector2<T>(x * m, y * m); }
	constexpr Vector2<T> operator/(T d) const { return Vector2<T>(x / d, y / d); }

	constexpr Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

	constexpr Vector2<T> operator*(const Direction2<T>& d) const { return d.RotatedCW(pi/2).ToVector(x) - d.ToVector(y); }

	Vector2<T>& operator+=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }
	Vector2<T>& operator-=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }
	Vector2<T>& operator*=(T m) { x *= m; y *= m; return *this; }
	Vector2<T>& operator/=(T d) { x /= d; y /= d; return *this; }
};

template<typename T>
struct Vector3 {
	T x, y, z;

	constexpr Vector3(Vector2<T> xy, T zz = 0) : x(xy.x), y(xy.y), z(zz) {}
	constexpr Vector3(T xx = 0, T yy = 0, T zz = 0) : x(xx), y(yy), z(zz) {}

	constexpr operator Vector2<T>() const { return Vector2<T>(x, y); }

	constexpr Vector3<T> operator+(const Vector3<T>& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
	constexpr Vector3<T> operator-(const Vector3<T>& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
	constexpr Vector3<T> operator*(T m) const { return Vector3<T>(x * m, y * m, z * m); }
	constexpr Vector3<T> operator/(T d) const { return Vector3<T>(x / d, y / d, z / d); }

	constexpr Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

	constexpr Vector3<T> operator*(const Direction2<T>& d) const { return Vector3(d.RotatedCW(pi/2).ToVector(x) - d.ToVector(y), z); }

	constexpr Vector3<T> Grounded() const { return Vector3(x, y, 0); }

	Vector3<T>& operator+=(const Vector3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3<T>& operator-=(const Vector3<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3<T>& operator*=(T m) { x *= m; y *= m; z *= m; return *this; }
	Vector3<T>& operator/=(T d) { x /= d; y /= d; z /= d; return *this; }
	Vector3<T>& Ground() { z = 0; return *this; }
};

typedef Direction2<float> Direction2f;
typedef Direction3<float> Direction3f;
typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;

#endif // GEOM_HH
