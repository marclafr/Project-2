#ifndef __UIHUDPANELBUTTONS_H__
#define __UIHUDPANELBUTTONS_H__

#include "UIComponents.h"
#include "UIButton.h"

class Task;
enum ENTITY_TYPE;
enum UNIT_TYPE;
enum BUILDING_TYPE;
enum TOWER_TYPE;
enum Side;

enum BUILDING_PANELINFO
{
	BP_NONE,
	BP_NO_SELECTION,
	BP_TOWNHALL,
	BP_UNIVERSITY,
	BP_TURRET,
	BP_CANNON,
	BP_TURRET_UPGRADED,
	BP_WOOD_WALL,
	BP_STONE_WALL,
	BP_BRICK_WALL
};

struct info_button
{
private:
	UIButton* btn;
	iPoint position;
	iPoint atlas;
	Task* task;
	bool delete_entity;

public:
	info_button(iPoint position, iPoint atlas, Task* task, bool delete_entity) : position(position), atlas(atlas), task(task), delete_entity(delete_entity) {}
	~info_button()
	{
		DELETE_PTR(task);
	}

	void CreateButton();
	void ButtonToDelete();
	const UIButton* GetButton() const;
	const Task* GetTask() const;
	bool IsForDelete();
};

class UIHUDPanelButtons : public UIComponents
{
private:
	std::vector<info_button*> panel_no_selection;
	std::vector<info_button*> panel_townhall;
	std::vector<info_button*> panel_university;
	std::vector<info_button*> panel_turret;
	std::vector<info_button*> panel_cannon;
	std::vector<info_button*> panel_turret_upgraded;
	std::vector<info_button*> panel_wood_wall;
	std::vector<info_button*> panel_stone_wall;
	std::vector<info_button*> panel_brick_wall;
	BUILDING_PANELINFO panel_type = BP_NONE;
	Building* b_selected = nullptr;

public:
	UIHUDPanelButtons(UICOMPONENT_TYPE type);
	~UIHUDPanelButtons();

	void SetPanel(Building* building);
	bool Update();
	//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
	info_button* AddButton(BUILDING_PANELINFO type,iPoint position, iPoint atlas, Task* task, bool delete_button = false);

	void CreatePanel();
	void DeletePanel();
};

#endif // __UIHUDPANELBUTTONS_H__