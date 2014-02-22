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

			building_types_.emplace(std::make_pair(obj.type, BuildingType()));
		}
	}

	for (auto& type : building_types_) {
		type.second.width = thingsdata.GetWord(type.first + 2);
		type.second.height = thingsdata.GetWord(type.first + 4);

		int block_matrix_offset = thingsdata.GetWord(type.first + 6);

		for (int nblock = 0; nblock < (type.second.width / 16) * (type.second.height / 16); nblock++)
			type.second.blocks.push_back(thingsdata.GetWord(block_matrix_offset + nblock * 2));
	}
}

void DatLevel::ForeachBuildingInstance(const std::function<void(const BuildingInstance&)>& f) const {
	for (auto& bi : building_instances_)
		f(bi);
}

const DatLevel::BuildingType& DatLevel::GetBuildingType(unsigned short type) const {
	auto it = building_types_.find(type);
	if (it == building_types_.end())
		throw std::runtime_error("unknown building type");
	return it->second;
}
