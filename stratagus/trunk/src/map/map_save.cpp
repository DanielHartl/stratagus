//       _________ __                 __
//      /   _____//  |_____________ _/  |______     ____  __ __  ______
//      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
//      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ |
//     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
//             \/                  \/          \//_____/            \/
//  ______________________                           ______________________
//                        T H E   W A R   B E G I N S
//         Stratagus - A free fantasy real time strategy game engine
//
/**@name map_save.cpp - Saving the map. */
//
//      (c) Copyright 2001-2005 by Lutz Sammer and Jimmy Salmon
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; only version 2 of the License.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//      02111-1307, USA.
//
//      $Id$

//@{

/*----------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "stratagus.h"
#include "map.h"
#include "tileset.h"
#include "minimap.h"
#include "player.h"
#include "unit.h"
#include "pathfinder.h"
#include "ui.h"

#include "script.h"

/*----------------------------------------------------------------------------
-- Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------*/

/**
** Save the complete map.
**
** @param file Output file.
*/
void SaveMap(CFile *file)
{
	int w;
	int h;
	int i;

	file->printf("\n--- -----------------------------------------\n");
	file->printf("--- MODULE: map $Id$\n");

	file->printf("LoadTileModels(\"%s\")\n\n", Map.TileModelsFileName);
	
	file->printf("StratagusMap(\n");

	file->printf("  \"version\", \"" StratagusFormatString "\",\n",
		StratagusFormatArgs(StratagusVersion));
	file->printf("  \"description\", \"%s\",\n", Map.Info.Description);

	file->printf("  \"the-map\", {\n");

	file->printf("  \"size\", {%d, %d},\n", Map.Info.MapWidth, Map.Info.MapHeight);
	file->printf("  \"%s\",\n", Map.NoFogOfWar ? "no-fog-of-war" : "fog-of-war");
	file->printf("  \"filename\", \"%s\",\n", Map.Info.Filename);

	file->printf("  \"map-fields\", {\n");
	for (h = 0; h < Map.Info.MapHeight; ++h) {
		file->printf("  -- %d\n", h);
		for (w = 0; w < Map.Info.MapWidth; ++w) {
			MapField* mf;

			mf = &Map.Fields[h * Map.Info.MapWidth + w];
			file->printf("  {%3d, %3d,", mf->Tile, mf->SeenTile);
			if (mf->Value) {
				file->printf(" %d,", mf->Value);
			}
			for (i = 0; i < PlayerMax; ++i) {
				if (mf->Visible[i] == 1) {
					file->printf(" \"explored\", %d,", i);
				}
			}
			if (mf->Flags & MapFieldHuman) {
				file->printf(" \"human\",");
			}
			if (mf->Flags & MapFieldLandAllowed) {
				file->printf(" \"land\",");
			}
			if (mf->Flags & MapFieldCoastAllowed) {
				file->printf(" \"coast\",");
			}
			if (mf->Flags & MapFieldWaterAllowed) {
				file->printf(" \"water\",");
			}
			if (mf->Flags & MapFieldNoBuilding) {
				file->printf(" \"mud\",");
			}
			if (mf->Flags & MapFieldUnpassable) {
				file->printf(" \"block\",");
			}
			if (mf->Flags & MapFieldWall) {
				file->printf(" \"wall\",");
			}
			if (mf->Flags & MapFieldRocks) {
				file->printf(" \"rock\",");
			}
			if (mf->Flags & MapFieldForest) {
				file->printf(" \"wood\",");
			}
#if 1
			// Not Required for save
			// These are required for now, UnitType::FieldFlags is 0 until
			// UpdateStats is called which is after the game is loaded
			if (mf->Flags & MapFieldLandUnit) {
				file->printf(" \"ground\",");
			}
			if (mf->Flags & MapFieldAirUnit) {
				file->printf(" \"air\",");
			}
			if (mf->Flags & MapFieldSeaUnit) {
				file->printf(" \"sea\",");
			}
			if (mf->Flags & MapFieldBuilding) {
				file->printf(" \"building\",");
			}
#endif
			if (w & 1) {
				file->printf("},\n");
			} else {
				file->printf("}, ");
			}
		}
	}
	file->printf("}})\n");
}

//@}
