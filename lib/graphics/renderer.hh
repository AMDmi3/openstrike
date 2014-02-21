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

#include <memory>

#include <game/visitor.hh>
#include <graphics/spritemanager.hh>

class SpriteManager;
class DatFile;
class Game;
class Camera;

class Renderer {
protected:
	SDL2pp::Renderer& renderer_;

	std::unique_ptr<SpriteManager::DirectionalSprite> sprite_heli_[12];
	SpriteManager::DirectionalSprite sprite_shadow_;
	SpriteManager::Animation sprite_rotor_;
	SpriteManager::SingleSprite sprite_bullet_;
	SpriteManager::DirectionalSprite sprite_hydra_;
	SpriteManager::DirectionalSprite sprite_hellfire_;
	SpriteManager::Animation sprite_explo_gun_object_;
	SpriteManager::Animation sprite_explo_gun_ground_;
	SpriteManager::Animation sprite_explo_hydra_;
	SpriteManager::Animation sprite_explo_hellfire_;
	SpriteManager::Animation sprite_explo_boom_;

protected:
	class RenderVisitor : public Visitor {
	protected:
		Renderer& parent_;
		const Camera& camera_;

	public:
		RenderVisitor(Renderer& parent, const Camera& camera);

		virtual void Visit(GameObject& obj);

		virtual void Visit(Heli& heli);
		virtual void Visit(Bullet& bullet);
		virtual void Visit(Rocket& rocket);
		virtual void Visit(Explosion& explosion);
	};

protected:
	std::unique_ptr<SpriteManager::DirectionalSprite>& GetHeliSprite(int forward, int side);

public:
	Renderer(SpriteManager& spriteman);

	void Render(Game& game, const Camera& camera);
};

#endif
