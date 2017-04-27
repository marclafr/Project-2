#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "j1SceneManager.h"
#include "j1Window.h"
#include "Camera.h"
#include "j1Render.h"
#include "j1UIManager.h"

#include "UIButton.h"

j1SceneManager::j1SceneManager() : j1Module()
{
	name.assign("scene_manager");
}

// Destructor
j1SceneManager::~j1SceneManager()
{}

bool j1SceneManager::Awake(pugi::xml_node& config)
{	
	//todo: assign pointers to scene modules
	current_scene = SC_MAIN_MENU;

	main_menu = (j1Module*) App->main_menu;
	game_scene = (j1Module*) App->scene;
	score_scene = (j1Module*) App->score_scene;

	return true;
}

bool j1SceneManager::Start()
{
	return true;
}

bool j1SceneManager::Update(float dt)
{
	return true;
}

bool j1SceneManager::PostUpdate()
{
	return true;
}


bool j1SceneManager::CleanUp()
{
	return true;
}


bool j1SceneManager::ChangeScene(SCENES change_to)
{
	bool ret = true;
	j1Module* current = GetCurrentScene();
	
	if(current != nullptr)
		current->Disable();

	switch (change_to)
	{
	case SC_MAIN_MENU:
		main_menu->Enable();
		current_scene = SC_MAIN_MENU;
		break;

	case SC_GAME:
		game_scene->Enable();
		current_scene = SC_GAME;
		break;

	case SC_SCORE:
		score_scene->Enable();
		current_scene = SC_SCORE;
		break;
	}

	//App->render->camera->FadeToBlack(0.05, 2, 1);

	return ret;
}

j1Module * j1SceneManager::GetCurrentScene() const
{
	switch (current_scene)
	{
	case SC_MAIN_MENU:
		return main_menu;

	case SC_GAME:
		return game_scene;

	case SC_SCORE:
		return score_scene;
	}
	return nullptr;
}