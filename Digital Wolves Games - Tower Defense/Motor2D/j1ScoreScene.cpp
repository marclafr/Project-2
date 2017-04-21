#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1MainMenu.h"
#include "j1SceneManager.h"
#include "j1ScoreScene.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIHUDPanelButtons.h"
#include "UIHUDDescription.h"
#include "UIHUDResources.h"
#include "UICheckbutton.h"
#include "UIGetEntitiesInfo.h"
#include "j1UIManager.h"

j1ScoreScene::j1ScoreScene() : j1Module()
{
	name.assign("scorescene");
}

// Destructor
j1ScoreScene::~j1ScoreScene()
{}

// Called before render is available
bool j1ScoreScene::Awake()
{
	LOG("Loading ScoreScene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1ScoreScene::Start()
{
	App->audio->PlayMusic("audio/music/Menu01.ogg");
	scene_changing = false;

	//BACKGROUND
	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background->Set({ 0, 0, 1336, 622 }, { 0, 1504, 1366, 622 });

	background = App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIIMAGE);
	background->Set({ 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });

	//BACK_MENU
	back_menu = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	back_menu->Set({ 1014, 638, 129, 36 }, { 1234, 1432, 129, 36 });
	back_menu->SetInteractive(true);

	//PLAY AGAIN
	play_again = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	play_again->Set({ 846, 637, 129, 37 }, { 1102, 1431, 129, 37 });
	play_again->SetInteractive(true);

	//SCORE
	score = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	score->Set({ 278, 696, 126, 55 }, { 0, 0, 0, 0 });
	score->SetInteractive(true);

	title_score = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_score->Set(316, 710, "Trophies");

	//ACHIEVEMENTS
	achievements = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	achievements->Set({ 416, 696, 123, 55 }, { 0, 0, 0, 0 });
	achievements->SetInteractive(true);

	title_achievements = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_achievements->Set(440, 710, "Achievements");

	//INVESTIGATIONS
	investigations = (UIButton*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UIBUTTON);
	investigations->Set({ 555, 696, 123, 55 }, { 0, 0, 0, 0 });
	investigations->SetInteractive(true);

	title_investigations = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
	title_investigations->Set(575, 710, "Investigations");

	if (App->scene->win == true)
	{
		title_win = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
		char text_score[256];
		sprintf_s(text_score, 256, " You Win!!  Score: %d", App->entity_manager->GetScore());
		title_win->Set(600, 200, text_score);
		App->audio->PlayMusic("audio/music/Main_Theme01.ogg");
	}

	if (App->scene->lose == true)
	{
		title_lose = (UILabel*)App->uimanager->addUIComponent(UICOMPONENT_TYPE::UILABEL);
		char text_score[256];
		sprintf_s(text_score, 256, " You Lose :(  Score: %d", App->entity_manager->GetScore());
		title_lose->Set(600, 200, text_score);
		App->audio->PlayMusic("audio/music/Lost_Game01.ogg");
	}


	return true;
}

// Called each loop iteration
bool j1ScoreScene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1ScoreScene::Update(float dt)
{
	
	//PLAY AGAIN
	if (play_again->GetStat() == SELECTED)
	{
		play_again->Set({ 842, 634, 137, 43 }, { 1085, 1385, 137, 43 });
	}
	if (play_again->GetStat() == UNSELECTED)
	{
		play_again->Set({ 846, 637, 129, 37 }, { 1102, 1431, 129, 37 });
	}
	if (play_again->GetStat() == CLICKL_UP)
	{
		App->scene_manager->ChangeScene(App->scene, this);
		scene_changing = true;
		App->scene->win = false;
		App->scene->lose = false;

	}

	//BACK MENU
	if (!scene_changing) 
	{
		if (back_menu->GetStat() == SELECTED)
		{
			back_menu->Set({ 1010, 635, 137, 42 }, { 1226, 1386, 137, 42 });
		}
		if (back_menu->GetStat() == UNSELECTED)
		{
			back_menu->Set({ 1014, 638, 129, 36 }, { 1234, 1432, 129, 36 });
		}
		if (back_menu->GetStat() == CLICKL_UP)
		{
			App->scene_manager->ChangeScene(App->main_menu, this);
			App->scene->win = false;
			App->scene->lose = false;
		}
	}

	//SCORE
	if (score->GetStat() == CLICKL_UP)
	{
		background->Set({ 0, 622, 1336, 144 }, { 0, 2131, 1366, 144 });
	}

	//ACHIEVEMENTS
	if (achievements->GetStat() == CLICKL_UP)
	{
		background->Set({ 0, 622, 1336, 144 }, { 0, 2278, 1366, 144 });
	}

	//INVESTIGATIONS
	if (investigations->GetStat() == CLICKL_UP)
	{
		background->Set({ 0, 622, 1336, 144 }, { 0, 2426, 1366, 144 });
	}

	return true;
}

// Called each loop iteration
bool j1ScoreScene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->scene_manager->ChangeScene(App->main_menu, this);

	return ret;
}

// Called before quitting
bool j1ScoreScene::CleanUp()
{
	LOG("Freeing  MainMenu");
	App->uimanager->CleanUp();
	return true;
}

