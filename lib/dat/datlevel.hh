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

#ifndef DATLEVEL_HH
#define DATLEVEL_HH

#include <vector>
#include <list>
#include <map>
#include <functional>

#include <dat/buffer.hh>

class MemRange;

class DatLevel {
public:
	struct BuildingInstance {
		unsigned short type;
		unsigned short x;
		unsigned short y;
	};

	struct BuildingType {
		unsigned short width;
		unsigned short height;

		std::vector<unsigned short> blocks;
	};

protected:
	std::list<BuildingInstance> building_instances_;
	std::map<unsigned int, BuildingType> building_types_;

public:
	DatLevel(const MemRange& leveldata, const MemRange& thingsdata, int width_blocks, int height_blocks);

	void ForeachBuildingInstance(const std::function<void(const BuildingInstance&)>& f) const;
	const BuildingType& GetBuildingType(unsigned short type) const;
};

#endif // DATLEVEL_HH
