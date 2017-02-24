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
#include "Test.h"
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

	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	movement_window_u = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 20, (int)App->win->_GetWindowSize().y, 100, true);
	SDL_Color color;
	color.r = 255;
	movement_window_u->SetDebugColor(color);
	movement_window_d = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, (int)App->win->_GetWindowSize().y - 20), 20, (int)App->win->_GetWindowSize().y, 100, true);

	troop = (Test*)App->entity->CreateEntity(test);
	troop->game_object->SetPos(fPoint(100,100));
	SDL_ShowCursor(0);
	return true;
}

bool SceneTest::PreUpdate()
{
	return true;
}

bool SceneTest::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == key_down)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == key_down)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat)
		App->render->camera.y += 5;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat)
		App->render->camera.y -= 5;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)
		App->render->camera.x += 5;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)
		App->render->camera.x -= 5;

	App->map->Draw();
	cursor_window->Set(iPoint(50, 10), 0, 0);
	int x, y;
	App->input->GetMousePosition(x, y);
	x -= App->render->camera.x;
	y -= App->render->camera.y;
	cursor->Set(iPoint(x, y), cursor_r);

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
