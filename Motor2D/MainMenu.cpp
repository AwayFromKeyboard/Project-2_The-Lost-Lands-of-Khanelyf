#include "MainMenu.h"
#include "j1Scene.h"
#include "Log.h"
#include "j1Input.h"
#include "Functions.h"
#include "j1App.h"
#include "j1Gui.h"
#include "Parallax.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "j1Window.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

bool MainMenu::Init()
{
	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 1, 7, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);
	cursor_window->SetEnabledAndChilds(false);
	cursor->SetEnabled(false);

	main_menu_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 0, 1139, 1680, 1050 };
	main_menu_image = (UI_Image*)main_menu_window->CreateImage(iPoint(0, 0), ui_r, false);

	start_button = (UI_Button*)main_menu_window->CreateButton(iPoint(1089, 189), 283, 129, 0);
	start_button->AddImage("standard", { 0, 2190, 283, 129 });
	//start_button->AddImage("hover", { 331, 2190, 283, 129 });
	//start_button->AddImage("clicked", { 662, 2190, 283, 129 });
	start_button->SetImage("standard");
	main_menu_window->SetEnabledAndChilds(false);
	main_menu_image->SetEnabled(false);
	start_button->SetEnabled(false);
	
	return true;
}

bool MainMenu::Start()
{
	cursor_window->SetEnabledAndChilds(true);
	//cursor->SetEnabled(true);
	main_menu_window->SetEnabledAndChilds(true);
	//main_menu_image->SetEnabled(true);
	//start_button->SetEnabled(true);

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
	cursor->Set(iPoint(mouse.x, mouse.y), cursor_r);

	return true;
}

bool MainMenu::PostUpdate()
{
	if (start_button->MouseClickEnterLeft() == true)
		App->scene->ChangeScene((Scene*)App->scene->scene_test);

	return true;
}

bool MainMenu::CleanUp()
{
	cursor_window->SetEnabledAndChilds(false);
	main_menu_window->SetEnabledAndChilds(false);

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