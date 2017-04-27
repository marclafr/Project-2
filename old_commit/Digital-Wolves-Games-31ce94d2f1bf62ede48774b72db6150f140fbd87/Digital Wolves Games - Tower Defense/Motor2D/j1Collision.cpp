#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Collision.h"

j1Collision::j1Collision()
{
	name.assign("collision");
}

j1Collision::~j1Collision() {}

bool j1Collision::Update(float dt)
{
	bool ret = true;
	std::vector<Entity*> unit1 = App->entity_manager->GetEntityVector();
	for (int i = 0; i < unit1.size(); i++)
	{
		if (unit1[i]->GetEntityType() == E_UNIT)
		{
			iPoint pos = App->map->WorldToMap(unit1[i]->GetX(), unit1[i]->GetY());
			Unit* unit_1 = (Unit*)unit1[i];
			//Check colisions between units
			std::vector<Entity*> unit2 = App->entity_manager->GetEntityVector();
			for (int j = 0; j < unit2.size(); j++)
			{
				if (unit1[i] != unit2[j])
				{
					if (DoUnitsIntersect((Unit*)unit1[i], (Unit*)unit2[j]) == true)
					{
						//Collision detected
						Unit* unit_2 = (Unit*)unit2[j];
						if (unit_1->GetPriority() > unit_2->GetPriority() && unit_1->GetAction() == A_IDLE && unit_2->GetAction() == A_IDLE)
							SplitUnits((Unit*)unit1[i]);
					}
				}

			}
		}
	}

	return ret;
}

bool j1Collision::AbleToBuild(iPoint pos)
{
	std::vector<Entity*> entities = App->entity_manager->GetEntityVector();
	for (int i = 0; i < entities.size(); i++)
	{
		iPoint tile = App->map->WorldToMap(entities[i]->GetX(), entities[i]->GetY());
		if (tile == App->map->WorldToMap(pos.x, pos.y))
		{
			return false;
		}
	}
	return true;
}

bool j1Collision::DoUnitsIntersect(Unit* unit1, Unit* unit2)
{
	float distance_x = unit1->GetX() - unit2->GetX();
	float distance_y = unit1->GetY() - unit2->GetY();
	return (sqrt(distance_x * distance_x + distance_y * distance_y) < unit1->GetUnitRadius() + unit2->GetUnitRadius());
}

iPoint j1Collision::FindClosestWalkable(Unit* unit)
{
	bool found = false;

	iPoint tile = App->map->WorldToMap(unit->GetX(), unit->GetY());
	iPoint origin = tile;

	int dist = 1;

	while (!found)
	{
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.x -= dist;
		tile.x -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		//diagonals
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.x += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		//back to origin and increment the tile distance
		tile.x--;
		tile.y++;
		dist++;
	}

	return App->map->MapToWorld(tile.x, tile.y);
}

void j1Collision::SplitUnits(Unit * unit1)
{
	unit1->GoTo(FindClosestWalkable(unit1));
}