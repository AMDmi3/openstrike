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
#include <graphics/spritemanager.hh>

std::vector<int> hflags = {
	SpriteManager::TextMap::HALIGN_LEFT,
	SpriteManager::TextMap::HALIGN_CENTER,
	SpriteManager::TextMap::HALIGN_RIGHT,
};

std::vector<int> vflags = {
	SpriteManager::TextMap::VALIGN_TOP,
	SpriteManager::TextMap::VALIGN_MEDIAN,
	SpriteManager::TextMap::VALIGN_BASELINE,
	SpriteManager::TextMap::VALIGN_BOTTOM,
};

void usage(const char* progname) {
	std::cerr << "Usage: " << progname << " <filename.dat>" << std::endl;
}

int realmain(int argc, char** argv) {
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	DatFile datfile(argv[1]);

	SDL2pp::SDL sdl(SDL_INIT_VIDEO);
	SDL2pp::Window window("OpenStrike Sprite Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

	int zoom = 2;
	renderer.SetScale(zoom, zoom);

	SpriteManager spriteman(renderer, datfile);

	SpriteManager::TextMap text(spriteman, "01CHARS", '!', 17, 126);

	spriteman.LoadAll();

	while (1) {
		renderer.SetDrawColor(0, 32, 32);
		renderer.Clear();

		text.Render(window.GetWidth() / zoom / 2, 0, "The quick brown fox jumps over the lazy dog", SpriteManager::TextMap::HALIGN_CENTER | SpriteManager::TextMap::VALIGN_TOP);

		renderer.SetDrawColor(255, 255, 255, 64);
		for (unsigned int x = 0; x < hflags.size(); x++) {
			int xstep = window.GetWidth() / zoom / hflags.size();
			int xpos = xstep / 2 + x * xstep;
			for (unsigned int y = 0; y < vflags.size(); y++) {
				int ystep = window.GetHeight() / zoom / vflags.size();
				int ypos = ystep / 2 + y * ystep;

				renderer.DrawLine(xpos - 40, ypos, xpos + 40, ypos);
				renderer.DrawLine(xpos, ypos - 10, xpos, ypos + 10);

				text.Render(xpos, ypos, "Fj op", hflags[x] | vflags[y]);
			}
		}

		renderer.Present();

		// Process events
		SDL_Event event;
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// exit
				return 0;
			} else if (event.type == SDL_MOUSEMOTION) {
				// ignore
				continue;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
					return 0;
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

