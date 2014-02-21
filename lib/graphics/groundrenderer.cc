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

#include <game/game.hh>
#include <graphics/camera.hh>

#include <graphics/groundrenderer.hh>

GroundRenderer::GroundRenderer(SDL2pp::Renderer& renderer) : renderer_(renderer) {
}

void GroundRenderer::Render(const Game& game, const Camera& camera) {
	renderer_.SetDrawColor(158, 126, 61);
	renderer_.FillRect(camera.GetViewport());

	// Draw sector grid
	renderer_.SetDrawColor(255, 0, 0);
	for (int vline = 0; vline <= game.GetWidth() / 512.0; vline++) {
		renderer_.DrawLine(
				camera.GameToScreen(Vector3f(vline * 512.0, 0, 0)),
				camera.GameToScreen(Vector3f(vline * 512.0, game.GetHeight(), 0))
			);
	}
	for (int hline = 0; hline <= game.GetHeight() / 512.0; hline++) {
		renderer_.DrawLine(
				camera.GameToScreen(Vector3f(0, hline * 512.0, 0)),
				camera.GameToScreen(Vector3f(game.GetWidth(), hline * 512.0, 0))
			);
	}
}
