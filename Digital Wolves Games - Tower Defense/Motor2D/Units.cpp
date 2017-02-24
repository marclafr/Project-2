
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "Units.h"


Unit::Unit(UNIT_TYPE u_type, iPoint pos): Entity(UNIT, pos), unit_type(u_type), direction(EAST), action_type(IDLE)
{}

void Unit::Update()
{
	AI();
	Move();
	Draw();
}

void Unit::Move()
{
}

void Unit::AI()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		action_type = ATTACK;

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
		action_type = IDLE;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		action_type = WALK;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		action_type = DISAPPEAR;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		action_type = DIE;
}

void Unit::Draw()
{
	SDL_Texture* tex = App->anim->GetTexture(unit_type);
	SDL_Rect rect;
	iPoint pivot;

	App->anim->GetAnimationFrame(rect, pivot, this);

	if (direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST)
		App->render->Blit(tex, GetX(), GetY(), &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y);
	else
		App->render->Blit(tex, GetX() - pivot.x, GetY() - pivot.y, &rect);
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const ACTION_TYPE Unit::GetActionType() const
{
	return action_type;
}
