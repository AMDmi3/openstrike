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

#include <set>

#include <dat/datlevel.hh>

DatLevel::DatLevel(const MemRange& leveldata, const MemRange& thingsdata, int width_blocks, int height_blocks) {
	std::set<int> known_building_types;

	for (int nblock = 0; nblock < width_blocks * height_blocks; nblock++) {
		int blockdata_offset = leveldata.GetWord(nblock * 2);
		int data_count = leveldata.GetWord(blockdata_offset);

		for (int ndata = 0; ndata < data_count; ndata++) {
			int data_offset = leveldata.GetWord(blockdata_offset + 2 + 2 * ndata);

			BuildingInstance obj;
			obj.type = leveldata.GetWord(data_offset);
			obj.x = leveldata.GetWord(data_offset + 8);
			obj.y = leveldata.GetWord(data_offset + 6);

			building_instances_.emplace_back(obj);

			known_building_types.emplace(obj.type);
		}
	}

	for (auto& building_type_offset : known_building_types) {
		BuildingType type;
		type.width = thingsdata.GetWord(building_type_offset + 2);
		type.height = thingsdata.GetWord(building_type_offset + 4);

		int block_matrix_offset = thingsdata.GetWord(building_type_offset + 6);

		for (int nblock = 0; nblock < (type.width / 16) * (type.height / 16); nblock++)
			type.blocks.push_back(thingsdata.GetWord(block_matrix_offset + nblock * 2));

		building_types_.emplace_back(std::move(type));
	}
}

void DatLevel::ForeachBuildingInstance(const std::function<void(const BuildingInstance&)>& f) const {
	for (auto& bi : building_instances_)
		f(bi);
}
