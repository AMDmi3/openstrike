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

#include <dat/buffer.hh>
#include <dat/datfile.hh>
#include <dat/datlevel.hh>

#include <gameobjects/building.hh>

#include <game/levelloader.hh>

LevelLoader::LevelLoader() {
}

Game LevelLoader::Load(const DatFile& datfile, const std::string& levelname, int width_blocks, int height_blocks) {
	Game game(width_blocks * 512, height_blocks * 1024);

	Buffer level_data = datfile.GetData(levelname);
	Buffer things_data = datfile.GetData("THINGS");

	DatLevel level(level_data, things_data, width_blocks, height_blocks);

	level.ForeachBuildingInstance([&game, &level](const DatLevel::BuildingInstance& bi) {
		const DatLevel::BuildingType& type = level.GetBuildingType(bi.type);
		game.Spawn<Building>(Vector3f(bi.x, bi.y * 2, 0), bi.type, type.width, type.height);
	});

	for (auto& fn : building_instance_processors_)
		level.ForeachBuildingInstance(fn);

	for (auto& fn : building_type_processors_)
		level.ForeachBuildingType(fn);

	return game;
}

void LevelLoader::AddBuildingInstanceProcessor(const DatLevel::BuildingInstanceProcessor& fn) {
	building_instance_processors_.push_back(fn);
}

void LevelLoader::AddBuildingTypeProcessor(const DatLevel::BuildingTypeProcessor& fn) {
	building_type_processors_.push_back(fn);
}
