#ifndef __UIHUDPANELBUTTONS_H__
#define __UIHUDPANELBUTTONS_H__

#include "UIComponents.h"
#include "UIButton.h"

enum ENTITY_TYPE;
enum UNIT_TYPE;
enum BUILDING_TYPE;

struct info_button
{
	UIButton* btn;
	uint x;
	uint y;

	ENTITY_TYPE e_type;

	BUILDING_TYPE b_type;

	UNIT_TYPE u_type;

	void SetUnit(UNIT_TYPE type);
	void SetBuilding(BUILDING_TYPE type);
};

class UIHUDPanelButtons : public UIComponents
{
public:
	std::list<info_button*> panel;

	info_button* if_active = nullptr;

public:
	UIHUDPanelButtons(UICOMPONENT_TYPE type);

	~UIHUDPanelButtons();

	bool Update();

	//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
	info_button* AddButton(uint x, uint y, uint atlas_x, uint atlas_y);

	void CreateEntity();
};

#endif // __UIHUDPANELBUTTONS_H__