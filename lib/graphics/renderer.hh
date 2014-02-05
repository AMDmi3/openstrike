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

#ifndef RENDERER_HH
#define RENDERER_HH

#include <game/visitor.hh>

class Renderer;
class SpriteManager;
class DatFile;
class Heli;

class Renderer : public Visitor {
protected:
	SDL2pp::Renderer& renderer_;
	SpriteManager& spriteman_;
	unsigned int heli_sprite_ids_;
	unsigned int shadow_sprite_ids_;
	unsigned int rotor_sprite_ids_;

public:
	Renderer(SDL2pp::Renderer& renderer, DatFile& datfile, SpriteManager& spriteman);

	virtual void Visit(const Heli& heli);
};

#endif
