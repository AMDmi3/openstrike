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

#include <game/gameobject.hh>

class Game;
class Visitor;

class Heli : public GameObject {
protected:
	// Turn rate when the heli is not accelerating forward
	static constexpr float still_turn_rate_ = 360.0/2.25;

	// Turn rate when the heli is accelerating forward
	static constexpr float accel_turn_rate_ = 360.0/4.5;

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

	static constexpr int gun_capacity_ = 1178;
	static constexpr int hydra_capacity_ = 38;
	static constexpr int hellfire_capacity_ = 8;
	static constexpr int armor_capacity_ = 600; // or just default?
	static constexpr int fuel_capacity_ = 100;  // or just default?

	static constexpr int gun_cooldown_ = 100; // ???
	static constexpr int hydra_cooldown_ = 100; // ???
	static constexpr int hellfire_cooldown_ = 100; // ???

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

	float direction_;

	float x_pos_;
	float y_pos_;
	float height_;

	float x_vel_;
	float y_vel_;

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

	virtual void Accept(Visitor& visitor) const;
	virtual void Update(unsigned int deltams);

	virtual void UpdatePhysics(unsigned int deltams);
	virtual void UpdateWeapons(unsigned int deltams);

	float GetDirection() const {
		return direction_;
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

	float GetXPos() const {
		return x_pos_;
	}

	float GetYPos() const {
		return y_pos_;
	}

	float GetHeight() const {
		return height_;
	}
};

#endif // HELI_HH
