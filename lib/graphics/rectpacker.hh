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

#ifndef RECTPACKER_HH
#define RECTPACKER_HH

#include <list>

class RectPacker {
public:
	struct Rect {
		int page;
		int x;
		int y;
		int width;
		int height;

		Rect() : page(-1), x(0), y(0), width(0), height(0) {
		}

		Rect(int npage, int nx, int ny, int nwidth, int nheight) : page(npage), x(nx), y(ny), width(nwidth), height(nheight) {
		}
	};

private:
	typedef std::list<Rect> RectList;

private:
	const int page_width_;
	const int page_height_;

	int num_pages_;

	RectList free_rects_;
	RectList used_rects_;

public:
	RectPacker(int pagewidth, int pageheight);
	~RectPacker();

	const Rect& Place(int width, int height, int padding = 0);
};

#endif // RECTPACKER_HH
