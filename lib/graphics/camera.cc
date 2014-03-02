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

#include <cmath>

#include <graphics/camera.hh>

Camera::Camera(const Vector3f& target, const SDL2pp::Rect& viewport) : target_(target), viewport_(viewport) {
}

void Camera::SetTarget(const Vector3f& target) {
	target_ = target;
}

void Camera::SetViewport(const SDL2pp::Rect& viewport) {
	viewport_ = viewport;
}

Vector3f Camera::GetTarget() const {
	return target_;
}

SDL2pp::Rect Camera::GetViewport() const {
	return viewport_;
}

SDL2pp::Point Camera::GameToScreen(const Vector3f& point) const {
	return SDL2pp::Point(
			viewport_.GetX() + viewport_.GetW() / 2 + (int)std::round(point.x) - (int)std::round(target_.x),
			viewport_.GetY() + viewport_.GetH() / 2 + (int)std::round(point.y / 2) - (int)std::round(target_.y / 2) - (int)std::round(point.z) + (int)std::round(target_.z)
		);
}
