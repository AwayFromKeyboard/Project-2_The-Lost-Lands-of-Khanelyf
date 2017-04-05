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
#include "GameObject.h"
#include "j1Collisions.h"
#include "Barracks.h"
#include "Building.h"

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

bool SceneTest::Start()
{
	if (App->map->Load("map_vertical_slice.tmx") == true)
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
	cursor_r = { 1, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 1, 84, 800, 600 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);

	//create_unit_button = (UI_Button*)...->CreateButton(iPoint(0, 0), 0, 0);
	//create_unit_button->AddImage("standard", { 0, 145, 60, 60 });
	//create_unit_button->SetImage("standard");
	//create_unit_button->AddImage("clicked", { 61, 145, 60, 60 });
	//create_unit_button->enabled = false;
	//
	//create_unit_button2 = (UI_Button*)...
	//create_unit_button2->AddImage("standard", { 0, 145, 60, 60 });
	//create_unit_button2->SetImage("standard");
	//create_unit_button2->AddImage("clicked", { 61, 145, 60, 60 });
	//create_unit_button2->enabled = false;
	//
	//create_unit_image = (UI_Image*)...
	//create_unit_image-> {0, 0, 255, 144};
	//create_unit_image->enabled = false;

	InitCameraMovement();

	App->map->GetEntitiesSpawn();

	gold = 100;
	gold_txt = (UI_Text*)general_ui_window->CreateText({ 33, 1 }, App->font->default_15);

	human_resources_txt = (UI_Text*)general_ui_window->CreateText({ 110, 1 }, App->font->default_15);

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
  
	//if () { // A barrack is selected
	//	create_unit_button->enabled = true;
	//	create_unit_button2->enabled = true;
	//	create_unit_image->enabled = true;
	//}
	//else if () { // There is not any barrack selected
	//	create_unit_button->enabled = false;
	//	create_unit_button2->enabled = false;
	//	create_unit_image->enabled = false
	//}

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
	//if (create_unit_button->MouseClickEnterLeft()) { // && barracks->create_barbarian == true
	//	create_unit_button->SetImage("clicked");
	//
	//	Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally);
	//	barb->game_object->SetPos(); // Barracks position
	//	gold -= barb->cost;
	//	current_human_resources += barb->human_cost;
	//}
	//if (create_unit_button->MouseClickOutLeft()) {
	//	create_unit_button->SetImage("standard");
	//}
	//
	//if (create_unit_button2->MouseClickEnterLeft()) { // && barracks->create_swordsman == true
	//	create_unit_button2->SetImage("clicked");
	//
	//	Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally);
	//	sword->game_object->SetPos(); // Barracks position
	//	gold -= sword->cost;
	//	current_human_resources += sword->human_cost;
	//}
	//if (create_unit_button2->MouseClickOutLeft()) {
	//	create_unit_button2->SetImage("standard");
	//}

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
	std::string txt = oss.str();
	gold_txt->SetText(txt);

	std::stringstream oss2;
	oss2 << current_human_resources << "/" << human_resources_max;
	std::string txt2 = oss2.str();
	human_resources_txt->SetText(txt2);

	if (App->input->GetKey(SDL_SCANCODE_A) == key_down && gold >= 5 && current_human_resources <= human_resources_max - 1)
	{
		Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally);
		barb->game_object->SetPos(fPoint(App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).x, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).y));
		gold -= barb->cost;
		current_human_resources += barb->human_cost;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == key_down && gold >= 10 && current_human_resources <= human_resources_max - 2)
	{
		Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally);
		sword->game_object->SetPos(fPoint(App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).x, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).y));
		gold -= sword->cost;
		current_human_resources += sword->human_cost;
	}
	else if (App->input->GetKey(SDL_SCANCODE_B) == key_down && gold >= 1)
	{
		Barracks* barrack = (Barracks*)App->entity->CreateEntity(barracks, building);
		barrack->game_object->SetPos(fPoint(App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).x, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).y));
		gold -= barrack->cost;
	}
}

void SceneTest::IncreaseGold(int gold)
{
	this->gold += gold;
}
