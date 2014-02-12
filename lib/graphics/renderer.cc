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

#include <cassert>

#include <SDL2pp/Renderer.hh>

#include <gameobjects/heli.hh>
#include <graphics/spritemanager.hh>
#include <dat/datfile.hh>
#include <dat/datgraphics.hh>

#include <graphics/renderer.hh>

Renderer::Renderer(SDL2pp::Renderer& renderer, DatFile& datfile, SpriteManager& spriteman) : renderer_(renderer), spriteman_(spriteman) {
	{
		Buffer data;
		if (datfile.Exists("CM0000"))
			data = datfile.GetData("CM0000"); // Jungle Strike
		else
			data = datfile.GetData("AP0000"); // Desert Strike

		DatGraphics gfx(data);
		heli_sprite_ids_ = spriteman.Add(gfx, 0, 13);
	}

	{
		Buffer data = datfile.GetData("SHADOWS");
		DatGraphics gfx(data);
		shadow_sprite_ids_ = spriteman.Add(gfx, 0, 13);
	}

	{
		Buffer data = datfile.GetData("ROTOR");
		DatGraphics gfx(data);
		rotor_sprite_ids_ = spriteman.Add(gfx, 0, 8);
	}
}

void Renderer::Visit(const Heli& heli) {
	float direction = heli.GetDirection();
	while (direction < 0.0f)
		direction += 360.0f;

	int phase = (int)((direction + 7.5) / 15);

	phase = phase % 24;

	int spriteid = 0;
	int flipflags = 0;

	if (phase >= 13) {
		phase = 24 - phase;
		flipflags = SpriteManager::HFLIP;
	}

	assert(phase >= 0 && phase <= 12);

	int shadow_offset = 16 + heli.GetHeight() * 20; // from Desert Strike

	spriteman_.Render(shadow_sprite_ids_ + phase, 320, 240, SpriteManager::PIVOT_FRAMECENTER | flipflags);

	// XXX: shadow should be transparent
	spriteman_.Render(heli_sprite_ids_ + phase, 320, 240 - shadow_offset, SpriteManager::PIVOT_FRAMECENTER | flipflags);

	spriteman_.Render(rotor_sprite_ids_ + heli.GetRotorPhase() / 100 % 8, 320, 240 - shadow_offset, SpriteManager::PIVOT_FRAMECENTER);
}
