#include "Defs.h"
#include "Log.h"
#include "MainMenu.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1Scene.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

bool MainMenu::Start()
{
	cursor_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), 37, 40, 100, true);
	cursor_r = { 38, 1, 37, 40 };
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 1, 685, 1127, 767 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);

	single_player_button = (UI_Button*)general_ui_window->CreateButton(iPoint(469, 20), 182, 234);
	single_player_button->AddImage("standard", { 1, 1453, 179, 234 });
	single_player_button->AddImage("highlight", { 181, 1453, 182, 234 });
	single_player_button->AddImage("clicked", { 364, 1453, 180, 234 });
	single_player_button->SetImage("standard");
	single_player_button->SetEnabled(true);
	
	SDL_ShowCursor(0);
	
	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);
		
	cursor->Set(iPoint(mouse.x, mouse.y), cursor_r);

	CheckSinglePlayerButton();
	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	if (App->scene->GetCurrentScene() != App->scene->main_menu)
	{
		App->gui->DeleteElement(cursor);
		App->gui->DeleteElement(general_ui_window);
	}

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

void MainMenu::CheckSinglePlayerButton()
{
	if (single_player_button->MouseClickOutLeft())
	{
		App->scene->ChangeScene((Scene*)App->scene->scene_test);
	}
	else if (single_player_button->MouseClickEnterLeft())
	{
		single_player_button->SetImage("clicked");
	}
	if (single_player_button->MouseEnter())
	{
		single_player_button->SetImage("highlight");
	}
	else if (single_player_button->MouseOut()) {
		single_player_button->SetImage("standard");
	}
}
