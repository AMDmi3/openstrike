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
	struct Constants {
		// Turn rate when the heli is not accelerating forward
		static constexpr float StillTurnRate() { return 2.0 * pi / 2.25; } // rad/sec

		// Turn rate when the heli is accelerating forward
		static constexpr float AccelTurnRate() { return 2.0 * pi / 4.5; } // rad/sec

		// Forward acceleration rate
		static constexpr float ForwardAccel() { return 0.0; }

		// Backward acceleration rate
		static constexpr float BackwardAccel() { return 0.0; }

		// Turning acceleration rate
		static constexpr float TurnAccel() { return 0.0; }

		// Jink acceleration rate
		static constexpr float JinkAccel() { return 0.0; }

		// Drag force
		static constexpr float Drag() { return 0.0; }

		// Max height
		static constexpr float MaxHeight() { return 20.0; }

		static constexpr int GunCapacity() { return 1178; }
		static constexpr int HydraCapacity() { return 38; }
		static constexpr int HellfireCapacity() { return 8; }
		static constexpr int ArmorCapacity() { return 600; } // or just default?
		static constexpr int FuelCapacity() { return 100; }  // or just default?

		static constexpr int GunCooldown() { return 250; } // Desert Strike (calculated 240 from video, rounded)
		static constexpr int HydraCooldown() { return 500; } // Desert Strike (calculated 474 from video, rounded)
		static constexpr int HellfireCooldown() { return 1000; } // Desert Strike (calculated 1150 from video - time to key press (no auto fire), rounded)

		static constexpr Vector3f GunOffset() { return Vector3f(33, -1, 7); }
		static constexpr float WeaponFirePitch() { return -0.121941; } // -atan(25/204), from Desert Strike

		static constexpr Vector3f RocketMountOffset() { return Vector3f(0, 10, 7); } // XXX: approx, check game

		static constexpr float GunDispersion() { return 0.5/180.0*pi; }
	};

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
	unsigned int age_;

	// movement
	Direction2f direction_;

	Vector3f pos_;
	Vector3f vel_;

	// payload
	int armor_;
	int fuel_;
	int load_;

	int guns_;
	int hydras_;
	int hellfires_;

	int gun_reload_; // ms
	int hydra_reload_; // ms
	int hellfire_reload_; // ms

	int hydra_at_left_;
	int hellfire_at_left_;

	// control
	int control_flags_;
	int tick_control_flags_;

public:
	Heli(Game& game, const Vector2f& pos);

	virtual void Accept(Visitor& visitor);
	virtual void Update(unsigned int deltams);

	virtual void UpdatePhysics(unsigned int deltams);
	virtual void UpdateWeapons(unsigned int deltams);

	unsigned int GetAge() const {
		return age_;
	}

	Direction2f GetDirection() const {
		return direction_;
	}

	Direction2f GetSectorDirection() const {
		return Direction2f((int)((direction_.yaw / pi * 12.0) + 0.5) * pi / 12.0);
	}

	Vector3f GetPos() const {
		return pos_;
	}

	void AddControlFlags(int flags) {
		control_flags_ |= flags;
		tick_control_flags_ |= flags;
	}

	void RemoveControlFlags(int flags) {
		control_flags_ &= ~flags;
	}

	int GetControlFlags() const {
		return control_flags_;
	}
};

#endif // HELI_HH
