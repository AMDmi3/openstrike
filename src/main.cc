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
#include <limits>

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include <dat/datfile.hh>
#include <dat/datgraphics.hh>
#include <graphics/spritemanager.hh>

#include <graphics/camera.hh>
#include <graphics/groundrenderer.hh>
#include <graphics/renderer.hh>
#include <game/game.hh>
#include <game/levelloader.hh>
#include <gameobjects/heli.hh>

void usage(const char* progname) {
	std::cerr << "Usage: " << progname << " <filename.dat>" << std::endl;
}

void SetupViewport(SDL2pp::Window& window, SDL2pp::Renderer& renderer) {
	static int target_w = 320;
	static int target_h = 200;

	int w = window.GetWidth();
	int h = window.GetHeight();

	int scale = std::min(w / target_w, h / target_h);

	renderer.SetViewport(SDL2pp::Rect((w - target_w * scale) / 2 / scale, (h - target_h * scale) / 2 / scale, 320, 200));
	renderer.SetScale(scale, scale);
}

int realmain(int argc, char** argv) {
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	// Data file
	DatFile datfile(argv[1]);

	// SDL stuff
	SDL2pp::SDL sdl(SDL_INIT_VIDEO);
	SDL2pp::Window window("OpenStrike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
	SetupViewport(window, renderer);

	// Game stuff
	SpriteManager spriteman(renderer, datfile);

	Renderer game_renderer(spriteman);
	GroundRenderer ground_renderer(renderer);

	Camera camera(Vector3f(0, 0, 0), SDL2pp::Rect(0, 0, 320, 200));

	// game_renderer has notified sprite manager of needed sprites,
	// now it will load them
	spriteman.LoadAll(
			[](int loaded, int total) {
				std::cerr << "Loading sprites: " << loaded << "/" << total << std::endl;
			}
		);

	LevelLoader level_loader;
	Game game = level_loader.Load(datfile, "LEVEL0", 12, 6); // sizes correspond to first level of Desert Strike
	Heli* heli = game.Spawn<Heli>(Vector2f(512 * 3 + 256, 512 + 256));

	unsigned int delta_ms, prev_ms, this_ms = SDL_GetTicks();
	while (1) {
		// Process events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return 0;
			} else if (event.type == SDL_WINDOWEVENT) {
				SetupViewport(window, renderer);
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:   heli->AddControlFlags(Heli::LEFT); break;
				case SDLK_RIGHT:  heli->AddControlFlags(Heli::RIGHT); break;
				case SDLK_UP:     heli->AddControlFlags(Heli::FORWARD); break;
				case SDLK_DOWN:	  heli->AddControlFlags(Heli::BACKWARD); break;
				case SDLK_z:	  heli->AddControlFlags(Heli::GUN); break;
				case SDLK_x:	  heli->AddControlFlags(Heli::HYDRA); break;
				case SDLK_c:	  heli->AddControlFlags(Heli::HELLFIRE); break;
				case SDLK_ESCAPE: case SDLK_q:
					return 0;
				}
			} else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:   heli->RemoveControlFlags(Heli::LEFT); break;
				case SDLK_RIGHT:  heli->RemoveControlFlags(Heli::RIGHT); break;
				case SDLK_UP:     heli->RemoveControlFlags(Heli::FORWARD); break;
				case SDLK_DOWN:	  heli->RemoveControlFlags(Heli::BACKWARD); break;
				case SDLK_z:	  heli->RemoveControlFlags(Heli::GUN); break;
				case SDLK_x:	  heli->RemoveControlFlags(Heli::HYDRA); break;
				case SDLK_c:	  heli->RemoveControlFlags(Heli::HELLFIRE); break;
				}
			}
		}

		// Update
		prev_ms = this_ms;
		this_ms = SDL_GetTicks();
		delta_ms = (prev_ms <= this_ms) ? (this_ms - prev_ms) : (std::numeric_limits<unsigned int>::max() - prev_ms + this_ms);

		game.Update(delta_ms);

		camera.SetTarget(heli->GetPos().Grounded() + Vector3f(120, 0, 0));

		// Render
		renderer.SetDrawColor(0, 0, 0);
		renderer.Clear();

		ground_renderer.Render(game, camera);
		game_renderer.Render(game, camera);

		renderer.Present();

		// Frame limiter
		SDL_Delay(1);
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

