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

#include <stdexcept>

#include <dat/buffer.hh>

#include <dat/datgraphics.hh>

DatGraphics::DatGraphics(const MemRange& data) : data_(data) {
	// Get section ranges (GRAPHICS, SPRITES and PALETTE)
	Slice graphics_section = data.GetSlice(0, FileStruct::Graphics::size);

	if (graphics_section.GetString(0, 8) != "GRAPHICS")
		throw std::logic_error("bad graphics file (expected GRAPHICS section)");

	transparency_ = graphics_section.GetByte(FileStruct::Graphics::offs_transparency_flag);

	size_t sprites_length = graphics_section.GetDWord(FileStruct::Graphics::offs_sprites_length);

	Slice sprites_section = data.GetSlice(FileStruct::Graphics::size, sprites_length);
	Slice palette_section = data.GetSlice(FileStruct::Graphics::size + sprites_length);

	// Parse SPRITES
	if (sprites_section.GetString(0, 8) == "SPRITES ") {
		if (sprites_section.GetDWord(FileStruct::Sprites::Header::offs_blocks_unk0) != 0)
			throw std::logic_error("probably BLOCKS file, not supported yet");

		int num_sprites = sprites_section.GetWord(FileStruct::Sprites::Header::offs_num_sprites);

		for (int i = 0; i < num_sprites; i++) {
			Slice sprite_entry(sprites_section.GetSlice(FileStruct::Sprites::Header::size + i * FileStruct::Sprites::Entry::size, FileStruct::Sprites::Entry::size));

			Sprite sprite;

			sprite.framewidth = sprite_entry.GetWord(FileStruct::Sprites::Entry::offs_frame_width);
			sprite.frameheight = sprite_entry.GetWord(FileStruct::Sprites::Entry::offs_frame_height);
			sprite.xoffset = sprite_entry.GetWord(FileStruct::Sprites::Entry::offs_x_offset);
			sprite.yoffset = sprite_entry.GetWord(FileStruct::Sprites::Entry::offs_y_offset);
			sprite.width = sprite_entry.GetWord(FileStruct::Sprites::Entry::offs_width);
			sprite.height = sprite_entry.GetWord(FileStruct::Sprites::Entry::offs_height);

			size_t this_data_offset = sprite_entry.GetDWord(FileStruct::Sprites::Entry::offs_data_offset);

			if (i < num_sprites - 1) {
				Slice next_sprite_entry(sprites_section.GetSlice(FileStruct::Sprites::Header::size + (i + 1) * FileStruct::Sprites::Entry::size, FileStruct::Sprites::Entry::size));
				size_t next_data_offset = next_sprite_entry.GetDWord(FileStruct::Sprites::Entry::offs_data_offset);
				sprite.data = sprites_section.GetSlice(this_data_offset, next_data_offset - this_data_offset);
			} else {
				sprite.data = sprites_section.GetSlice(this_data_offset); // till the end of sprites section
			}

			sprites_.push_back(sprite);
		}
	} else if (sprites_section.GetString(0, 8) == "PICTURE ") {
		Sprite sprite;

		sprite.framewidth = sprite.width = sprites_section.GetWord(FileStruct::Picture::Header::offs_width);
		sprite.frameheight = sprite.height = sprites_section.GetWord(FileStruct::Picture::Header::offs_height);
		sprite.xoffset = sprite.yoffset = 0;
		sprite.data = sprites_section.GetSlice(FileStruct::Picture::Header::size, sprite.width * sprite.height);

		sprites_.push_back(sprite);
	} else {
		throw std::logic_error("unknown graphics file");
	}

	// Parse PALETTE
	if (palette_section.GetString(0, 8) != "PALETTE ")
		throw std::logic_error("bad graphics file (expected PALETTE section)");

	int num_colors = palette_section.GetWord(FileStruct::Palette::Header::offs_num_colors);

	for (int i = 0; i < num_colors; i++) {
		Color c;
		c.red = FixColor(palette_section.GetByte(FileStruct::Palette::Header::size + i * 3));
		c.green = FixColor(palette_section.GetByte(FileStruct::Palette::Header::size + i * 3 + 1));
		c.blue = FixColor(palette_section.GetByte(FileStruct::Palette::Header::size + i * 3 + 2));

		palette_.push_back(c);
	}
}

unsigned int DatGraphics::GetNumSprites() const {
	return sprites_.size();
}

unsigned short DatGraphics::GetWidth(unsigned int num) const {
	return sprites_[num].width;
}

unsigned short DatGraphics::GetHeight(unsigned int num) const {
	return sprites_[num].height;
}

unsigned short DatGraphics::GetFrameWidth(unsigned int num) const {
	return sprites_[num].framewidth;
}

unsigned short DatGraphics::GetFrameHeight(unsigned int num) const {
	return sprites_[num].frameheight;
}

unsigned short DatGraphics::GetXOffset(unsigned int num) const {
	return sprites_[num].xoffset;
}

unsigned short DatGraphics::GetYOffset(unsigned int num) const {
	return sprites_[num].yoffset;
}

std::vector<unsigned char> DatGraphics::GetPixels(unsigned int num) const {
	size_t pixels_size = sprites_[num].width * sprites_[num].height * 4;

	std::vector<unsigned char> pixels(pixels_size, 0);

	const Slice& data = sprites_[num].data;

	size_t data_pos = 0;
	size_t out_pos = 0;
	size_t pixel_in_line = 0;
	unsigned char mask;
	size_t width = sprites_[num].width;

	if (transparency_) {
		while (data_pos < data.GetSize() && out_pos + 4 <= pixels_size) {
			mask = data[data_pos++];

			// optimize by computing max. available number of iterations
			for (int j = 0; j < 8 && pixel_in_line < width && data_pos < data.GetSize() && out_pos + 4 <= pixels_size; j++, pixel_in_line++) {
				if (mask & (0x80 >> j)) {
					unsigned char color = data[data_pos++];
					if (color >= palette_.size())
						throw std::logic_error("color not found in the palette");

					pixels[out_pos++] = palette_[color].blue;
					pixels[out_pos++] = palette_[color].green;
					pixels[out_pos++] = palette_[color].red;
					pixels[out_pos++] = 255;
				} else {
					pixels[out_pos++] = 0;
					pixels[out_pos++] = 0;
					pixels[out_pos++] = 0;
					pixels[out_pos++] = 0;
				}
			}

			if (pixel_in_line == width)
				pixel_in_line = 0;
		}
	} else {
		for (; data_pos < data.GetSize() && out_pos + 4 <= pixels_size; ) {
			unsigned char color = data[data_pos++];
			if (color >= palette_.size())
				throw std::logic_error("color not found in the palette");

			pixels[out_pos++] = palette_[color].blue;
			pixels[out_pos++] = palette_[color].green;
			pixels[out_pos++] = palette_[color].red;
			pixels[out_pos++] = 255;
		}
	}

	return pixels;
}
