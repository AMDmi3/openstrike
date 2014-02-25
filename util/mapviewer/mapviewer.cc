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

#include <graphics/camera.hh>
#include <graphics/spritemanager.hh>
#include <graphics/renderer.hh>
#include <graphics/groundrenderer.hh>
#include <dat/datfile.hh>
#include <game/levelloader.hh>
#include <game/game.hh>

void usage(const char* progname) {
	std::cerr << "Usage: " << progname << " <filename.dat>" << std::endl;
}

int realmain(int argc, char** argv) {
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	DatFile datfile(argv[1]);
	int zoom = 1;

	SDL2pp::SDL sdl(SDL_INIT_VIDEO);
	SDL2pp::Window window("OpenStrike Map Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    // Game stuff
    SpriteManager spriteman(renderer, datfile);

    Renderer game_renderer(spriteman);
    GroundRenderer ground_renderer(renderer);

    // game_renderer has notified sprite manager of needed sprites,
    // now it will load them
    spriteman.LoadAll(
            [](int loaded, int total) {
                std::cerr << "Loading sprites: " << loaded << "/" << total << std::endl;
            }
        );

    Game game = LevelLoader::Load(datfile, "LEVEL0", 12, 6); // sizes correspond to first level of Desert Strike

    Camera camera(Vector3f(12 * 512 / 2, 6 * 512 / 2, 0), SDL2pp::Rect(0, 0, 800, 600));
	static const int x_scroll_speed = 32;
	static const int y_scroll_speed = 64;

	while (1) {
		SDL2pp::Rect viewport(0, 0, window.GetWidth(), window.GetHeight());

		camera.SetViewport(viewport);
		renderer.SetViewport(viewport);
		renderer.SetScale(zoom, zoom);

		renderer.SetDrawColor(0, 32, 32);
		renderer.Clear();

		ground_renderer.Render(game, camera);
		game_renderer.Render(game, camera);

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
				if (event.key.keysym.sym == SDLK_LEFT)
					camera.SetTarget(camera.GetTarget() + Vector3f(-x_scroll_speed * zoom, 0, 0));
				if (event.key.keysym.sym == SDLK_RIGHT)
					camera.SetTarget(camera.GetTarget() + Vector3f(x_scroll_speed * zoom, 0, 0));
				if (event.key.keysym.sym == SDLK_UP)
					camera.SetTarget(camera.GetTarget() + Vector3f(0, -y_scroll_speed * zoom, 0));
				if (event.key.keysym.sym == SDLK_DOWN)
					camera.SetTarget(camera.GetTarget() + Vector3f(0, y_scroll_speed * zoom, 0));
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

