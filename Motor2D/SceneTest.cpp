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
#include <sstream>
#include "j1Entity.h"
#include "Hero.h"
#include "GameObject.h"

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

bool SceneTest::Start()
{
	if (App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	debug_tex = App->tex->LoadTexture("maps/path2.png");

	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 1, 84, 800, 600 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);
	
	InitCameraMovement();

	troop = (Hero*)App->entity->CreateEntity(player);
	fPoint pos(App->map->MapToWorld(12, 0).x, App->map->MapToWorld(12, 0).y);
	troop->game_object->SetPos(pos);
	SDL_ShowCursor(0);
	return true;
}

bool SceneTest::PreUpdate()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) {
		App->pathfinding->CreatePath(App->map->WorldToMapPoint(troop->game_object->GetPos()), p);
	}

	return true;
}

bool SceneTest::Update(float dt)
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);
	
	UpdateCameraMovement();

	App->map->Draw();
	cursor->Set(iPoint(mouse.x, mouse.y), cursor_r);

	troop->path = App->pathfinding->GetPath();

	if (troop->path.size() > 0)
	{
		troop->state = unit_move;
	}

	for (uint i = 0; i < troop->path.size(); i++)
	{
		iPoint pos = App->map->MapToWorld(troop->path.at(i).x, troop->path.at(i).y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}
	
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

void SceneTest::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
