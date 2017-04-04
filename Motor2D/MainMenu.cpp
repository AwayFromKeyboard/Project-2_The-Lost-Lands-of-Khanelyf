#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "MainMenu.h"
#include "j1Scene.h"
#include <sstream>
#include "j1Entity.h"
#include "GameObject.h"
#include "j1Collisions.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

bool MainMenu::Start()
{
	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	main_menu_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 0, 1166, 1920, 1080 };
	main_menu_image = (UI_Image*)main_menu_window->CreateImage(iPoint(0, 0), ui_r);

	SDL_ShowCursor(0);
	return true;
}

bool MainMenu::PreUpdate()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	return true;
}

bool MainMenu::Update(float dt)
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);

	UpdateCameraMovement();

	App->map->Draw();
	cursor->Set(iPoint(mouse.x, mouse.y), cursor_r);

	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	/*if (App->scene->GetCurrentScene() != App->scene->scene_test)
	{
	App->gui->DeleteElement(cursor);
	App->gui->DeleteElement(general_ui_window);
	App->gui->DeleteElement(gold_txt);
	}*/

	return true;
}

bool MainMenu::Load(pugi::xml_node &)
{
	return true;
}

bool MainMenu::Save(pugi::xml_node &) const
{
	return true;
}

void MainMenu::OnColl(Collider* col1, Collider* col2)
{
}