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
	debug_tex = App->tex->LoadTexture("maps/path2.png");

	App->collisions->UpdateQuadtree();

	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 1, 84, 800, 600 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);
	
	InitCameraMovement();

	App->map->GetEntitiesSpawn();

	gold = 1000;
	gold_txt = (UI_Text*)general_ui_window->CreateText({ 500, 25 }, App->font->default);

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
	/*if (App->scene->GetCurrentScene() != App->scene->scene_test)
	{
		App->gui->DeleteElement(cursor);
		App->gui->DeleteElement(general_ui_window);
		App->gui->DeleteElement(gold_txt);
	}*/

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
	oss << "Gold: " << gold;
	std::string txt = oss.str();
	gold_txt->SetText(txt);

	if (App->input->GetKey(SDL_SCANCODE_C) == key_down && gold >= TROOP_PRICE)
	{
		gold -= TROOP_PRICE;
		Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally);
		barb->game_object->SetPos(fPoint(App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).x, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y).y));
	}
}