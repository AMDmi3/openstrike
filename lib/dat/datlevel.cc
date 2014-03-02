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
#include <iostream>

#include <dat/datlevel.hh>

// this is stored in the code; not sure if it'd be better to extract
// if from there instead of just keeping this table here
const std::map<unsigned short, std::string> DatLevel::gfx_resources_ = {
	{ 0xb024, "HANGAR" },
	{ 0xb026, "COMMAND" },
	{ 0xb028, "MIG23" },
	{ 0xb02a, "FRIGATE" },
	{ 0xb02c, "BARKS" },
	{ 0xb02e, "BLDG1" },
	{ 0xb032, "BLDGS23" },
	{ 0xb034, "BRADLEY" },
	{ 0xb038, "BUNKER" },
	{ 0xb03c, "FENCE" },
	{ 0xb03e, "GUARDS" },
	{ 0xb042, "NOMADS" },
	{ 0xb044, "PWIRES" },
	{ 0xb046, "PWPOLES" },
	{ 0xb048, "RESCUEBO" },
	{ 0xb04a, "ROADS" },
	{ 0xb04c, "ROCKS" },
	{ 0xb050, "SUBSTATI" },
	{ 0xb052, "TENTS" },
	{ 0xb054, "TOWER" },
	{ 0xb064, "JAIL" },
	{ 0xb066, "NMEHQ" },
	{ 0xb068, "POW" },
	{ 0xb06e, "BLDG6" },
};

DatLevel::DatLevel(const MemRange& leveldata, const MemRange& thingsdata, int width_blocks, int height_blocks) {
	for (int nblock = 0; nblock < width_blocks * height_blocks; nblock++) {
		int blockdata_offset = leveldata.GetWord(nblock * 2);
		int data_count = leveldata.GetWord(blockdata_offset);

		for (int ndata = 0; ndata < data_count; ndata++) {
			int data_offset = leveldata.GetWord(blockdata_offset + 2 + 2 * ndata);

			BuildingInstance obj;
			obj.type = leveldata.GetWord(data_offset);
			obj.y = leveldata.GetWord(data_offset + 2) * 8;
			obj.x = leveldata.GetWord(data_offset + 4) * 8;
			obj.bbox_y = leveldata.GetWord(data_offset + 6);
			obj.bbox_x = leveldata.GetWord(data_offset + 8);

			building_instances_.emplace_back(obj);

			building_types_.emplace(std::make_pair(obj.type, BuildingType()));
		}
	}

	for (auto& type : building_types_) {
		unsigned short blocks_identifier = thingsdata.GetWord(type.first);

		type.second.width = thingsdata.GetWord(type.first + 2);
		type.second.height = thingsdata.GetWord(type.first + 4);

		auto resource = gfx_resources_.find(blocks_identifier);
		type.second.resource_name = (resource == gfx_resources_.end()) ? "" : resource->second;

		if (resource == gfx_resources_.end()) {
			std::cerr << "Warning: no resource name for object type " << blocks_identifier << std::endl;
		}

		int block_matrix_offset = thingsdata.GetWord(type.first + 6);

		for (int nblock = 0; nblock < (type.second.width / 16) * (type.second.height / 16); nblock++)
			type.second.blocks.push_back(thingsdata.GetWord(block_matrix_offset + nblock * 2));

		int nbboxes = thingsdata.GetWord(type.first + 20);

		for (int nbbox = 0; nbbox < nbboxes; nbbox++) {
			int bbox_offset = type.first + 22 + nbbox * 12;
			BuildingType::BBox bbox;
			bbox.y1 = thingsdata.GetWord(bbox_offset + 0);
			bbox.x1 = thingsdata.GetWord(bbox_offset + 2);
			bbox.y2 = thingsdata.GetWord(bbox_offset + 4);
			bbox.x2 = thingsdata.GetWord(bbox_offset + 6);
			bbox.z1 = thingsdata.GetWord(bbox_offset + 8);
			bbox.z2 = thingsdata.GetWord(bbox_offset + 10);

			type.second.bboxes.emplace_back(bbox);
		}
	}
}

void DatLevel::ForeachBuildingInstance(const BuildingInstanceProcessor& fn) const {
	for (auto& bi : building_instances_)
		fn(bi);
}

void DatLevel::ForeachBuildingType(const BuildingTypeProcessor& fn) const {
	for (auto& bt : building_types_)
		fn(bt.first, bt.second);
}

const DatLevel::BuildingType& DatLevel::GetBuildingType(unsigned short type) const {
	auto it = building_types_.find(type);
	if (it == building_types_.end())
		throw std::runtime_error("unknown building type");
	return it->second;
}
