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
	debug_tex = App->tex->LoadTexture("maps/path2.png");

	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 1, 84, 800, 600 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);
	

	movement_window_u = (UI_Window*)App->gui->UI_CreateWin(iPoint(MOVEMENT_AREA, -1), App->win->_GetWindowSize().x - 2*MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_d = (UI_Window*)App->gui->UI_CreateWin(iPoint(MOVEMENT_AREA, App->win->_GetWindowSize().y - MOVEMENT_AREA), App->win->_GetWindowSize().x - 2*MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_l = (UI_Window*)App->gui->UI_CreateWin(iPoint(-1, MOVEMENT_AREA), MOVEMENT_AREA, App->win->_GetWindowSize().y - 2* MOVEMENT_AREA, 5);
	movement_window_r = (UI_Window*)App->gui->UI_CreateWin(iPoint(App->win->_GetWindowSize().x - MOVEMENT_AREA, MOVEMENT_AREA), MOVEMENT_AREA, App->win->_GetWindowSize().y - 2 * MOVEMENT_AREA, 5);
	movement_window_u_l = (UI_Window*)App->gui->UI_CreateWin(iPoint(-1, -1), MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_u_r = (UI_Window*)App->gui->UI_CreateWin(iPoint(App->win->_GetWindowSize().x - MOVEMENT_AREA, -1), MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_d_l = (UI_Window*)App->gui->UI_CreateWin(iPoint(-1, App->win->_GetWindowSize().y - MOVEMENT_AREA), MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_d_r = (UI_Window*)App->gui->UI_CreateWin(iPoint(App->win->_GetWindowSize().x - MOVEMENT_AREA, App->win->_GetWindowSize().y - MOVEMENT_AREA), MOVEMENT_AREA, MOVEMENT_AREA, 5);

	movement_window_u->SetEnabled(true);
	movement_window_d->SetEnabled(true);
	movement_window_l->SetEnabled(true);
	movement_window_r->SetEnabled(true);
	movement_window_u_l->SetEnabled(true);
	movement_window_u_r->SetEnabled(true);
	movement_window_d_l->SetEnabled(true);
	movement_window_d_r->SetEnabled(true);
	
	/*rect_u = (UI_ColoredRect*)movement_window_u->CreateColoredRect(movement_window_u->GetPosition(), movement_window_u->GetRect().w, movement_window_u->GetRect().h, { 255, 0, 0, 100 });
	rect_d = (UI_ColoredRect*)movement_window_d->CreateColoredRect(movement_window_d->GetPosition(), movement_window_d->GetRect().w, movement_window_d->GetRect().h, { 255, 0, 0, 100 });
	rect_l = (UI_ColoredRect*)movement_window_l->CreateColoredRect(movement_window_l->GetPosition(), movement_window_l->GetRect().w, movement_window_l->GetRect().h, { 255, 0, 0, 100 });
	rect_r = (UI_ColoredRect*)movement_window_r->CreateColoredRect(movement_window_r->GetPosition(), movement_window_r->GetRect().w, movement_window_r->GetRect().h, { 255, 0, 0, 100 });
	rect_u_l = (UI_ColoredRect*)movement_window_u_l->CreateColoredRect(movement_window_u_l->GetPosition(), movement_window_u_l->GetRect().w, movement_window_u_l->GetRect().h, { 0, 255, 0, 100 });
	rect_u_r = (UI_ColoredRect*)movement_window_u_r->CreateColoredRect(movement_window_u_r->GetPosition(), movement_window_u_r->GetRect().w, movement_window_u_r->GetRect().h, { 0, 255, 0, 100 });
	rect_d_l = (UI_ColoredRect*)movement_window_d_l->CreateColoredRect(movement_window_d_l->GetPosition(), movement_window_d_l->GetRect().w, movement_window_d_l->GetRect().h, { 0, 255, 0, 100 });
	rect_d_r = (UI_ColoredRect*)movement_window_d_r->CreateColoredRect(movement_window_d_r->GetPosition(), movement_window_d_r->GetRect().w, movement_window_d_r->GetRect().h, { 0, 255, 0, 100 });*/

	troop = (Test*)App->entity->CreateEntity(test);
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
	if (movement_window_u->MouseEnter() || App->input->GetKey(SDL_SCANCODE_UP) == key_repeat) {
		App->render->camera.y += CAMERA_SPEED;
	}
	if (movement_window_d->MouseEnter() || App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat) {
		App->render->camera.y -= CAMERA_SPEED;
	}
	if (movement_window_l->MouseEnter() || App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat) {
		App->render->camera.x += CAMERA_SPEED;
	}
	if (movement_window_r->MouseEnter() || App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat) {
		App->render->camera.x -= CAMERA_SPEED;
	}
	if (movement_window_u_l->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat && App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)) {
		App->render->camera.x += CAMERA_SPEED;
		App->render->camera.y += CAMERA_SPEED;
	}
	if (movement_window_u_r->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat && App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)) {
		App->render->camera.x -= CAMERA_SPEED;
		App->render->camera.y += CAMERA_SPEED;
	}
	if (movement_window_d_l->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat && App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)) {
		App->render->camera.x += CAMERA_SPEED;
		App->render->camera.y -= CAMERA_SPEED;
	}
	if (movement_window_d_r->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat && App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)) {
		App->render->camera.x -= CAMERA_SPEED;
		App->render->camera.y -= CAMERA_SPEED;
	}

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
		troop->end_movement = false;
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
