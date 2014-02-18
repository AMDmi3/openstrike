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
#include <iostream>

#include <game/game.hh>
#include <graphics/spritemanager.hh>

#include <gameobjects/heli.hh>
#include <gameobjects/bullet.hh>
#include <gameobjects/rocket.hh>
#include <gameobjects/explosion.hh>

#include <graphics/renderer.hh>

Renderer::Renderer(SpriteManager& spriteman)
	: sprite_shadow_(spriteman, "SHADOWS"),
	  sprite_rotor_(spriteman, "ROTOR", 0, 8),
	  sprite_bullet_(spriteman, "WEAPONS", 0),
	  sprite_hydra_(spriteman, "WEAPONS", 1),
	  sprite_hellfire_(spriteman, "WEAPONS", 14),
	  sprite_explo_gun_object_(spriteman, "SMALLARM", 0, 6),
	  sprite_explo_gun_ground_(spriteman, "SMALLARM", 6, 6),
	  sprite_explo_hydra_(spriteman, "EXPLODE", {14, 15, 19, 20, 21}), // XXX: from Desert Strike; yes, explosion anims are non-contigous
	  sprite_explo_hellfire_(spriteman, "EXPLODE", {14, 15, 0, 1, 2, 22, 1, 0, 19, 20, 21}),
	  sprite_explo_boom_(spriteman, "EXPLODE", {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}) {

	// Load all heli sprite sets
	std::string heli_letters = "AP"; // XXX: only Desert Strike for now
	std::string forward_letters[] = { "B1", "00", "F1", "F2" };
	std::string side_letters[] = { "RL", "00", "RR" };

	for (int forward = -1; forward <= 2; forward++)
		for (int side = -1; side <= 1; side++)
			GetHeliSprite(forward, side).reset(new SpriteManager::DirectionalSprite(spriteman, heli_letters + forward_letters[forward + 1] + side_letters[side + 1]));
}

void Renderer::Render(Game& game) {
	// XXX: this should be split into two phases:
	// - get all (renderable? visible?) objects from game
	// - sort them
	// - render each
	game.Accept(*this);
}

void Renderer::Update(unsigned int) {
}

void Renderer::Visit(GameObject&) {
	static bool warning_displayed = false;
	if (!warning_displayed)
		std::cerr << "Warning: Renderer::Visit() not implemented for some objects" << std::endl;
	warning_displayed = true;
}

std::unique_ptr<SpriteManager::DirectionalSprite>& Renderer::GetHeliSprite(int forward, int side) {
	assert(forward >= -1 && forward <= 2 && side >= -1 && side <= 1);
	return sprite_heli_[(forward + 1) * 3 + (side + 1)];
}

void Renderer::Visit(Heli& heli) {
	int shadow_offset = 16 + heli.GetPos().z; // sprite offset from Desert Strike

	int sprite_forward = 0;
	int sprite_side = 0;

	if (heli.GetControlFlags() & Heli::LEFT)
		sprite_side = -1;
	if (heli.GetControlFlags() & Heli::RIGHT)
		sprite_side = 1;
	if (heli.GetControlFlags() & Heli::BACKWARD)
		sprite_forward = -1;
	if (heli.GetControlFlags() & Heli::FORWARD)
		sprite_forward = 2;

	// take mirroring into account
	if (heli.GetSectorDirection().yaw > pi * 1.01)
		sprite_side = -sprite_side;

	// XXX: shadow should be transparent
	sprite_shadow_.Render(40, 100, heli.GetDirection().yaw);
	GetHeliSprite(sprite_forward, sprite_side)->Render(40, 100 - shadow_offset, heli.GetDirection().yaw);
	sprite_rotor_.Render(40, 100 - shadow_offset, heli.GetAge() / 30);
}

void Renderer::Visit(Bullet& bullet) {
	Vector3f pos = bullet.GetPos();
	sprite_bullet_.Render(40 + pos.x, 100 - pos.y / 2 - pos.z);
}

void Renderer::Visit(Rocket& rocket) {
	Vector3f pos = rocket.GetPos();
	switch (rocket.GetType()) {
	case Rocket::HYDRA:
		sprite_hydra_.Render(40 + pos.x, 100 - pos.y / 2 - pos.z, rocket.GetDirection().yaw);
		break;
	case Rocket::HELLFIRE:
		sprite_hellfire_.Render(40 + pos.x, 100 - pos.y / 2 - pos.z, rocket.GetDirection().yaw);
		break;
	}
}

void Renderer::Visit(Explosion& explosion) {
	Vector3f pos = explosion.GetPos();

	SpriteManager::Animation* anim;

	switch (explosion.GetType()) {
	case Explosion::GUN_OBJECT: anim = &sprite_explo_gun_object_; break;
	case Explosion::GUN_GROUND: anim = &sprite_explo_gun_ground_; break;
	case Explosion::HYDRA:      anim = &sprite_explo_hydra_; break;
	case Explosion::HELLFIRE:   anim = &sprite_explo_hellfire_; break;
	case Explosion::BOOM:       anim = &sprite_explo_boom_; break;
	default:
		assert(false);
		return;
	}

	anim->Render(40 + pos.x, 100 - pos.y / 2, std::min(anim->GetNumFrames() - 1, (unsigned int)(anim->GetNumFrames() * explosion.GetAge())));
}
