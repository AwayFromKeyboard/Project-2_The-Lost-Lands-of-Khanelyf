#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "SceneTest.h"
#include "j1Scene.h"
#include <sstream>
#include "j1Entity.h"
#include "Hero.h"
#include "Barbarian.h"
#include "Swordsman.h"
#include "Unit.h"
#include "j1Collisions.h"
#include "QuestManager.h"
#include "Barracks.h"
#include "BasicBuilding.h"
#include "Building.h"
#include "Object.h"

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

bool SceneTest::Start()
{
	if (App->map->Load("map.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	//LOAD FXs

	death_id = App->audio->LoadFx("audio/fx/Death.wav");
	death2_id = App->audio->LoadFx("audio/fx/Death2.wav");
	get_hit_id = App->audio->LoadFx("audio/fx/Get_hit.wav");
	get_hit2_id = App->audio->LoadFx("audio/fx/Get_hit2.wav");
	get_hit3_id = App->audio->LoadFx("audio/fx/Get_hit3.wav");
	get_hit4_id = App->audio->LoadFx("audio/fx/Get_hit4.wav");
	swords_clash_id = App->audio->LoadFx("audio/fx/Sword.wav");
	swords_clash2_id = App->audio->LoadFx("audio/fx/Sword2.wav");
	swords_clash3_id = App->audio->LoadFx("audio/fx/Sword3.wav");
	swords_clash4_id = App->audio->LoadFx("audio/fx/Sword4.wav");
	swords_clash5_id = App->audio->LoadFx("audio/fx/Sword5.wav");

	debug_tex = App->tex->LoadTexture("maps/path2.png");

	App->collisions->UpdateQuadtree();

	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 7, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 0, 88, 1680, 1050 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);

	InitCameraMovement();

	App->map->GetEntitiesSpawn();

	gold = 0;
	gold_txt = (UI_Text*)general_ui_window->CreateText({ 33, 1 }, App->font->default_15);

	human_resources_txt = (UI_Text*)general_ui_window->CreateText({ general_ui_window->GetRect().w / 15, 1 }, App->font->default_15);

	App->audio->PlayMusic("audio/music/main_game.ogg");

	SDL_ShowCursor(0);
	return true;
}

bool SceneTest::PreUpdate()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	CheckUnitCreation(p);
  

	return true;
}

bool SceneTest::Update(float dt)
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);
	
	UpdateCameraMovement();

	App->map->Draw();
	cursor->Set(iPoint(mouse.x, mouse.y), cursor_r);

	return true;
}

bool SceneTest::PostUpdate()
{

	return true;
}

bool SceneTest::CleanUp()
{
	return true;
}

bool SceneTest::Load(pugi::xml_node &)
{
	return true;
}

bool SceneTest::Save(pugi::xml_node &) const
{
	return true;
}

void SceneTest::OnColl(Collider* col1, Collider* col2)
{
}

void SceneTest::CheckUnitCreation(iPoint p)
{
	std::stringstream oss;
	oss << gold;
	gold_txt->SetText(oss.str());

	std::stringstream oss2;
	oss2 << current_human_resources << "/" << human_resources_max;
	human_resources_txt->SetText(oss2.str());

	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_A) == key_down)
	{
		Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}

	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_S) == key_down)
	{
		Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	else if (App->input->GetKey(SDL_SCANCODE_B) == key_down && gold >= 90 && create_barrack == true)
	{
		Barracks* barrack = (Barracks*)App->entity->CreateEntity(barracks, building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
		gold -= barrack->cost;
		if (App->questmanager->GetCurrentQuest()->type == quest_type::create && App->questmanager->GetCurrentQuest()->id == quest_id::quest_leader) {
			App->questmanager->GetCurrentQuest()->progress++;
		}
		create_barrack = false;
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_U) == key_down)
	{
		BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, enemy_building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y), 1);
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_I) == key_down)
	{
		BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, enemy_building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y), 2);
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_O) == key_down)
	{
		BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, enemy_building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y), 3);
	}
}

void SceneTest::IncreaseGold(int gold)
{
	this->gold += gold;
}
