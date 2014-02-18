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

#include <iostream>

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include <dat/datfile.hh>
#include <dat/datgraphics.hh>

int FindNextGraphics(DatFile& datfile, int current = 0, bool forward = true) {
	bool found = false;
	int next = current;

	do {
		next += forward ? 1 : -1;
		if (next < 0)
			next = datfile.GetCount() - 1;
		if (next >= datfile.GetCount())
			next = 0;

		if (next == current)
			throw std::runtime_error("no graphics found in data file");

		found = true;
		try {
			DatGraphics gr(datfile.GetData(next));
		} catch (...) {
			found = false;
		}
	} while (!found);

	return next;
}

void usage(const char* progname) {
	std::cerr << "Usage: " << progname << " <filename.dat>" << std::endl;
}

int realmain(int argc, char** argv) {
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	DatFile datfile(argv[1]);
	int current_graphics = FindNextGraphics(datfile);
	int zoom = 2;

	SDL2pp::SDL sdl(SDL_INIT_VIDEO);
	SDL2pp::Window window("OpenStrike Sprite Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	SDL2pp::Renderer render(window, -1, SDL_RENDERER_ACCELERATED);

	render.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

	while (1) {
		std::cerr << "Displaying " << datfile.GetName(current_graphics) << "..." << std::endl;
		window.SetTitle(std::string("OpenStrike Sprite Viewer [") + datfile.GetName(current_graphics) + "]");

		Buffer graphics_data = datfile.GetData(current_graphics);
		DatGraphics graphics(graphics_data);

		render.SetDrawColor(0, 32, 32);
		render.Clear();

		int x = 0, y = 0, maxheight = 0;
		for (unsigned int i = 0; i < graphics.GetNumSprites(); ++i) {
			if (graphics.GetWidth(i) == 0 || graphics.GetHeight(i) == 0)
				continue;

			SDL2pp::Texture tex(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, graphics.GetWidth(i), graphics.GetHeight(i));
			tex.Update(SDL2pp::Rect::Null(), graphics.GetPixels(i).data(), graphics.GetWidth(i) * 4);
			tex.SetBlendMode(SDL_BLENDMODE_BLEND);

			if (x + graphics.GetFrameWidth(i) * zoom > window.GetWidth()) {
				x = 0;
				y += maxheight;
				maxheight = 0;
			}

			render.SetDrawColor(0, 255, 0, 191);
			render.DrawRect(SDL2pp::Rect(
						x,
						y,
						graphics.GetFrameWidth(i) * zoom,
						graphics.GetFrameHeight(i) * zoom
					));

			render.SetDrawColor(255, 255, 255, 32);
			render.DrawRect(SDL2pp::Rect(
						x + graphics.GetXOffset(i) * zoom,
						y + graphics.GetYOffset(i) * zoom,
						graphics.GetWidth(i) * zoom,
						graphics.GetHeight(i) * zoom
					));

			render.Copy(tex, SDL2pp::Rect::Null(), SDL2pp::Rect(
						x + graphics.GetXOffset(i) * zoom,
						y + graphics.GetYOffset(i) * zoom,
						graphics.GetWidth(i) * zoom,
						graphics.GetHeight(i) * zoom
					));

			maxheight = std::max(maxheight, graphics.GetFrameHeight(i) * zoom);
			x += graphics.GetFrameWidth(i) * zoom;
		}

		render.Present();

		// Process events
		SDL_Event event;
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// exit
				return 0;
			} else if (event.type == SDL_MOUSEMOTION) {
				// igmore
				continue;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
					return 0;
				if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_PAGEUP)
					current_graphics = FindNextGraphics(datfile, current_graphics, false);
				if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_PAGEDOWN)
					current_graphics = FindNextGraphics(datfile, current_graphics, true);
				if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_EQUALS || event.key.keysym.sym == SDLK_KP_PLUS)
					zoom++;
				if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS)
					if (zoom > 1)
						zoom--;
			}
			// repaint
			break;
		}
	}

	return 0;
}

int main(int argc, char** argv) {
	try {
		return realmain(argc, argv);
	} catch (SDL2pp::Exception& e) {
		std::cerr << "Error: " << e.what() << " (" << e.GetSDLError() << ")" << std::endl;
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 1;
}

