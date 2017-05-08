#include "j1Animation.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "p2Log.h"
#include "j1Render.h"

j1Animation::j1Animation()
{
	name.assign("animation");
}

// Destructor
j1Animation::~j1Animation()
{
	for (int i = 0; i < animation_types.size(); i++)
	{
		animation_types[i]->CleanUp();
		delete animation_types[i];
	}

	animation_types.clear();
}

bool j1Animation::Start()
{
	return true;
}
bool j1Animation::Awake(pugi::xml_node& config)
{
	bool ret = true;

	std::string anim_folder = "animations/Units_data.xml";

	//Load UNIT animations data from animations folder
	char* buff = nullptr;
	int size = App->fs->Load(anim_folder.c_str(), &buff);
	pugi::xml_document anim_data;
	pugi::xml_parse_result result = anim_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading UNIT animations data: %s", result.description());
		return false;
	}

	//Loading units
	pugi::xml_node unit_node = anim_data.child("Sprites").first_child();

	while (unit_node != NULL)
	{
		pugi::xml_node action_node = unit_node.first_child();
		while (action_node != NULL)
		{
			pugi::xml_node direction_node = action_node.first_child();

			while (direction_node != NULL)
			{
				Animation* new_anim = new Animation(ANIM_UNIT);
				pugi::xml_node sprite_node = direction_node.first_child();

				while (sprite_node != NULL)
				{
					new_anim->frames.push_back({ sprite_node.attribute("x").as_int(),sprite_node.attribute("y").as_int(), sprite_node.attribute("w").as_int(),sprite_node.attribute("h").as_int() });

					float pX = sprite_node.attribute("w").as_int() * sprite_node.attribute("pX").as_float();
					float pY = sprite_node.attribute("h").as_int() * sprite_node.attribute("pY").as_float();
					pX = (pX > (floor(pX) + 0.5f)) ? ceil(pX) : floor(pX);
					pY = (pY > (floor(pY) + 0.5f)) ? ceil(pY) : floor(pY);
					new_anim->pivot_points.push_back({ (int)pX, (int)pY });

					sprite_node = sprite_node.next_sibling();
				}

				new_anim->SetUnit(unit_node);
				new_anim->SetAction(action_node);
				new_anim->SetDirection(direction_node);

				std::string action = action_node.name();

				if (!action.compare("disappear"))
				{
					new_anim->speed = 1000.0f;
					new_anim->loop = false;
				}

				if (!action.compare("die"))
				{
					new_anim->loop = false;
				}

				animation_types.push_back(new_anim);
				direction_node = direction_node.next_sibling();
			}
			action_node = action_node.next_sibling();
		}
		unit_node = unit_node.next_sibling();
	}

	//Load ARROW/BOMBS animations data from animations folder
	std::string anim_folder2 = "animations/ArrowsBombs.xml";

	buff = nullptr;
	size = App->fs->Load(anim_folder2.c_str(), &buff);
	pugi::xml_document anim_data2;
	result = anim_data2.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading ARROW and BOMBS animations data: %s", result.description());
		return false;
	}

	pugi::xml_node arrows_bombs_node = anim_data2.child("TextureAtlas").first_child();
	while (arrows_bombs_node != NULL)
	{
		std::string anim_name = arrows_bombs_node.attribute("anim_name").as_string();
		Animation* new_anim2 = new Animation(AnimString2Enum(anim_name));

		pugi::xml_node sprite_node2 = arrows_bombs_node.first_child();

		while (sprite_node2 != NULL)
		{
			new_anim2->frames.push_back({ sprite_node2.attribute("x").as_int(),sprite_node2.attribute("y").as_int(), sprite_node2.attribute("w").as_int(),sprite_node2.attribute("h").as_int() });

			int pX = sprite_node2.attribute("pX").as_float();
			int pY =  sprite_node2.attribute("pY").as_float();
			new_anim2->pivot_points.push_back({ pX, pY });
			sprite_node2 = sprite_node2.next_sibling();
		}

		new_anim2->speed = 75.0f; 
		if (new_anim2->name == ANIM_FIRE_FLOOR)
			new_anim2->loop = false;
		animation_types.push_back(new_anim2);
		arrows_bombs_node = arrows_bombs_node.next_sibling();
	}


	std::string anim_folder3 = "animations/Fire.xml";
	buff = nullptr;
	size = App->fs->Load(anim_folder3.c_str(), &buff);
	pugi::xml_document anim_data3;
	result = anim_data3.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading ARROW and BOMBS animations data: %s", result.description());
		return false;
	}

	pugi::xml_node fire_node = anim_data3.child("Sprites").first_child();

	while (fire_node != NULL)
	{
		std::string anim_name = fire_node.attribute("anim_name").as_string();
		Animation* new_anim3 = new Animation(AnimString2Enum(anim_name));

		pugi::xml_node sprite_node3 = fire_node.first_child();

		while (sprite_node3 != NULL)
		{
			new_anim3->frames.push_back({ sprite_node3.attribute("x").as_int(),sprite_node3.attribute("y").as_int(), sprite_node3.attribute("w").as_int(),sprite_node3.attribute("h").as_int() });

			int pX = sprite_node3.attribute("pX").as_int();
			int pY = sprite_node3.attribute("pY").as_int();
			new_anim3->pivot_points.push_back({ (int)pX, (int)pY });
			sprite_node3 = sprite_node3.next_sibling();
		}

		new_anim3->speed = 75.0f;
		animation_types.push_back(new_anim3);
		fire_node = fire_node.next_sibling();
	}

	return ret;
}


bool j1Animation::CleanUp()
{
	return true;
}

Animation* j1Animation::GetAnimationType(const ANIMATION_NAME name, const UNIT_TYPE unit, const ACTION action, const DIRECTION direction) const
{
	DIRECTION dir = direction;
	if (name == ANIM_UNIT)
	{
		switch (dir)
		{
		case D_NORTH_EAST:
			dir = D_NORTH_WEST;
			break;

		case D_EAST:
			dir = D_WEST;
			break;

		case D_SOUTH_EAST:
			dir = D_SOUTH_WEST;
			break;

		default:
			break;
		}

		for (int i = 0; i < animation_types.size(); i++)
			if (animation_types[i]->unit_type == unit && animation_types[i]->action == action && animation_types[i]->direction_type == dir)
				return animation_types[i];
	}

	for (int i = 0; i < animation_types.size(); i++)
	{
		if (animation_types[i]->GetName() == name)
			return animation_types[i];
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------//

Animation::Animation(ANIMATION_NAME name) : name(name)
{}

// Destructor
Animation::~Animation()
{}

void Animation::SetSpeed(float spd)
{
	speed = spd;
}

void Animation::SetLoopState(bool state)
{
	loop = state;
}

const int Animation::GetNumFrames() const
{
	return (frames.size() - 1);
}

const float Animation::GetSpeed() const
{
	return speed;
}

const bool Animation::GetLoopState() const
{
	return loop;
}

const SDL_Rect Animation::GetFrame(int frame_num) const
{
	//TODO: if frame_num> frames.size?????? it broke once
	return frames[frame_num];
}

const iPoint Animation::GetPivot(int frame_num) const
{
	return pivot_points[frame_num];
}

const ACTION Animation::GetActionType() const
{
	return action;
}

const UNIT_TYPE Animation::GetUnitType() const
{
	return unit_type;
}

const DIRECTION Animation::GetDirection() const
{
	return direction_type;
}

const ANIMATION_NAME Animation::GetName() const
{
	return name;
}

AnimationManager::AnimationManager() : anim_type(nullptr), current_frame(0.0f), anim_timer(j1Timer()), idle_wait_timer(j1Timer()), speed(START_SPEED), loop(true), wait_started(false), finished(false)
{}

AnimationManager::AnimationManager(Animation * type) : anim_type(type), wait_started(false)
{
	loop = type->GetLoopState();
	speed = type->GetSpeed();
	anim_timer.Start();
}

AnimationManager::AnimationManager(const AnimationManager & copy) : anim_type(copy.anim_type), current_frame(copy.current_frame), speed(copy.speed), loop(copy.loop), wait_started(copy.wait_started), finished(copy.finished)
{
	anim_timer.SetTicks(copy.anim_timer.Read());
	idle_wait_timer.SetTicks(copy.idle_wait_timer.Read());
}

AnimationManager::~AnimationManager()
{}

void AnimationManager::ChangeAnimation(Animation * type, float new_speed)
{
	anim_type = type;
	loop = type->GetLoopState();
	if (new_speed != 0.0f)
		speed = new_speed;
	else
		speed = type->GetSpeed();

	Reset();
}

const AnimationManager AnimationManager::operator = (const AnimationManager & anim)
{
	anim_type = anim.anim_type;
	current_frame = anim.current_frame;
	anim_timer.SetTicks(anim.anim_timer.Read());
	idle_wait_timer.SetTicks(anim.idle_wait_timer.Read());
	wait_started = anim.wait_started;
	speed = anim.speed;
	loop = anim.loop;
	finished = anim.finished;

	return *this;
}

bool AnimationManager::Update(SDL_Rect & rect, iPoint & pivot_point)
{
	if (finished == true && loop == true)
	{
		Reset();
	}
	
	rect = anim_type->GetFrame(current_frame);
	pivot_point = anim_type->GetPivot(current_frame);

	if (wait_started == false)
		current_frame = (float)floor(anim_timer.Read() / speed);

	return Finished();
}

bool AnimationManager::Finished()
{
	if (current_frame >= anim_type->GetNumFrames())
	{
		if (anim_type->GetName() == ANIM_UNIT)
		{
			if (anim_type->GetActionType() == A_IDLE)
			{
				if (wait_started == false)
				{
					wait_started = true;
					idle_wait_timer.Start();
				}

				if (idle_wait_timer.ReadSec() >= IDLE_ANIMATION_WAIT)
				{
					finished = true;
					wait_started = false;
					return true;
				}
			}
			else
			{
				finished = true;
				return true;
			}
		}
		//NON_UNIT
		else
		{
			finished = true;
			return true;
		}
	}
	return false;
}

void AnimationManager::Reset()
{
	current_frame = 0.0f;
	anim_timer.Start();
	finished = false;
	wait_started = false;
}

void Animation::SetUnit(const pugi::xml_node node)
{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
		//INFANTRY:
	if (strcmp(node.name(), "militia") == 0)
		unit_type = U_MILITIA;

	else if (strcmp(node.name(), "manatarms") == 0)
		unit_type = U_MANATARMS;

	else if (strcmp(node.name(), "longswordman") == 0)
		unit_type = U_LONGSWORDMAN;

	else if (strcmp(node.name(), "twohandedswordman") == 0)
		unit_type = U_TWOHANDEDSWORDMAN;

	else if (strcmp(node.name(), "champion") == 0)
		unit_type = U_CHAMPION;

	else if (strcmp(node.name(), "spearman") == 0)
		unit_type = U_SPEARMAN;

	else if (strcmp(node.name(), "pikeman") == 0)
		unit_type = U_PIKEMAN;

	//--------
		//ARCHERS
	else if (strcmp(node.name(), "archer") == 0)
		unit_type = U_ARCHER;

	else if (strcmp(node.name(), "arbalest") == 0)
		unit_type = U_ARBALEST;

	else if (strcmp(node.name(), "cavalryarcher") == 0)
		unit_type = U_CAVALRYARCHER;

	else if (strcmp(node.name(), "heavycavalryarcher") == 0)
		unit_type = U_HEAVYCAVALRYARCHER;

	//-------
		//CAVALRY	
	else if (strcmp(node.name(), "knight") == 0)
		unit_type = U_KNIGHT;

	else if (strcmp(node.name(), "cavalier") == 0)
		unit_type = U_CAVALIER;

	else if (strcmp(node.name(), "paladin") == 0)
		unit_type = U_PALADIN;
	//-------
		//SIEGE
	else if (strcmp(node.name(), "siegeram") == 0)
		unit_type = U_SIEGERAM;
	//-------

	//TODO: ADD UNIT

	else
	{
		unit_type = U_NO_UNIT;
		LOG("ERROR: XML Node UNIT TYPE does not match");
	}
}

void Animation::SetAction(const pugi::xml_node node)
{
	if (strcmp(node.name(), "attack") == 0)
		action = A_ATTACK;

	else if (strcmp(node.name(), "die") == 0)
		action = A_DIE;

	else if (strcmp(node.name(), "disappear") == 0)	//IF PROBLEM CHECK DIsAppEAR SPELLING!!!!!
		action = A_DISAPPEAR;

	else if (strcmp(node.name(), "idle") == 0)
		action = A_IDLE;

	else if (strcmp(node.name(), "walk") == 0)
		action = A_WALK;

	else
	{
		action = A_NO_ACTION;
		LOG("ERROR: XML Node ACTION TYPE does not match");
	}
}

void Animation::SetDirection(const pugi::xml_node node)
{
	if (strcmp(node.name(), "north") == 0)
		direction_type = D_NORTH;

	else if (strcmp(node.name(), "south") == 0)
		direction_type = D_SOUTH;

	else if (strcmp(node.name(), "south_west") == 0)
		direction_type = D_SOUTH_WEST;

	else if (strcmp(node.name(), "west") == 0)
		direction_type = D_WEST;

	else if (strcmp(node.name(), "north_west") == 0)
		direction_type = D_NORTH_WEST;

	else
	{
		direction_type = D_NO_DIRECTION;
		LOG("ERROR: XML Node DIRECTION TYPE does not match");
	}

}

ANIMATION_NAME j1Animation::AnimString2Enum(const std::string name)
{
	if (name == "simple_arrow")
		return ANIM_SIMPLE_ARROW;

	else if (name == "fire_arrow")
		return ANIM_FIRE_ARROW;

	else if (name == "ice_arrow")
		return ANIM_ICE_ARROW;

	else if (name == "air_arrow")
		return ANIM_AIR_ARROW;

	else if (name == "simple_bomb")
		return ANIM_SIMPLE_BOMB;

	else if (name == "fire_bomb")
		return ANIM_FIRE_BOMB;

	else if (name == "ice_bomb")
		return ANIM_ICE_BOMB;

	else if (name == "air_bomb")
		return ANIM_AIR_BOMB;

	else if (name == "fire_floor")
		return ANIM_FIRE_FLOOR;

	return NO_ANIM_NAME;
}

bool Animation::CleanUp()
{
	frames.clear();
	pivot_points.clear();

	return true;
}
