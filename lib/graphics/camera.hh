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

#ifndef CAMERA_HH
#define CAMERA_HH

#include <SDL2pp/Point.hh>
#include <SDL2pp/Rect.hh>
#include <math/geom.hh>

class Camera {
protected:
	Vector3f target_;
	SDL2pp::Rect viewport_;

public:
	Camera(const Vector3f& target, const SDL2pp::Rect& viewport);

	void SetTarget(const Vector3f& target);
	void SetViewport(const SDL2pp::Rect& viewport);

	Vector3f GetTarget() const;
	SDL2pp::Rect GetViewport() const;

	SDL2pp::Point GameToScreen(const Vector3f& point) const;
};

#endif // CAMERA_HH
