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

#ifndef DATGRAPHICS_HH
#define DATGRAPHICS_HH

#include <vector>

#include <dat/buffer.hh>

class DatGraphics {
protected:
	struct Sprite {
		unsigned short width;
		unsigned short height;
		unsigned short xoffset;
		unsigned short yoffset;
		unsigned short framewidth;
		unsigned short frameheight;
		Slice data;
	};

	struct Color {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};

protected:
	const MemRange& data_;

	std::vector<Sprite> sprites_;
	std::vector<Color> palette_;

protected:
	struct FileStruct {
		struct Graphics {
			static const int size = 32;
			static const int offs_sprites_length = 12;
			static const int offs_blocks_unk0 = 16;
			static const int offs_blocks_unk1 = 20;
		};
		struct Sprites {
			struct Header {
				static const int size = 16;
				static const int offs_num_sprites = 8;
			};
			struct Entry {
				static const int size = 16;
				static const int offs_frame_width = 0;
				static const int offs_frame_height = 2;
				static const int offs_x_offset = 4;
				static const int offs_y_offset = 6;
				static const int offs_width = 8;
				static const int offs_height = 10;
				static const int offs_data_offset = 12;
			};
		};
		struct Palette {
			struct Header {
				static const int size = 32;
				static const int offs_num_colors = 8;
			};
		};
	};

protected:
	inline unsigned char FixColor(unsigned char color) {
		return (color << 2) | (color >> 4);
	}

public:
	DatGraphics(const MemRange& data);

	unsigned int GetNumSprites() const;

	unsigned short GetWidth(unsigned int num) const;
	unsigned short GetHeight(unsigned int num) const;
	unsigned short GetFrameWidth(unsigned int num) const;
	unsigned short GetFrameHeight(unsigned int num) const;
	unsigned short GetXOffset(unsigned int num) const;
	unsigned short GetYOffset(unsigned int num) const;

	std::vector<unsigned char> GetPixels(unsigned int num) const;
};

#endif // DATGRAPHICS_HH
