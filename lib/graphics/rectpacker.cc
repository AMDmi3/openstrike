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
#include <limits>

#include <graphics/rectpacker.hh>

RectPacker::RectPacker(int pagewidth, int pageheight) : page_width_(pagewidth), page_height_(pageheight), num_pages_(0) {
}

RectPacker::~RectPacker() {
}

const RectPacker::Rect& RectPacker::Place(int width, int height, int padding) {
	int alwidth = width + padding * 2;
	int alheight = height + padding * 2;

	if (alwidth > page_width_ || alheight > page_height_)
		throw std::logic_error("rect to large to fit into page");

	int area = alwidth * alheight;

	// Find rect with smallest wasted area
	RectList::iterator best_rect = free_rects_.end();
	int best_wasted_area = std::numeric_limits<int>::max();
	for (RectList::iterator current_rect = free_rects_.begin(); current_rect != free_rects_.end(); ++current_rect) {
		if (alwidth > current_rect->width || alheight > current_rect->height)
			continue;

		int wasted_area = current_rect->width * current_rect->height - area;
		if (wasted_area < best_wasted_area) {
			best_wasted_area = wasted_area;
			best_rect = current_rect;
		}
	}

	// If not found, allocate new page
	if (best_rect == free_rects_.end()) {
		free_rects_.emplace_back(Rect(num_pages_++, 0, 0, page_width_, page_height_));
		best_rect = --free_rects_.end();
	}

	// Safely allocate two more rects: one free for split and one used for result
	used_rects_.emplace_back();
	try {
		free_rects_.emplace_back();
	} catch (...) {
		used_rects_.pop_back();
		throw;
	}

	used_rects_.back().page = best_rect->page;
	used_rects_.back().x = best_rect->x + padding;
	used_rects_.back().y = best_rect->y + padding;
	used_rects_.back().width = width;
	used_rects_.back().height = height;

	// find split direction so resulting rectangle is more square
	if (best_rect->width - width - padding * 2 < best_rect->height - height - padding * 2) {
		// split into top/bottom
		free_rects_.back() = Rect(
				best_rect->page,
				best_rect->x,
				best_rect->y + alheight,
				best_rect->width,
				best_rect->height - alheight
			);
		*best_rect = Rect(
				best_rect->page,
				best_rect->x + alwidth,
				best_rect->y,
				best_rect->width - alwidth,
				alheight
			);
	} else {
		// split into left/right
		free_rects_.back() = Rect(
				best_rect->page,
				best_rect->x + alwidth,
				best_rect->y,
				best_rect->width - alwidth,
				best_rect->height
			);
		*best_rect = Rect(
				best_rect->page,
				best_rect->x,
				best_rect->y + alheight,
				alwidth,
				best_rect->height - alheight
			);
	}

	return used_rects_.back();
}
