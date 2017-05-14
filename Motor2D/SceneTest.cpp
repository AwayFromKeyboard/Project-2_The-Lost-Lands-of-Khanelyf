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
#include "Minimap.h"
#include "Object.h"
#include "Player.h"
#include "Boss_Axe_Knight.h"
#include "BrokenBuilding.h"
#include "ParticleManager.h"
#include "Fire.h"
#include "Escorted_NPC.h"

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
  
	if (enemy_waves_active)
	{
		if (start_waves_timer)
		{
			enemy_waves_timer.Start();
			start_waves_timer = false;
			enemy_wave_number = enemy_waves::first;
		}

		if (enemy_wave_number == enemy_waves::first && enemy_waves_timer.ReadSec() >= 60) {
			enemy_wave_number = enemy_waves::second;


		}
		else if (enemy_wave_number == enemy_waves::second && enemy_waves_timer.ReadSec() >= 180) {
			enemy_wave_number = enemy_waves::third;


		}
		else if (enemy_wave_number == enemy_waves::third && enemy_waves_timer.ReadSec() >= 300) {
			enemy_wave_number = enemy_waves::fourth;


		}
		else if (enemy_wave_number == enemy_waves::fourth && enemy_waves_timer.ReadSec() >= 420) {
			enemy_wave_number = enemy_waves::fifth;


		}
		else if (enemy_wave_number == enemy_waves::fifth && enemy_waves_timer.ReadSec() >= 540) {
			enemy_wave_number = enemy_waves::sixth;


		}
		else if (enemy_wave_number == enemy_waves::sixth && enemy_waves_timer.ReadSec() >= 660) {
			enemy_wave_number = enemy_waves::seventh;


		}
		else if (enemy_wave_number == enemy_waves::seventh && enemy_waves_timer.ReadSec() >= 780) {
			enemy_wave_number = enemy_waves::eighth;


		}
		else if (enemy_wave_number == enemy_waves::eighth && enemy_waves_timer.ReadSec() >= 900) {
			enemy_wave_number = enemy_waves::ninth;


		}
		else if (enemy_wave_number == enemy_waves::ninth && enemy_waves_timer.ReadSec() >= 1020) {
			enemy_wave_number = enemy_waves::tenth;

		}

		else if (enemy_wave_number == enemy_waves::tenth && enemy_waves_timer.ReadSec() >= 1020) {
			enemy_wave_number = enemy_waves::none;

			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
			{
				if ((*it)->type == entity_type::enemy_boss)
				{
					(*it)->move_to_player = true;
				}
			}
		}
	}

	return true;
}

bool SceneTest::Update(float dt)
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);
	
	UpdateCameraMovement();

	if (App->player->pause_status || App->player->audio_muted) {
		
		App->audio->PauseMusic();
	}else
		App->audio->ResumeMusic();
	
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
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_B) == key_down)
	{
		Barracks* barrack = (Barracks*)App->entity->CreateEntity(barracks, building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));

		if (App->questmanager->GetCurrentQuest()->type == quest_type::create && App->questmanager->GetCurrentQuest()->id == quest_id::quest_leader) {
			App->questmanager->GetCurrentQuest()->progress++;
		}
		create_barrack = false;
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_W) == key_down)
	{
		BossAxeKnight* boss_axe_knight = (BossAxeKnight*)App->entity->CreateEntity(boss, enemy_boss, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
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
	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_J) == key_down)
	{
		Entity* object_entity = App->entity->CreateEntity(provisions, object, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_N) == key_down)
	{
		BrokenBuilding* brokenbuilding = (BrokenBuilding*)App->entity->CreateEntity(broken_building, building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == key_down && App->debug_mode)
	{
		App->player->GetHero()->levelup_points += 5;
	}

	if (App->questmanager->GetCurrentQuest()->id == quest_id::quest_escort && !escortedNPC_created) {
		escortedNPC_created = true;
		EscortedNPC* escorted_npc = (EscortedNPC*)App->entity->CreateEntity(npc_escort, npc, App->map->MapToWorld(90, 70));
	}

	if (App->questmanager->GetCurrentQuest()->id == quest_id::quest_provisions && !mission_provision_created) {
		mission_provision_created = true;
		Entity* object_entity = App->entity->CreateEntity(provisions, object, App->map->MapToWorld(21, 69));
	}

	if (App->input->GetKey(SDL_SCANCODE_Z) == key_down)
	{
		enemy_wave_number = enemy_waves::none;

		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
		{
			if ((*it)->type == entity_type::enemy_boss)
			{
				(*it)->move_to_player = true;
			}
		}
	}
}

void SceneTest::IncreaseGold(int gold)
{
	this->gold += gold;
}
