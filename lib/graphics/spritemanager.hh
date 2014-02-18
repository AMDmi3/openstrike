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

#ifndef SPRITEMANAGER_HH
#define SPRITEMANAGER_HH

#include <vector>
#include <map>
#include <functional>
#include <string>

#include <SDL2pp/Texture.hh>
#include <SDL2pp/Renderer.hh>

#include <graphics/rectpacker.hh>

class DatGraphics;
class DatFile;

class SpriteManager {
public:
	enum Flags {
		PIVOT_USEFRAME     = 0x01, // use frame rect instead of sprite rect for pivot
		PIVOT_USECENTER    = 0x02, // use center instead of corner for pivot

		PIVOT_IMAGECORNER  = 0,
		PIVOT_IMAGECENTER  = PIVOT_USECENTER,
		PIVOT_FRAMECORNER  = PIVOT_USEFRAME,
		PIVOT_FRAMECENTER  = PIVOT_USEFRAME | PIVOT_USECENTER,

		PIVOT_MASK         = PIVOT_USEFRAME | PIVOT_USECENTER,

		HFLIP_FRAME        = 0x04,
		HFLIP_SPRITE       = 0x08,

		HFLIP        = HFLIP_FRAME | HFLIP_SPRITE,
	};

public:
	typedef unsigned int sprite_id_t;
	typedef std::function<void(int,int)> LoadingStatusCallback;

public:
	class SingleSprite {
	protected:
		SpriteManager& manager_;
		sprite_id_t id_;
		int flags_;

	public:
		SingleSprite(SpriteManager& manager, const std::string& name, unsigned int frame, int flags = PIVOT_FRAMECENTER);

		void Render(int x, int y);
	};

	class DirectionalSprite {
	protected:
		SpriteManager& manager_;
		std::vector<sprite_id_t> ids_;
		int flags_;

	public:
		DirectionalSprite(SpriteManager& manager, const std::string& name, unsigned int startframe = 0, unsigned int nframes = 13, int flags = PIVOT_FRAMECENTER);

		void Render(int x, int y, float angle);
	};

	class Animation {
	protected:
		SpriteManager& manager_;
		std::vector<sprite_id_t> ids_;
		int flags_;

	public:
		Animation(SpriteManager& manager, int flags = PIVOT_FRAMECENTER);
		Animation(SpriteManager& manager, const std::string& name, unsigned int startframe, unsigned int nframes, int flags = PIVOT_FRAMECENTER);
		Animation(SpriteManager& manager, const std::string& name, const std::vector<unsigned int>& frames, int flags = PIVOT_FRAMECENTER);

		void AddFrames(const std::string& name, unsigned int startframe, unsigned int nframes = 1);
		void AddFrames(const std::string& name, const std::vector<unsigned int>& frames);

		void Render(int x, int y, unsigned int nframe);

		unsigned int GetNumFrames() const;
	};

protected:
	static const int atlas_page_width_;
	static const int atlas_page_height_;

protected:
	struct SpriteInfo {
		unsigned int width;
		unsigned int height;
		unsigned int xoffset;
		unsigned int yoffset;
		unsigned int framewidth;
		unsigned int frameheight;

		unsigned int atlaspage;
		unsigned int atlasx;
		unsigned int atlasy;

		bool loaded;

		std::string resource;
		unsigned int frame;

		SpriteInfo(const std::string& r, unsigned int f) : loaded(false), resource(r), frame(f) {
		}
	};

	typedef std::vector<SDL2pp::Texture> AtlasPageVector;
	typedef std::vector<SpriteInfo> SpriteInfoVector;
	typedef std::pair<std::string, int> SpriteLocation;
	typedef std::map<SpriteLocation, sprite_id_t> SpriteMap;

protected:
	SDL2pp::Renderer& renderer_;
	DatFile& datfile_;

	AtlasPageVector atlas_pages_;
	SpriteInfoVector sprites_;
	SpriteMap known_sprites_;

	RectPacker rect_packer_;

protected:
	sprite_id_t Add(const std::string& resource, unsigned int frame, bool load_immediately = false);
	void Render(sprite_id_t id, int x, int y, int flags);

	void Load(sprite_id_t id, const DatGraphics& graphics);
	void Load(sprite_id_t id);

public:
	SpriteManager(SDL2pp::Renderer& renderer, DatFile& datfile);
	~SpriteManager();

	void LoadAll(const LoadingStatusCallback& statuscb = nullptr);
};

#endif // SPRITEMANAGER_HH
