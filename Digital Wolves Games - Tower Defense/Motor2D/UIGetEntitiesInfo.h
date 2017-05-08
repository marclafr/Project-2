#ifndef __GETENTITYINFO__
#define __GETENTITYINFO__

#define TURRET_BUILD 11000
#define STONEWALL_BUILD 11000

#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Resources.h"
#include "Towers.h"
#include "j1Investigations.h"

SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type);
const char* GetUnitName(const UNIT_TYPE type);
const char* GetUnitPrice(const UNIT_TYPE type);

SDL_Rect GetResourceIconPositionFromAtlas(const RESOURCE_TYPE type);
const char* GetResourceName(const RESOURCE_TYPE type);

SDL_Rect GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type);
const char* GetBuildingName(const BUILDING_TYPE type);
const char* GetBuildingPrice(const BUILDING_TYPE type);

const uint GetBuildingPercentage(const Entity* build);
const double GetBuildTotalTime(const BUILDING_TYPE type);
const bool isBuilded(const Entity* build);

SDL_Rect GetInvestigationIconPositionFromAtlas(const INVESTIGATION_TYPE type);
const uint GetInvestigationPercentage(const Investigation* type);
const char* GetInvestigationName(const INVESTIGATION_TYPE type);
const char* GetInvestigationPrice(const INVESTIGATION_TYPE type);

SDL_Rect GetTowerIconPositionFromAtlas(const TOWER_TYPE type);
const char* GetTowerName(const TOWER_TYPE type);

const char* GetSideName(const Side type);

int ReturnValueBarHPUnit(const UNIT_TYPE type, const uint hp, const uint pixels = 32);
int ReturnValueBarHPBuilding(const BUILDING_TYPE type, const uint hp, const uint pixels = 32);
int ReturnValueBarHPResource(const RESOURCE_TYPE type, const uint hp, const uint pixels = 32);


int ReturnValueHeightCorrectionUnit(const UNIT_CLASS u_class);
int ReturnValueHeightCorrectionBuilding(const BUILDING_TYPE type);
int ReturnValueHeightCorrectionResource(const RESOURCE_TYPE type);
int ReturnValueHeightCorrectionTowers(const TOWER_TYPE type);
#endif