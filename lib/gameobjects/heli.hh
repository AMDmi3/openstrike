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

#ifndef HELI_HH
#define HELI_HH

#include <math/pi.hh>
#include <math/geom.hh>

#include <game/gameobject.hh>

class Game;
class Visitor;

class Heli : public GameObject {
protected:
	// Turn rate when the heli is not accelerating forward
	static constexpr float still_turn_rate_ = 2.0 * pi / 2.25; // rad/sec

	// Turn rate when the heli is accelerating forward
	static constexpr float accel_turn_rate_ = 2.0 * pi / 4.5; // rad/sec

	// Forward acceleration rate
	static constexpr float forward_accel_ = 0.0;

	// Backward acceleration rate
	static constexpr float backward_accel_ = 0.0;

	// Turning acceleration rate
	static constexpr float turn_accel_ = 0.0;

	// Jink acceleration rate
	static constexpr float jink_accel_ = 0.0;

	// Drag force
	static constexpr float drag_ = 0.0;

	// Max height
	static constexpr float maxheight_ = 20.0;

	static constexpr int gun_capacity_ = 1178;
	static constexpr int hydra_capacity_ = 38;
	static constexpr int hellfire_capacity_ = 8;
	static constexpr int armor_capacity_ = 600; // or just default?
	static constexpr int fuel_capacity_ = 100;  // or just default?

	static constexpr int gun_cooldown_ = 250; // Desert Strike (calculated 240 from video, rounded)
	static constexpr int hydra_cooldown_ = 500; // Desert Strike (calculated 474 from video, rounded)
	static constexpr int hellfire_cooldown_ = 1000; // Desert Strike (calculated 1150 from video - time to key press (no auto fire), rounded)

	static constexpr Vector3f gun_offset_ = Vector3f(33, -1, 7); // Desert Strike
	static constexpr float weapon_fire_pitch_ = -0.121941; // -atan(25/204), from Desert Strike

public:
	enum ControlFlags {
		LEFT     = 0x01,
		RIGHT    = 0x02,
		FORWARD  = 0x04,
		BACKWARD = 0x08,
		JINK     = 0x10,
		GUN      = 0x20,
		HYDRA    = 0x40,
		HELLFIRE = 0x80,
	};

protected:
	// movement
	unsigned int rotor_phase_;

	Direction2f direction_;

	Vector3f pos_;
	Vector3f vel_;

	// resources
	int guns_;
	int hydras_;
	int hellfires_;

	int armor_;
	int fuel_;
	int load_;

	int gun_reload_; // ms
	int hydra_reload_; // ms
	int hellfire_reload_; // ms

	// control
	int control_flags_;
	int tick_control_flags_;

public:
	Heli(Game& game);

	virtual void Accept(Visitor& visitor);
	virtual void Update(unsigned int deltams);

	virtual void UpdatePhysics(unsigned int deltams);
	virtual void UpdateWeapons(unsigned int deltams);

	Direction2f GetDirection() const {
		return direction_;
	}

	Vector3f GetPos() const {
		return pos_;
	}

	unsigned int GetRotorPhase() const {
		return rotor_phase_;
	}

	void AddControlFlags(int flags) {
		control_flags_ |= flags;
		tick_control_flags_ |= flags;
	}

	void RemoveControlFlags(int flags) {
		control_flags_ &= ~flags;
	}
};

#endif // HELI_HH
