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

#include <game/game.hh>
#include <game/visitor.hh>

#include <gameobjects/bullet.hh>
#include <gameobjects/rocket.hh>

#include <gameobjects/heli.hh>

Heli::Heli(Game& game) : GameObject(game) {
	age_ = 0;

	pos_.z = Constants::MaxHeight();

	guns_ = Constants::GunCapacity();
	hydras_ = Constants::HydraCapacity();;
	hellfires_ = Constants::HellfireCapacity();;

	hydra_at_left_ = true;
	hellfire_at_left_ = true;

	armor_ = Constants::ArmorCapacity();
	fuel_ = Constants::FuelCapacity();
	load_ = 0;

	gun_reload_ = hydra_reload_ = hellfire_reload_ = 0; // ready to fire

	control_flags_ = tick_control_flags_ = 0;
}

void Heli::Accept(Visitor& visitor) {
	visitor.Visit(*this);
}

void Heli::Update(unsigned int deltams) {
	UpdatePhysics(deltams);
	UpdateWeapons(deltams);

	// Post-update
	tick_control_flags_ = 0;

	age_ += deltams;
}

void Heli::UpdatePhysics(unsigned int deltams) {
	float delta_sec = deltams / 1000.0f;

	// Turning
	float turn_rate = (control_flags_ & FORWARD) ? Constants::AccelTurnRate() : Constants::StillTurnRate();

	if (control_flags_ & LEFT)
		direction_.RotateCCW(turn_rate * delta_sec);
	if (control_flags_ & RIGHT)
		direction_.RotateCW(turn_rate * delta_sec);

	// XXX: acceleration

	// XXX: position
	pos_ += vel_ * delta_sec;
}

void Heli::UpdateWeapons(unsigned int deltams) {
	// take key taps into account
	int combined_control_flags = control_flags_ | tick_control_flags_;

	// Cooldown
	if (gun_reload_ > 0)
		gun_reload_ -= deltams;
	if (hydra_reload_ > 0)
		hydra_reload_ -= deltams;
	if (hellfire_reload_ > 0)
		hellfire_reload_ -= deltams;

	// Process gunfire
	if (combined_control_flags & GUN && guns_ > 0 && gun_reload_ <= 0) {
		game_.Spawn<Bullet>(pos_ + Constants::GunOffset() * GetSectorDirection(), Direction3f(GetSectorDirection(), Constants::WeaponFirePitch()));

		guns_--;
		gun_reload_ = Constants::GunCooldown();
	}

	if (combined_control_flags & HYDRA && hydras_ > 0 && hydra_reload_ <= 0) {
		Vector3f mount_offset = Constants::RocketMountOffset();
		if (hydra_at_left_)
			mount_offset.y = -mount_offset.y;

		game_.Spawn<Rocket>(pos_ + mount_offset * GetSectorDirection(), Direction3f(GetSectorDirection(), Constants::WeaponFirePitch()), Rocket::HYDRA);

		hydras_--;
		hydra_reload_ = Constants::HydraCooldown();
		hydra_at_left_ = !hydra_at_left_;
	}

	if (tick_control_flags_ /* hellfires have no autofire */ & HELLFIRE && hellfires_ > 0 && hellfire_reload_ <= 0) {
		Vector3f mount_offset = Constants::RocketMountOffset();
		if (hellfire_at_left_)
			mount_offset.y = -mount_offset.y;

		game_.Spawn<Rocket>(pos_ + mount_offset * GetSectorDirection(), Direction3f(GetSectorDirection(), Constants::WeaponFirePitch()), Rocket::HELLFIRE);

		hellfires_--;
		hellfire_reload_ = Constants::HellfireCooldown();
		hellfire_at_left_ = !hellfire_at_left_;
	}

	// XXX: no-ammo case should emit clicking sound; this
	// probably should be done via emitting some kind of event
}
