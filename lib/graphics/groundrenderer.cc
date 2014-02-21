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

#include <graphics/camera.hh>

#include <graphics/groundrenderer.hh>

GroundRenderer::GroundRenderer(SDL2pp::Renderer& renderer) : renderer_(renderer) {
}

void GroundRenderer::Render(const Camera& camera, const SDL2pp::Rect& viewport) {
	renderer_.SetDrawColor(158, 126, 61);
	renderer_.FillRect(viewport);
}
