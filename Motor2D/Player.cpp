#include "Player.h"
#include <sstream>
#include "j1App.h"
#include "j1Input.h"
#include "Unit.h"
#include "j1Gui.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1Entity.h"
#include "Defs.h"
#include "Hero.h"
#include "Log.h"
#include "Barbarian.h"
#include "Swordsman.h"
#include "SceneTest.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "Object.h"
#include "Building.h"
#include "Minimap.h"
#include "Barracks.h"
#include "Blacksmith.h"
#include "BasicBuilding.h"
#include "Functions.h"
#include "QuestManager.h"
#include "j1Audio.h"
#include "ParticleManager.h"
#include "Particle.h"

Player::Player()
{
	name = "";
}

Player::~Player()
{
}

bool Player::Start()
{
	bool ret = true;

	help_window = (UI_Window*)App->gui->UI_CreateWin({ 0 + (App->win->_GetWindowSize().x / 40), (App->win->_GetWindowSize().y) - (App->win->_GetWindowSize().y /6) }, 0, 0, 100);

	helping_txt = (UI_Text*)help_window->CreateText({ 0 + (App->win->_GetWindowSize().x / 40), (App->win->_GetWindowSize().y) - (App->win->_GetWindowSize().y / 6) }, App->font->default_15);
	helping_txt->click_through = true;
	
	help_window->SetEnabledAndChilds(false);

	victory_window = (UI_Window*)App->gui->UI_CreateWin({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 9), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3)-(App->win->_GetWindowSize().y /10) }, 100, 200, 100);
	
	victory = (UI_Text*)victory_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x /9), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3)- (App->win->_GetWindowSize().y / 10) }, App->font->default_50);
	victory->SetText("Victory is yours!");
		
	lose = (UI_Text*)victory_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x /14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3) - (App->win->_GetWindowSize().y / 10) }, App->font->default_50);
	lose->SetText("You Lose");
	
	victory_window->SetEnabledAndChilds(false);
	//pause menu

	pause_window = (UI_Window*)App->gui->UI_CreateWin({ (App->win->_GetWindowSize().x/2)- (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y/2)- (App->win->_GetWindowSize().y / 3) },240, 386, 12);
	
	controls_bg = (UI_Image*)pause_window->CreateImage({ (App->win->_GetWindowSize().x / 3) + (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3) }, { 271,2300,310,499 });
	pause_bg= (UI_Image*)pause_window->CreateImage({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3) }, {0,2300,240,386});

	create_group_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 12) - (App->win->_GetWindowSize().y / 300) }, 186, 31);
	create_group_button->AddImage("standard", { 592, 2722, 228, 21 });
	create_group_button->SetImage("standard");
	create_group_button->AddImage("clicked", { 592, 2770, 228, 21 });
	create_group_button->AddImage("hovered", { 592, 2746, 228, 21 });

	move_map_right_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 9) + (App->win->_GetWindowSize().y / 500) }, 186, 31);
	move_map_right_button->AddImage("standard", { 592, 2722, 228, 21 });
	move_map_right_button->SetImage("standard");
	move_map_right_button->AddImage("clicked", { 592, 2770, 228, 21 });
	move_map_right_button->AddImage("hovered", { 592, 2746, 228, 21 });

	move_map_left_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 8) - (App->win->_GetWindowSize().y / 200) }, 186, 31);
	move_map_left_button->AddImage("standard", { 592, 2722, 228, 21 });
	move_map_left_button->SetImage("standard");
	move_map_left_button->AddImage("clicked", { 592, 2770, 228, 21 });
	move_map_left_button->AddImage("hovered", { 592, 2746, 228, 21 });

	move_map_down_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) + (App->win->_GetWindowSize().y / 70) }, 186, 31);
	move_map_down_button->AddImage("standard", { 592, 2722, 228, 21 });
	move_map_down_button->SetImage("standard");
	move_map_down_button->AddImage("clicked", { 592, 2770, 228, 21 });
	move_map_down_button->AddImage("hovered", { 592, 2746, 228, 21 });

	move_map_up_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) - (App->win->_GetWindowSize().y / 100) }, 186, 31);
	move_map_up_button->AddImage("standard", { 592, 2722, 228, 21 });
	move_map_up_button->SetImage("standard");
	move_map_up_button->AddImage("clicked", { 592, 2770, 228, 21 });
	move_map_up_button->AddImage("hovered", { 592, 2746, 228, 21 });

	charge_button_options = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) }, 186, 31);
	charge_button_options->AddImage("standard", { 592, 2722, 228, 21 });
	charge_button_options->SetImage("standard");
	charge_button_options->AddImage("clicked", { 592, 2770, 228, 21 });
	charge_button_options->AddImage("hovered", { 592, 2746, 228, 21 });

	whirlwind_button_options = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) - (App->win->_GetWindowSize().y / 40) }, 186, 31);
	whirlwind_button_options->AddImage("standard", { 592, 2722, 228, 21 });
	whirlwind_button_options->SetImage("standard");
	whirlwind_button_options->AddImage("clicked", { 592, 2770, 228, 21 });
	whirlwind_button_options->AddImage("hovered", { 592, 2746, 228, 21 });

	battlecry_button_options = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) }, 186, 31);
	battlecry_button_options->AddImage("standard", { 592, 2722, 228, 21 });
	battlecry_button_options->SetImage("standard");
	battlecry_button_options->AddImage("clicked", { 592, 2770, 228, 21 });
	battlecry_button_options->AddImage("hovered", { 592, 2746, 228, 21 });

	backoptions = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 14), (App->win->_GetWindowSize().y / 2) + (App->win->_GetWindowSize().y / 17) }, 186, 31);
	backoptions->AddImage("standard", { 592, 2722, 228, 21 });
	backoptions->SetImage("standard");
	backoptions->AddImage("clicked", { 592, 2770, 228, 21 });
	backoptions->AddImage("hovered", { 592, 2746, 228, 21 });

	audio_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) }, 186, 31);
	audio_button->AddImage("standard", { 25, 2695, 186, 31 });
	audio_button->SetImage("standard");
	audio_button->AddImage("clicked", { 25, 2768, 186, 31 });
	audio_button->AddImage("hovered", { 26, 2732, 186, 31 });

	controls_button = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) - (App->win->_GetWindowSize().y / 97) }, 186, 31);
	controls_button->AddImage("standard", { 25, 2695, 186, 31 });
	controls_button->SetImage("standard");
	controls_button->AddImage("clicked", { 25, 2768, 186, 31 });
	controls_button->AddImage("hovered", { 26, 2732, 186, 31 });

	backpause = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 17) }, 186, 31);
	backpause->AddImage("standard", { 25, 2695, 186, 31 });
	backpause->SetImage("standard");
	backpause->AddImage("clicked", { 25, 2768, 186, 31 });
	backpause->AddImage("hovered", { 26, 2732, 186, 31 });

	options = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) }, 186, 31);
	options->AddImage("standard", { 25, 2695, 186, 31 });
	options->SetImage("standard");
	options->AddImage("clicked", { 25, 2768, 186, 31 });
	options->AddImage("hovered", { 26, 2732, 186, 31 });

	quit_game = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 8) }, 186, 31);
	quit_game->AddImage("standard", { 25, 2695, 186, 31 });
	quit_game->SetImage("standard");
	quit_game->AddImage("clicked", { 25, 2768, 186, 31 });
	quit_game->AddImage("hovered", { 26, 2732, 186, 31 });

	mainmenu = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 10) + (App->win->_GetWindowSize().y / 97) }, 186, 31);
	mainmenu->AddImage("standard", { 25, 2695, 186, 31 });
	mainmenu->SetImage("standard");
	mainmenu->AddImage("clicked", { 25, 2768, 186, 31 });
	mainmenu->AddImage("hovered", { 26, 2732, 186, 31 });
	
	back= (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 18) }, 186, 31);
	back->AddImage("standard", { 25, 2695, 186, 31 });
	back->SetImage("standard");
	back->AddImage("clicked", { 25, 2768, 186, 31 });
	back->AddImage("hovered", { 26, 2732, 186, 31 });
	
	save = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) }, 186, 31);
	save->AddImage("standard", { 25, 2695, 186, 31 });
	save->SetImage("standard");
	save->AddImage("clicked", { 25, 2768, 186, 31 });
	save->AddImage("hovered", { 26, 2732, 186, 31 });
	
	load = (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) - (App->win->_GetWindowSize().y / 97) }, 186, 31);
	load->AddImage("standard", { 25, 2695, 186, 31 });
	load->SetImage("standard");
	load->AddImage("clicked", { 25, 2768, 186, 31 });
	load->AddImage("hovered", { 26, 2732, 186, 31 });

	create_group_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 12) - (App->win->_GetWindowSize().y / 200) }, App->font->default_15);
	create_group_txt->SetText("Create unit group -");
	create_group_txt->click_through = true;

	create_group_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) + (App->win->_GetWindowSize().x / 60), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 12) - (App->win->_GetWindowSize().y / 200) }, App->font->default_15);
	create_group_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[CREATE_GROUP]);
	create_group_txt2->SetText(create_group_txt2->str);
	create_group_txt2->click_through = true;

	move_map_right_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 9) }, App->font->default_15);
	move_map_right_txt->SetText("Move screen right -");
	move_map_right_txt->click_through = true;

	move_map_right_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) + (App->win->_GetWindowSize().x / 50), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 9) }, App->font->default_15);
	move_map_right_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_RIGHT]);
	move_map_right_txt2->SetText(move_map_right_txt2->str);
	move_map_right_txt2->click_through = true;

	move_map_left_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 7) + (App->win->_GetWindowSize().y / 95) }, App->font->default_15);
	move_map_left_txt->SetText("Move screen left -");
	move_map_left_txt->click_through = true;

	move_map_left_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) + (App->win->_GetWindowSize().x / 75), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 7) + (App->win->_GetWindowSize().y / 95) }, App->font->default_15);
	move_map_left_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_LEFT]);
	move_map_left_txt2->SetText(move_map_left_txt2->str);
	move_map_left_txt2->click_through = true;

	move_map_down_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 7) - (App->win->_GetWindowSize().y / 90) }, App->font->default_15);
	move_map_down_txt->SetText("Move screen down -");
	move_map_down_txt->click_through = true;

	move_map_down_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) + (App->win->_GetWindowSize().x / 40), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 7) - (App->win->_GetWindowSize().y / 90) }, App->font->default_15);
	move_map_down_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_DOWN]);
	move_map_down_txt2->SetText(move_map_down_txt2->str);
	move_map_down_txt2->click_through = true;

	move_map_up_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) - (App->win->_GetWindowSize().y / 90) }, App->font->default_15);
	move_map_up_txt->SetText("Move screen up -");
	move_map_up_txt->click_through = true;

	move_map_up_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) + (App->win->_GetWindowSize().x / 80), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) - (App->win->_GetWindowSize().y / 90) }, App->font->default_15);
	move_map_up_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_UP]);
	move_map_up_txt2->SetText(move_map_up_txt2->str);
	move_map_up_txt2->click_through = true;

	battlecry_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) }, App->font->default_15);
	battlecry_txt->SetText("Battlecry -");
	battlecry_txt->click_through = true;

	battlecry_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 55), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) }, App->font->default_15);
	battlecry_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[BATTLECRY]);
	battlecry_txt2->SetText(battlecry_txt2->str);
	battlecry_txt2->click_through = true;

	whirlwind_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) + (App->win->_GetWindowSize().y / 40) }, App->font->default_15);
	whirlwind_txt->SetText("Whirlwind -");
	whirlwind_txt->click_through = true;

	whirlwind_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 80), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) + (App->win->_GetWindowSize().y / 40) }, App->font->default_15);
	whirlwind_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[WHIRLWIND]);
	whirlwind_txt2->SetText(whirlwind_txt2->str);
	whirlwind_txt2->click_through = true;

	charge_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) }, App->font->default_15);
	charge_txt->SetText("Charge -");
	charge_txt->click_through = true;

	charge_txt2 = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 37), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) }, App->font->default_15);
	charge_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[CHARGE]);
	charge_txt2->SetText(charge_txt2->str);
	charge_txt2->click_through = true;

	backoptions_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 70), (App->win->_GetWindowSize().y / 2) + (App->win->_GetWindowSize().y / 18) }, App->font->default_15);
	backoptions_txt->SetText("Back");
	backoptions_txt->click_through = true;

	backpause_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 16) }, App->font->default);
	backpause_txt->SetText("Back");
	backpause_txt->click_through = true;

	controls_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) - (App->win->_GetWindowSize().y / 80) }, App->font->default);
	controls_txt->SetText("Controls");
	controls_txt->click_through = true;

	audio_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) - (App->win->_GetWindowSize().y / 180) }, App->font->default);
	audio_txt->SetText("Audio");
	audio_txt->click_through = true;

	audio_on_off = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) + (App->win->_GetWindowSize().x / 50), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) - (App->win->_GetWindowSize().y / 180) }, App->font->default);
	audio_on_off->SetText("On");
	audio_on_off->click_through = true;

	pause_menu_txt= (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x /40), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3) + (App->win->_GetWindowSize().y / 60) }, App->font->default,0,false,0,0,0);
	pause_menu_txt->SetText("PAUSE");
	
	quit_txt= (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 8) - (App->win->_GetWindowSize().y / 180) }, App->font->default);
	quit_txt->SetText("Quit");
	quit_txt->click_through = true;

	mainmenu_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 10)  +(App->win->_GetWindowSize().y / 120) }, App->font->default);
	mainmenu_txt->SetText("Main Menu");
	mainmenu_txt->click_through = true;
	
	back_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 20), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 17) }, App->font->default);
	back_txt->SetText("Resume Game");
	back_txt->click_through = true;
	
	save_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) - (App->win->_GetWindowSize().y / 180) }, App->font->default);
	save_txt->SetText("Save");
	save_txt->click_through = true;

	load_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) - (App->win->_GetWindowSize().y / 80) }, App->font->default);
	load_txt->SetText("Load");
	load_txt->click_through = true;

	options_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) - (App->win->_GetWindowSize().y / 180) }, App->font->default);
	options_txt->SetText("Options");
	options_txt->click_through = true;
	pause_window->SetEnabledAndChilds(false);
	//options 

	//stats

	attributes_window = (UI_Window*)App->gui->UI_CreateWin({ 0, 0 }, 0, 0, 10);
	life_txt = (UI_Text*)attributes_window->CreateText({ 149, 940 }, App->font->default_15);
	damage_txt = (UI_Text*)attributes_window->CreateText({ 149, 956 }, App->font->default_15);
	armor_txt = (UI_Text*)attributes_window->CreateText({ 149, 972 }, App->font->default_15);
	pierce_armor_txt = (UI_Text*)attributes_window->CreateText({ 149, 988 }, App->font->default_15);
	
	levelup_window = (UI_Window*)App->gui->UI_CreateWin({ 0, 0 }, 0, 0, 10);
	life_button = (UI_Button*)levelup_window->CreateButton({ 180, 945 }, 10, 10);
	life_button->AddImage("standard", { 792, 73, 10, 10 });
	life_button->SetImage("standard");
	damage_button = (UI_Button*)levelup_window->CreateButton({ 180, 961 }, 10, 10);
	damage_button->AddImage("standard", { 792, 73, 10, 10 });
	damage_button->SetImage("standard");
	armor_button = (UI_Button*)levelup_window->CreateButton({ 180, 977 }, 10, 10);
	armor_button->AddImage("standard", { 792, 73, 10, 10 });
	armor_button->SetImage("standard");
	pierce_armor_button = (UI_Button*)levelup_window->CreateButton({ 180, 993 }, 10, 10);
	pierce_armor_button->AddImage("standard", { 792, 73, 10, 10 });
	pierce_armor_button->SetImage("standard");
	level_points_txt = (UI_Text*)levelup_window->CreateText({ 150, 1017 }, App->font->default_10);
	level_gained_txt = (UI_Text*)levelup_window->CreateText({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x/10,  (App->win->_GetWindowSize().y) - (App->win->_GetWindowSize().y / 6) }, App->font->default_20, 0, false, 255, 255, 0);
	level_gained_txt->SetText("You have gained a level! Check your stats and upgrade your hero!");
	level_gained_txt->click_through = true;
	levelup_window->SetEnabledAndChilds(false);

	barracks_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(280, 200), 225, 144, 11);
	brokenbuilding_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(480, 200), 225, 144, 11);

	unit_scroll = (UI_Image*)barracks_ui_window->CreateImage(iPoint(0, 750), { 1326, 2190, 224, 125 });
	unit_scroll->click_through = true;

	//Buttons for barracks

	create_unit_button = (UI_Button*)barracks_ui_window->CreateButton(iPoint(25, 785), 60, 60);
	create_unit_button->AddImage("standard", { 705, 0, 60, 60 });
	create_unit_button->SetImage("standard");
	create_unit_button->AddImage("clicked", { 645, 0, 60, 60 });
	create_unit_button->change_click_through = true;

	create_unit_button2 = (UI_Button*)barracks_ui_window->CreateButton(iPoint(112, 785), 60, 60);
	create_unit_button2->AddImage("standard", { 705, 0, 60, 60 });
	create_unit_button2->SetImage("standard");
	create_unit_button2->AddImage("clicked", { 645, 0, 60, 60 });
	create_unit_button2->change_click_through = true;

	barbarian_img = (UI_Button*)barracks_ui_window->CreateButton(iPoint(37, 795), 37, 36);
	barbarian_img->AddImage("standard", { 765, 0, 37, 36 });
	barbarian_img->SetImage("standard");
	barbarian_img->click_through = true;

	swordsman_img = (UI_Button*)barracks_ui_window->CreateButton(iPoint(123, 795), 37, 36);
	swordsman_img->AddImage("standard", { 765, 36, 37, 36 });
	swordsman_img->SetImage("standard");
	swordsman_img->click_through = true;

	barracks_ui_window->SetEnabledAndChilds(false);

	//Buttons for brokenbuilding
	create_building_button = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(3, 785), 60, 60);
	create_building_button->AddImage("standard", { 705, 0, 60, 60 });
	create_building_button->SetImage("standard");
	create_building_button->AddImage("clicked", { 645, 0, 60, 60 });
	create_building_button->change_click_through = true;

	create_building_button2 = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(66, 785), 60, 60);
	create_building_button2->AddImage("standard", { 705, 0, 60, 60 });
	create_building_button2->SetImage("standard");
	create_building_button2->AddImage("clicked", { 645, 0, 60, 60 });
	create_building_button2->change_click_through = true;

	create_building_button3 = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(129, 785), 60, 60);
	create_building_button3->AddImage("standard", { 705, 0, 60, 60 });
	create_building_button3->SetImage("standard");
	create_building_button3->AddImage("clicked", { 645, 0, 60, 60 });
	create_building_button3->change_click_through = true;

	barrack_img = (UI_Button*)brokenbuilding_ui_window->CreateImage(iPoint(15, 795), { 808, 48, 39, 38 });
	barrack_img->click_through = true;

	house_img = (UI_Button*)brokenbuilding_ui_window->CreateImage(iPoint(76, 800), { 847, 52, 37, 33 });
	house_img->click_through = true;

	blacksmith_img = (UI_Button*)brokenbuilding_ui_window->CreateImage(iPoint(139, 795), { 852, 0, 45, 36 });
	blacksmith_img->click_through = true;

	brokenbuilding_ui_window->SetEnabledAndChilds(false);

	//player abilities

	player_abilities = (UI_Window*)App->gui->UI_CreateWin(iPoint(400, 200), 0, 0, 12);

	//Choose ability

	choose_ability_b = (UI_Button*)player_abilities->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 8) }, 186, 31);
	choose_ability_b->AddImage("standard", { 25, 2695, 186, 31 });
	choose_ability_b->SetImage("standard");
	choose_ability_b->AddImage("clicked", { 25, 2768, 186, 31 });
	choose_ability_b->AddImage("hovered", { 26, 2732, 186, 31 });
	choose_ability_b->change_click_through = true;

	choose_ability_uw = (UI_Button*)player_abilities->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) }, 186, 31);
	choose_ability_uw->AddImage("standard", { 25, 2695, 186, 31 });
	choose_ability_uw->SetImage("standard");
	choose_ability_uw->AddImage("clicked", { 25, 2768, 186, 31 });
	choose_ability_uw->AddImage("hovered", { 26, 2732, 186, 31 });
	choose_ability_uw->change_click_through = true;

	choose_ability_b_txt = (UI_Text*)player_abilities->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 20), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) }, App->font->default);
	choose_ability_b_txt->SetText("Berserker");
	choose_ability_b_txt->click_through = true;

	choose_ability_uw_txt = (UI_Text*)player_abilities->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 20), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 8) }, App->font->default);
	choose_ability_uw_txt->SetText("Tactical");
	choose_ability_uw_txt->click_through = true;

	//Battlecry && Undiying Will

	battlecry_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9), 25, 25);
	battlecry_ability->AddImage("standard", { 645, 60, 25, 25 });
	battlecry_ability->SetImage("standard");
	battlecry_ability->AddImage("clicked", { 670, 60, 25, 25 });

	battlecry_key = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 19, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_15);
	battlecry_key->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[BATTLECRY]);
	battlecry_key->SetText(battlecry_key->str);

	battlecry_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_15);
	battlecry_cd->SetEnabled(false);

	undying_will_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_15);
	undying_will_cd->SetEnabled(false);

	show_ability_name = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 14, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_10);

	//Whirlwind

	whirlwind_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12), 25, 25);
	whirlwind_ability->AddImage("standard", { 695, 60, 25, 25 });
	whirlwind_ability->SetImage("standard");
	whirlwind_ability->AddImage("clicked", { 720, 60, 25, 25 });

	whirlwind_key = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 19, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12 }, App->font->default_15);
	whirlwind_key->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[WHIRLWIND]);
	whirlwind_key->SetText(whirlwind_key->str);

	whirlwind_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12 }, App->font->default_15);
	whirlwind_cd->SetEnabled(false);

	//Charge

	charge_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18), 25, 25);
	charge_ability->AddImage("standard", { 802, 0, 25, 25 });
	charge_ability->SetImage("standard");
	charge_ability->AddImage("clicked", { 827, 0, 25, 25 });

	charge_key = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 19, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18 }, App->font->default_15);
	charge_key->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[CHARGE]);
	charge_key->SetText(charge_key->str);

	charge_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18 }, App->font->default_15);
	charge_cd->SetEnabled(false);

	//drop object interface

	inventory = (UI_Window*)App->gui->UI_CreateWin(iPoint(1300, 1000), 0, 0, 13);
	item_drop = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x - App->win->_GetWindowSize().x / 5, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 8), 60, 60);
	item_drop->AddImage("standard", { 898, 0, 40, 40 });
	item_drop->SetImage("standard");
	item_drop->SetEnabled(false);

	return ret;
}

bool Player::PreUpdate()
{
	bool ret = true;

	if (victory_status) {
		victory_window->SetEnabledAndChilds(true);
		lose->SetEnabled(false);
		pause_status = true;
		back->SetEnabled(false);
		save->SetEnabled(false);
		load->SetEnabled(false);
		save_txt->SetEnabled(false);
		load_txt->SetEnabled(false);
		pause_menu_txt->SetEnabled(false);
		back_txt->SetEnabled(false);
	}
	if (lose_status) {
		victory_window->SetEnabledAndChilds(true);
		victory->SetEnabled(false);
		pause_status = true;
		back->SetEnabled(false);
		save->SetEnabled(false);
		load->SetEnabled(false);
		save_txt->SetEnabled(false);
		load_txt->SetEnabled(false);
		pause_menu_txt->SetEnabled(false);
		back_txt->SetEnabled(false);
	}

	if (hero->life <= 0) {
		App->player->lose_status = true;
	}

	if (active_ability == not_chosen && pause_status == false) {
		pause_status = !pause_status;
		choose_ability_b->SetEnabled(true);
		choose_ability_b_txt->SetEnabled(true);
		choose_ability_uw->SetEnabled(true);
		choose_ability_uw_txt->SetEnabled(true);
		show_ability_name->SetText("");
	}

	if ((choose_ability_b->MouseClickEnterLeft() && active_ability == not_chosen) || (active_ability == battlecry_active && loaded))
	{
		active_ability = battlecry_active;

		show_ability_name->SetText("BC");

		choose_ability_b->SetEnabled(false);
		choose_ability_b_txt->SetEnabled(false);

		choose_ability_uw->SetEnabled(false);
		choose_ability_uw_txt->SetEnabled(false);

		pause_status = !pause_status;
		
		if (loaded)
			loaded = false;
	}
	else if ((choose_ability_uw->MouseClickEnterLeft() && active_ability == not_chosen) || (active_ability == undying_will_active && loaded))
	{
		active_ability = undying_will_active;

		show_ability_name->SetText("UW");

		choose_ability_b->SetEnabled(false);
		choose_ability_b_txt->SetEnabled(false);

		choose_ability_uw->SetEnabled(false);
		choose_ability_uw_txt->SetEnabled(false);

		pause_status = !pause_status;

		if (loaded)
			loaded = false;
	}

	if (choose_ability_b->MouseClickEnterLeft() && active_ability != not_chosen && active_ability != battlecry_active && App->scene->scene_test->gold >= 50)
	{
		active_ability = battlecry_active;
		App->scene->scene_test->gold = App->scene->scene_test->gold - 50;

		show_ability_name->SetText("BC");

		choose_ability_b->SetEnabled(false);
		choose_ability_b_txt->SetEnabled(false);

		choose_ability_uw->SetEnabled(false);
		choose_ability_uw_txt->SetEnabled(false);

		battlecry_ability->SetImage("clicked");

		battlecry_timer.Start();
		battlecry_cd->enabled = true;
		undying_will_cd->enabled = false;
	}
	else if (choose_ability_uw->MouseClickEnterLeft() && active_ability != not_chosen && active_ability != undying_will_active && App->scene->scene_test->gold >= 50)
	{
		active_ability = undying_will_active;
		App->scene->scene_test->gold = App->scene->scene_test->gold - 50;

		show_ability_name->SetText("UW");

		choose_ability_b->SetEnabled(false);
		choose_ability_b_txt->SetEnabled(false);

		choose_ability_uw->SetEnabled(false);
		choose_ability_uw_txt->SetEnabled(false);

		battlecry_ability->SetImage("clicked");

		undying_will_timer.Start();
		undying_will_cd->enabled = true;
		battlecry_cd->enabled = false;
	}
	if (choose_ability_b->MouseEnter())
		 {
		help_window->SetEnabledAndChilds(true);
		helping_txt->SetText("Undiying Will. Gives the player the state of invencibility (cannot be harmed) for 4 seconds (20s cd)");
		text_on = true;
		}
	else if (choose_ability_uw->MouseEnter()) {
		help_window->SetEnabledAndChilds(true);
		helping_txt->SetText("Battlecry. Gives nearby units a buff of +5 damage for 5 seconds (30s cd)");
		text_on = true;
		
	}
	else if (text_on && ((choose_ability_b->enabled == false || (choose_ability_b->MouseOut() && choose_ability_uw->MouseOut())) && battlecry_ability->MouseOut() && whirlwind_ability->MouseOut() && charge_ability->MouseOut() && pierce_armor_txt->MouseOut() && damage_txt->MouseOut() && armor_txt->MouseOut() && life_txt->MouseOut()) && (create_unit_button->enabled == false || (create_unit_button->MouseOut() && create_unit_button2->MouseOut())) && (create_building_button->enabled == false || (create_building_button->MouseOut() && create_building_button2->MouseOut() && create_building_button3->MouseOut())) && (item_drop->enabled == false || item_drop->MouseOut()))
		 {
		help_window->SetEnabledAndChilds(false);
		text_on = false;
		}

	if (text_on == false)
		 help_window->SetEnabledAndChilds(false);
	else
		 help_window->SetEnabledAndChilds(true);
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == key_down)
	{
		if (active_ability != not_chosen)
		{
			choose_ability_b->SetEnabled(false);
			choose_ability_b_txt->SetEnabled(false);

			choose_ability_uw->SetEnabled(false);
			choose_ability_uw_txt->SetEnabled(false);
		}
		if (change_controls_status)
		{
			change_controls_status = false;

			button_on_clicked = false;
			backoptions->SetImage("standard");

			//active options menu
			pause_bg->SetEnabled(true);
			audio_button->SetEnabled(true);
			audio_button->click_through = false;
			controls_button->SetEnabled(true);
			controls_button->click_through = false;
			backpause->SetEnabled(true);
			backpause->click_through = false;
			controls_txt->SetEnabled(true);
			backpause_txt->SetEnabled(true);
			audio_txt->SetEnabled(true);
			audio_on_off->SetEnabled(true);

			//quit controls menu
			controls_bg->SetEnabled(false);
			backoptions->SetEnabled(false);
			battlecry_button_options->SetEnabled(false);
			whirlwind_button_options->SetEnabled(false);
			charge_button_options->SetEnabled(false);
			move_map_up_button->SetEnabled(false);
			move_map_down_button->SetEnabled(false);
			move_map_left_button->SetEnabled(false);
			move_map_right_button->SetEnabled(false);
			create_group_button->SetEnabled(false);

			backoptions_txt->SetEnabled(false);
			battlecry_txt->SetEnabled(false);
			whirlwind_txt->SetEnabled(false);
			charge_txt->SetEnabled(false);
			move_map_up_txt->SetEnabled(false);
			move_map_down_txt->SetEnabled(false);
			move_map_left_txt->SetEnabled(false);
			move_map_right_txt->SetEnabled(false);
			create_group_txt->SetEnabled(false);
			battlecry_txt2->SetEnabled(false);
			whirlwind_txt2->SetEnabled(false);
			charge_txt2->SetEnabled(false);
			move_map_up_txt2->SetEnabled(false);
			move_map_down_txt2->SetEnabled(false);
			move_map_left_txt2->SetEnabled(false);
			move_map_right_txt2->SetEnabled(false);
			create_group_txt2->SetEnabled(false);

		}
		else if (audio_status)
		{

		}
		else if (options_status)
		{
			options_status = false;

			button_on_clicked = false;
			backpause->SetImage("standard");

			//active initial pause menu
			save->SetEnabled(true);
			save->click_through = false;
			load->SetEnabled(true);
			load->click_through = false;
			quit_game->SetEnabled(true);
			quit_game->click_through = false;
			back->SetEnabled(true);
			back->click_through = false;
			options->SetEnabled(true);
			options->click_through = false;
			save_txt->SetEnabled(true);
			load_txt->SetEnabled(true);
			options_txt->SetEnabled(true);
			quit_txt->SetEnabled(true);
			back_txt->SetEnabled(true);
			mainmenu->SetEnabled(true);
			mainmenu->click_through = false;
			mainmenu_txt->SetEnabled(true);
			mainmenu_txt->click_through = false;


			//quit options menu
			audio_button->SetEnabled(false);
			controls_button->SetEnabled(false);
			backpause->SetEnabled(false);

			controls_txt->SetEnabled(false);
			backpause_txt->SetEnabled(false);
			audio_txt->SetEnabled(false);
			audio_on_off->SetEnabled(false);
		}
		else
			pause_status = !pause_status;
	}

	if (audio_button->MouseClickEnterLeft())
	{
		audio_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;

		if (audio_muted)
		{
			audio_muted = !audio_muted;
			audio_on_off->SetText("On");
			App->audio->ResumeMusic();
		}
		else
		{
			audio_muted = !audio_muted;
			audio_on_off->SetText("Off");
			App->audio->PauseMusic();
		}

	}
	else if (controls_button->MouseClickEnterLeft())
	{
		change_controls_status = true;
		controls_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (backpause->MouseClickEnterLeft())
	{
		backpause->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (back->MouseClickEnterLeft())
	{
		back->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (load->MouseClickEnterLeft())
	{
		load->SetImage("clicked");
		App->entity->loaded = false;
		App->LoadGame("Save_File.xml");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (options->MouseClickEnterLeft() && pause_status)
	{
		options_status = true;
		options->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (save->MouseClickEnterLeft())
	{
		save->SetImage("clicked");
		App->SaveGame("Save_File.xml");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (mainmenu->MouseClickEnterLeft() && pause_status)
	{
		mainmenu->SetImage("clicked");
		App->entity->UnselectEverything();
		App->scene->scene_test->main_menu_window->SetEnabledAndChilds(true);
		lose_status = false;
		victory_status = false;
		victory_window->SetEnabledAndChilds(false);
		App->LoadGame("Restart_Game_File.xml");
		App->audio->RestartAudio();
		pause_status = false;

		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (backpause->MouseClickEnterLeft())
	{
		backpause->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (backoptions->MouseClickEnterLeft())
	{
		backoptions->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (battlecry_button_options->MouseClickEnterLeft())
	{
		battlecry_button_options->SetImage("clicked");
		change_button_config = true;

		whirlwind_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (whirlwind_button_options->MouseClickEnterLeft())
	{
		whirlwind_button_options->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (charge_button_options->MouseClickEnterLeft())
	{
		charge_button_options->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		whirlwind_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (move_map_up_button->MouseClickEnterLeft())
	{
		move_map_up_button->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		whirlwind_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (move_map_down_button->MouseClickEnterLeft())
	{
		move_map_down_button->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		whirlwind_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (move_map_left_button->MouseClickEnterLeft())
	{
		move_map_left_button->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		whirlwind_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (move_map_right_button->MouseClickEnterLeft())
	{
		move_map_right_button->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		whirlwind_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		create_group_button->SetImage("standard");
	}
	else if (create_group_button->MouseClickEnterLeft())
	{
		create_group_button->SetImage("clicked");
		change_button_config = true;

		battlecry_button_options->SetImage("standard");
		whirlwind_button_options->SetImage("standard");
		charge_button_options->SetImage("standard");
		move_map_up_button->SetImage("standard");
		move_map_down_button->SetImage("standard");
		move_map_left_button->SetImage("standard");
		move_map_right_button->SetImage("standard");
	}

	if (!pause_status && (pause_window->enabled) && active_ability != not_chosen)
	{
		options_status = false;
		pause_window->SetEnabledAndChilds(false);
	}

	if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && (save->CompareState("clicked") || load->CompareState("clicked")))
	{
		button_on_clicked = false;
		save->SetImage("standard");
		load->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && back->CompareState("clicked") && button_on_clicked == true)
	{
		pause_status = !pause_status;
		button_on_clicked = false;
		back->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && mainmenu->CompareState("clicked") && button_on_clicked == true)
	{
		pause_status = !pause_status;
		button_on_clicked = false;
		mainmenu->SetImage("standard");
	}

	else if (button_clicked.ReadSec() >= 0.1 && audio_button->CompareState("clicked") && button_on_clicked == true)
	{
		button_on_clicked = false;
		audio_button->SetImage("standard");
	}

	if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && (save->CompareState("clicked") || options->CompareState("clicked"))) {
		button_on_clicked = false;
		options->SetImage("standard");

		//quit initial pause menu
		save->SetEnabled(false);
		save->click_through = true;
		load->SetEnabled(false);
		load->click_through = true;
		quit_game->SetEnabled(false);
		quit_game->click_through = true;
		back->SetEnabled(false);
		back->click_through = true;
		options->SetEnabled(false);
		options->click_through = true;
		save_txt->SetEnabled(false);
		load_txt->SetEnabled(false);
		options_txt->SetEnabled(false);
		quit_txt->SetEnabled(false);
		back_txt->SetEnabled(false);
		mainmenu->SetEnabled(false);
		mainmenu->click_through = true;
		mainmenu_txt->SetEnabled(false);

		//active options button
		audio_button->SetEnabled(true);
		controls_button->SetEnabled(true);
		backpause->SetEnabled(true);

		controls_txt->SetEnabled(true);
		backpause_txt->SetEnabled(true);
		audio_txt->SetEnabled(true);
		audio_on_off->SetEnabled(true);
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && backpause->CompareState("clicked"))
	{
		options_status = false;

		button_on_clicked = false;
		backpause->SetImage("standard");

		//active initial pause menu
		save->SetEnabled(true);
		save->click_through = false;
		load->SetEnabled(true);
		load->click_through = false;
		mainmenu->SetEnabled(true);
		mainmenu->click_through = false;
		quit_game->SetEnabled(true);
		quit_game->click_through = false;
		back->SetEnabled(true);
		back->click_through = false;
		options->SetEnabled(true);
		options->click_through = false;
		save_txt->SetEnabled(true);
		load_txt->SetEnabled(true);
		options_txt->SetEnabled(true);
		quit_txt->SetEnabled(true);
		back_txt->SetEnabled(true);
		mainmenu_txt->SetEnabled(true);

		//quit options menu
		audio_button->SetEnabled(false);
		controls_button->SetEnabled(false);
		backpause->SetEnabled(false);

		controls_txt->SetEnabled(false);
		backpause_txt->SetEnabled(false);
		audio_txt->SetEnabled(false);
		audio_on_off->SetEnabled(false);
	}

	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && controls_button->CompareState("clicked"))
	{
		button_on_clicked = false;
		controls_button->SetImage("standard");

		//quit options menu
		pause_bg->SetEnabled(false);
		audio_button->SetEnabled(false);
		audio_button->click_through = true;
		controls_button->SetEnabled(false);
		controls_button->click_through = true;
		backpause->SetEnabled(false);
		backpause->click_through = true;

		controls_txt->SetEnabled(false);
		backpause_txt->SetEnabled(false);
		audio_txt->SetEnabled(false);
		audio_on_off->SetEnabled(false);

		//active controls menu
		controls_bg->SetEnabled(true);
		backoptions->SetEnabled(true);
		battlecry_button_options->SetEnabled(true);
		whirlwind_button_options->SetEnabled(true);
		charge_button_options->SetEnabled(true);
		move_map_up_button->SetEnabled(true);
		move_map_down_button->SetEnabled(true);
		move_map_left_button->SetEnabled(true);
		move_map_right_button->SetEnabled(true);
		create_group_button->SetEnabled(true);

		backoptions_txt->SetEnabled(true);
		battlecry_txt->SetEnabled(true);
		whirlwind_txt->SetEnabled(true);
		charge_txt->SetEnabled(true);
		move_map_up_txt->SetEnabled(true);
		move_map_down_txt->SetEnabled(true);
		move_map_left_txt->SetEnabled(true);
		move_map_right_txt->SetEnabled(true);
		create_group_txt->SetEnabled(true);
		battlecry_txt2->SetEnabled(true);
		whirlwind_txt2->SetEnabled(true);
		charge_txt2->SetEnabled(true);
		move_map_up_txt2->SetEnabled(true);
		move_map_down_txt2->SetEnabled(true);
		move_map_left_txt2->SetEnabled(true);
		move_map_right_txt2->SetEnabled(true);
		create_group_txt2->SetEnabled(true);
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && backoptions->CompareState("clicked"))
	{
		change_controls_status = false;

		button_on_clicked = false;
		backoptions->SetImage("standard");

		//active options menu
		pause_bg->SetEnabled(true);
		audio_button->SetEnabled(true);
		audio_button->click_through = false;
		controls_button->SetEnabled(true);
		controls_button->click_through = false;
		backpause->SetEnabled(true);
		backpause->click_through = false;
		controls_txt->SetEnabled(true);
		backpause_txt->SetEnabled(true);
		audio_txt->SetEnabled(true);
		audio_on_off->SetEnabled(true);

		//quit controls menu
		controls_bg->SetEnabled(false);
		backoptions->SetEnabled(false);
		battlecry_button_options->SetEnabled(false);
		whirlwind_button_options->SetEnabled(false);
		charge_button_options->SetEnabled(false);
		move_map_up_button->SetEnabled(false);
		move_map_down_button->SetEnabled(false);
		move_map_left_button->SetEnabled(false);
		move_map_right_button->SetEnabled(false);
		create_group_button->SetEnabled(false);

		backoptions_txt->SetEnabled(false);
		battlecry_txt->SetEnabled(false);
		whirlwind_txt->SetEnabled(false);
		charge_txt->SetEnabled(false);
		move_map_up_txt->SetEnabled(false);
		move_map_down_txt->SetEnabled(false);
		move_map_left_txt->SetEnabled(false);
		move_map_right_txt->SetEnabled(false);
		create_group_txt->SetEnabled(false);
		battlecry_txt2->SetEnabled(false);
		whirlwind_txt2->SetEnabled(false);
		charge_txt2->SetEnabled(false);
		move_map_up_txt2->SetEnabled(false);
		move_map_down_txt2->SetEnabled(false);
		move_map_left_txt2->SetEnabled(false);
		move_map_right_txt2->SetEnabled(false);
		create_group_txt2->SetEnabled(false);
	}



	if (App->input->GetKey(SDL_SCANCODE_F1) == key_down)
		App->debug_mode = !App->debug_mode;


	if (App->input->GetKey(SDL_SCANCODE_F2) == key_down && App->debug_mode)
		App->gui->debug = !App->gui->debug;
	else if (!App->debug_mode)
		App->gui->debug = false;

	if (App->input->GetKey(SDL_SCANCODE_F3) == key_down && App->debug_mode)
		App->collisions->debug = !App->collisions->debug;
	else if (!App->debug_mode)
		App->gui->debug = false;

	if (App->input->GetKey(SDL_SCANCODE_F10) == key_down)
		ShellExecute(NULL, "open", "https://github.com/AwayFromKeyboard/Project-2_The-Lost-Lands-of-Khanelyf/issues", NULL, NULL, SW_SHOWMAXIMIZED);

	//pause
	if (pause_status && !pause_window->enabled && active_ability != not_chosen) {
		pause_window->SetEnabledAndChilds(true);

		controls_bg->SetEnabled(false);
		audio_button->SetEnabled(false);
		controls_button->SetEnabled(false);
		backpause->SetEnabled(false);
		controls_bg->SetEnabled(false);
		backoptions->SetEnabled(false);
		battlecry_button_options->SetEnabled(false);
		whirlwind_button_options->SetEnabled(false);
		charge_button_options->SetEnabled(false);
		move_map_up_button->SetEnabled(false);
		move_map_down_button->SetEnabled(false);
		move_map_left_button->SetEnabled(false);
		move_map_right_button->SetEnabled(false);
		create_group_button->SetEnabled(false);

		controls_txt->SetEnabled(false);
		backpause_txt->SetEnabled(false);
		audio_txt->SetEnabled(false);
		audio_on_off->SetEnabled(false);
		backoptions->SetEnabled(false);
		backoptions_txt->SetEnabled(false);
		battlecry_txt->SetEnabled(false);
		whirlwind_txt->SetEnabled(false);
		charge_txt->SetEnabled(false);
		move_map_up_txt->SetEnabled(false);
		move_map_down_txt->SetEnabled(false);
		move_map_left_txt->SetEnabled(false);
		move_map_right_txt->SetEnabled(false);
		create_group_txt->SetEnabled(false);
		battlecry_txt2->SetEnabled(false);
		whirlwind_txt2->SetEnabled(false);
		charge_txt2->SetEnabled(false);
		move_map_up_txt2->SetEnabled(false);
		move_map_down_txt2->SetEnabled(false);
		move_map_left_txt2->SetEnabled(false);
		move_map_right_txt2->SetEnabled(false);
		create_group_txt2->SetEnabled(false);

	}

	else if ((!pause_status && pause_window->enabled && active_ability != not_chosen))
	{
		pause_window->SetEnabledAndChilds(false);
	}
	if (backpause->MouseEnter()) {
		backpause->SetImage("hovered");
		}
	else if (backpause->MouseOut() && backpause->CompareState("hovered")) {
		backpause->SetImage("standard");
		}
	if (controls_button->MouseEnter()) {
		controls_button->SetImage("hovered");
		}
	else if (controls_button->MouseOut() && controls_button->CompareState("hovered"))
		controls_button->SetImage("standard");

	if (audio_button->MouseEnter()) {
		audio_button->SetImage("hovered");
		}
	else if (audio_button->MouseOut() && audio_button->CompareState("hovered"))
		audio_button->SetImage("standard");

	if (backoptions->MouseEnter()) {
		backoptions->SetImage("hovered");
		}
	else if (backoptions->MouseOut() && backoptions->CompareState("hovered"))
		backoptions->SetImage("standard");

	if (battlecry_button_options->MouseEnter()) {
		battlecry_button_options->SetImage("hovered");
		}
	else if (battlecry_button_options->MouseOut() && battlecry_button_options->CompareState("hovered")){
		battlecry_button_options->SetImage("standard");
		}
	if (whirlwind_button_options->MouseEnter()) {
		whirlwind_button_options->SetImage("hovered");
		}
	else if (whirlwind_button_options->MouseOut() && whirlwind_button_options->CompareState("hovered"))
		whirlwind_button_options->SetImage("standard");
	
	if (charge_button_options->MouseEnter()) {
		charge_button_options->SetImage("hovered");
	}
	else if (charge_button_options->MouseOut() && charge_button_options->CompareState("hovered")){
		charge_button_options->SetImage("standard");
		}
	if (move_map_up_button->MouseEnter())
		move_map_up_button->SetImage("hovered");
	else if (move_map_up_button->MouseOut() && move_map_up_button->CompareState("hovered"))
		move_map_up_button->SetImage("standard");

	if (move_map_down_button->MouseEnter())
		move_map_down_button->SetImage("hovered");
	else if (move_map_down_button->MouseOut() && move_map_down_button->CompareState("hovered"))
		move_map_down_button->SetImage("standard");

	if (move_map_left_button->MouseEnter())
		move_map_left_button->SetImage("hovered");
	else if (move_map_left_button->MouseOut() && move_map_left_button->CompareState("hovered"))
		move_map_left_button->SetImage("standard");

	if (move_map_right_button->MouseEnter())
		move_map_right_button->SetImage("hovered");
	else if (move_map_right_button->MouseOut() && move_map_right_button->CompareState("hovered"))
		move_map_right_button->SetImage("standard");

	if (create_group_button->MouseEnter())
		create_group_button->SetImage("hovered");
	else if (create_group_button->MouseOut() && create_group_button->CompareState("hovered"))
		create_group_button->SetImage("standard");

	//backbutton
	if (back->MouseEnter()) {
		back->SetImage("hovered");
		
	}
	else if (back->MouseOut() && back->CompareState("hovered"))
		back->SetImage("standard");

	if (mainmenu->MouseEnter()) {
		mainmenu->SetImage("hovered");
		
	}
	else if (mainmenu->MouseOut() && mainmenu->CompareState("hovered"))
		mainmenu->SetImage("standard");

	//savebutton
	if (save->MouseEnter()) {
		save->SetImage("hovered");
		
	}
	else if (save->MouseOut() && save->CompareState("hovered"))
		save->SetImage("standard");
	
	//load button
	if (load->MouseEnter()) {
		load->SetImage("hovered");
		
	}
	else if (load->MouseOut() && load->CompareState("hovered"))
		load->SetImage("standard");
	
	// options button

	if (options->MouseEnter()) {
		options->SetImage("hovered");
		
	}
	else if (options->MouseOut() && options->CompareState("hovered"))
		options->SetImage("standard");

	//quitbutton
	if (quit_game->MouseEnter()) {
		quit_game->SetImage("hovered");
		
	}
	else if (quit_game->MouseOut())
		quit_game->SetImage("standard");

	if (quit_game->MouseClickEnterLeft()) {
		quit_game->SetImage("clicked");
		App->stop_exe = true;
	}


	if (!pause_status)
	{


		if (life_txt->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Hero's Life points");
			text_on = true;
		}
		
		else if (armor_txt->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Hero's Armor points");
			text_on = true;
		}
	
		else if (damage_txt->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Hero's Damage points");
			text_on = true;
		}
		
		else if (pierce_armor_txt->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Hero's Armor Penetration points");
			text_on = true;
		}
		else if (create_building_button->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Create a barrack to hire troops. Requirements: 90 Gold. (You can only have one building of this type)");
			text_on = true;
		}
		else if (create_building_button2->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Create a house to increase by 1 the maximum amount of units you can have. Requirements: 30 Gold (You need a barrack to build one)");
			text_on = true;
		}
		else if (create_building_button3->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Create a blacksmith to change your first ability for 50 gold. Requirements: 50 Gold");
			text_on = true;
		}
		else if (create_unit_button->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Hire a Barbarian. Requirements: 10 Gold");
			text_on = true;
		}
		else if (create_unit_button2->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Hire a Swordsman. Requirements: 30 Gold & complete mission 4");
			text_on = true;
		}
		else if (item_drop->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Click to drop the item");
			text_on = true;
		}

		else if (battlecry_ability->MouseEnter())
		{

			if (active_ability == battlecry_active) {
				help_window->SetEnabledAndChilds(true);
				helping_txt->SetText("Battlecry. Gives nearby units a buff of +5 damage for 5 seconds (30s cd)");
				text_on = true;
			}
			else if (active_ability == undying_will_active) {
				help_window->SetEnabledAndChilds(true);
				helping_txt->SetText("Undiying Will. Gives the player the state of invencibility (cannot be harmed) for 4 seconds (20s cd)");
				text_on = true;
			}
		}

		else if (whirlwind_ability->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Whirlwind. A spin that does 40 AoE damage (8s cd)");
			text_on = true;
		}

		else if (charge_ability->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Charge. Click on the ground below an enemy inside the range of action to charge at him and deal him 40 damage + your actual attack (12s cd)");
			text_on = true;
		}

		else if (choose_ability_uw->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Undiying Will. Gives the player the state of invencibility (cannot be harmed) for 4 seconds (20s cd). Requirements: 50 Gold");
			text_on = true;
		}

		else if (choose_ability_b->MouseEnter())
		{
			help_window->SetEnabledAndChilds(true);
			helping_txt->SetText("Battlecry. Gives nearby units a buff of +5 damage for 5 seconds (30s cd). Requirements: 50 Gold");
			text_on = true;
		}
		
		else if (text_on && ((choose_ability_b->enabled == false || (choose_ability_b->MouseOut() && choose_ability_uw->MouseOut())) && battlecry_ability->MouseOut() && whirlwind_ability->MouseOut() && charge_ability->MouseOut() && pierce_armor_txt->MouseOut() && damage_txt->MouseOut() && armor_txt->MouseOut() && life_txt->MouseOut()) && (create_unit_button->enabled == false || (create_unit_button->MouseOut() && create_unit_button2->MouseOut())) && (create_building_button->enabled == false || (create_building_button->MouseOut() && create_building_button2->MouseOut() && create_building_button3->MouseOut())) && (item_drop->enabled==false || item_drop->MouseOut()))
		{
			help_window->SetEnabledAndChilds(false);
			text_on = false;
		}

		if(text_on == false)
			help_window->SetEnabledAndChilds(false);
		else
			help_window->SetEnabledAndChilds(true);


		//Barracks create unit buttons
		if (create_unit_button->MouseClickEnterLeft() && create_barbarian == true)
		{
			create_unit_button->SetImage("clicked");

			if (App->scene->scene_test->gold >= 10 && App->scene->scene_test->current_human_resources <= App->scene->scene_test->human_resources_max - 1) {
				Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally, iPoint(barracks_position.x + 100, barracks_position.y + 100));
				App->scene->scene_test->gold -= barb->cost;
				App->scene->scene_test->current_human_resources += barb->human_cost;
			}
		}
		if (create_unit_button->MouseClickOutLeft())
		{
			create_unit_button->SetImage("standard");
		}

		if (create_unit_button2->MouseClickEnterLeft() && create_swordsman == true)
		{
			create_unit_button2->SetImage("clicked");

			if (App->scene->scene_test->gold >= 30 && App->scene->scene_test->current_human_resources <= App->scene->scene_test->human_resources_max - 2) {
				Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally, iPoint(barracks_position.x + 100, barracks_position.y + 100));
				App->scene->scene_test->gold -= sword->cost;
				App->scene->scene_test->current_human_resources += sword->human_cost;
			}
		}

		if (create_unit_button2->MouseClickOutLeft())
		{
			create_unit_button2->SetImage("standard");
		}

		//Brokenbuilding create building buttons
		
				
		if (create_building_button->MouseClickEnterLeft() && create_building_button->CompareState("standard") && (App->scene->scene_test->gold >= 90 || App->debug_mode))
		{
			create_building_button->SetImage("clicked");

			if (!App->debug_mode)
				App->scene->scene_test->gold -= 90;	//Barracks cost

			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
			{
				if ((*it)->GetSelected())
				{
					iPoint pos = (*it)->position;
					(*it)->state = entity_death;
					Barracks* barrack = (Barracks*)App->entity->CreateEntity(barracks, building,  pos);
					unit_scroll->SetEnabled(false);
					brokenbuilding_ui_window->SetEnabledAndChilds(false);
					App->scene->scene_test->create_barrack = false;
					if (App->questmanager->GetCurrentQuest()->type == quest_type::create && App->questmanager->GetCurrentQuest()->id == quest_id::quest_leader)
					{
						App->questmanager->GetCurrentQuest()->progress++;
					}
				}
			}
			
		}
		if (create_building_button->MouseClickOutLeft())
		{
			create_building_button->SetImage("standard");
		}

		if (create_building_button2->MouseClickEnterLeft() && create_building_button2->CompareState("standard") && (App->scene->scene_test->gold >= 30 || App->debug_mode) && App->scene->scene_test->create_barrack == false)
		{
			create_building_button2->SetImage("clicked");

			if (!App->debug_mode)
				App->scene->scene_test->gold -= 30;	//Basic Building cost

			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
			{
				if ((*it)->GetSelected())
				{
					iPoint pos = (*it)->position;
					(*it)->state = entity_death;
					BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, ally_building, pos, RandomGenerate(1, 3));
					unit_scroll->SetEnabled(false);
					brokenbuilding_ui_window->SetEnabledAndChilds(false);
				}
			}

		}

		if (create_building_button2->MouseClickOutLeft())
		{
			create_building_button2->SetImage("standard");
		}

		if (create_building_button3->MouseClickEnterLeft() && create_building_button3->CompareState("standard") && (App->scene->scene_test->gold >= 50 || App->debug_mode) && App->scene->scene_test->create_blacksmith == true && blacksmith_alive == false)
		{
			create_building_button3->SetImage("clicked");

			if (!App->debug_mode)
				App->scene->scene_test->gold -= 50;	//Blacksmith cost

			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
			{
				if ((*it)->GetSelected())
				{
					iPoint pos = (*it)->position;
					(*it)->state = entity_death;
					Blacksmith* blacksmith = (Blacksmith*)App->entity->CreateEntity(blacksmiths, building, iPoint(pos.x + 40, pos.y + 50));
					unit_scroll->SetEnabled(false);
					brokenbuilding_ui_window->SetEnabledAndChilds(false);
				}
			}
			blacksmith_alive = true;
		}

		if (create_building_button3->MouseClickOutLeft())
		{
			create_building_button3->SetImage("standard");
		}

		//player abilities
		if (!hero->is_holding_object)
		{
			if (draw_battlecry_range || (App->input->GetKey(App->input->controls[BATTLECRY]) == key_repeat || battlecry_ability->MouseEnter()) && battlecry_ability->CompareState("standard") && active_ability == battlecry_active)
			{
				draw_battlecry_range = true;
				CheckAbilityRange(BATTLECRY_RANGE);
			}
			else if (draw_whirlwind_range || ((App->input->GetKey(App->input->controls[WHIRLWIND]) == key_repeat || whirlwind_ability->MouseEnter()) && whirlwind_ability->CompareState("standard")))
			{
				draw_whirlwind_range = true;
				CheckAbilityRange(WHIRLWIND_RANGE);
			}
			else if (clicked_charge_1 && !clicked_charge_2 || draw_charge_range || (App->input->GetKey(App->input->controls[CHARGE]) == key_repeat || charge_ability->MouseEnter())&& charge_ability->CompareState("standard"))
			{
				draw_charge_range = true;
				CheckStraightAbilityRange(CHARGE_RANGE);
			}

			if ((App->input->GetKey(App->input->controls[BATTLECRY]) == key_up || battlecry_ability->MouseOut()) && draw_battlecry_range)
			{
				draw_battlecry_range = false;
				range_visited.clear();
			}
			if ((App->input->GetKey(App->input->controls[WHIRLWIND]) == key_up || whirlwind_ability->MouseOut()) && draw_whirlwind_range)
			{
				draw_whirlwind_range = false;
				range_visited.clear();
			}
			if (!clicked_charge_1 && (App->input->GetKey(App->input->controls[CHARGE]) == key_up || charge_ability->MouseOut()) && draw_charge_range)
			{
				draw_charge_range = false;
			}

			//Battlecry
			if (active_ability == battlecry_active)
			{
				if (((App->input->GetKey(App->input->controls[BATTLECRY]) == key_repeat && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) || battlecry_ability->MouseClickEnterLeft()) && battlecry_ability->CompareState("standard") && active_ability == battlecry_active)
				{
					battlecry_ability->SetImage("clicked");
					Battlecry();
					battlecry_cd->SetEnabled(true);
					battlecry_timer.Start();
					draw_battlecry_range = false;
					range_visited.clear();
					create_new_buff_particle = true;
				}
				if (battlecry_timer.ReadSec() >= COOLDOWN_BATTLECRY) {
					battlecry_cd->SetEnabled(false);
					battlecry_ability->SetImage("standard");
				}
				else if (battlecry_timer.ReadSec() >= DURATION_BATTLECRY) {
					StopBuff(-BATTLECRY_BUFF);
				}
			}
			//Undying Will
			if (active_ability == undying_will_active)
			{
				if ((App->input->GetKey(App->input->controls[BATTLECRY]) == key_down || battlecry_ability->MouseClickEnterLeft()) && battlecry_ability->CompareState("standard"))
				{
					undying_state_active = true;

					(UndWillBuff*)App->particle->CreateParticle(particle_type::undwillbuff, 0, { hero->position.x, hero->position.y });

					battlecry_ability->SetImage("clicked");

					undying_will_cd->SetEnabled(true);
					undying_will_timer.Start();
				}
				if (undying_will_timer.ReadSec() >= 5)
					undying_state_active = false;

				if (undying_will_timer.ReadSec() >= COOLDOWN_UNDYING_WILL) {
					undying_will_cd->SetEnabled(false);
					battlecry_ability->SetImage("standard");
				}
			}

			// Whirlwind

			if ((whirlwind_ability->MouseClickEnterLeft() || (App->input->GetKey(App->input->controls[WHIRLWIND]) == key_repeat && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down)) && whirlwind_ability->CompareState("standard")) {
				whirlwind_ability->SetImage("clicked");
				Whirlwind();
				whirlwind_cd->SetEnabled(true);
				whirlwind_timer.Start();
				draw_whirlwind_range = false;
				range_visited.clear();
			}
			if (whirlwind_timer.ReadSec() >= COOLDOWN_WHIRLWIND)
			{
				whirlwind_cd->SetEnabled(false);
				whirlwind_ability->SetImage("standard");
			}

			//Charge

			if (!clicked_charge_1 && charge_ability->MouseClickEnterLeft()) {
				clicked_charge_1 = true;
				App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_attack_r);
			}
			else if (clicked_charge_1 && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) {
				clicked_charge_2 = true;
			}
			if ((clicked_charge_2 || (App->input->GetKey(App->input->controls[CHARGE]) == key_repeat && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down)) && charge_ability->CompareState("standard"))
			{
				Charge();
			}

			if (charge_ability->CompareState("clicked"))
			{
				if (charge_timer.ReadSec() >= COOLDOWN_CHARGE) {
					charge_cd->SetEnabled(false);
					charge_ability->SetImage("standard");
					clicked_charge_1 = false;
					clicked_charge_2 = false;
				}
				else if (charge_timer.ReadSec() >= 3 && charge_damage_buff == true)
				{
					GetHero()->damage -= CHARGE_DAMAGE;
					charge_damage_buff = false;
				}
				else if (charge_timer.ReadSec() >= 1 && charge_speed_buff == true)
				{
					GetHero()->speed -= CHARGE_SPEED;
					charge_speed_buff = false;
				}
			}
		}

		//Object interface
		if (item_drop->MouseClickEnterLeft() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down)
		{
			GetHero()->DropObject();
		}
	}
	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;
	
	if (!App->minimap->IsMouseOver())
	{
		if (App->scene->scene_test->main_menu_window->enabled == false)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down && App->gui->GetMouseHover() == nullptr && App->input->GetKey(App->input->controls[BATTLECRY]) != key_repeat && App->input->GetKey(App->input->controls[WHIRLWIND]) != key_repeat && App->input->GetKey(App->input->controls[CHARGE]) != key_repeat)
			{
				iPoint mouse;
				App->input->GetMouseWorld(mouse.x, mouse.y);
				App->entity->UnselectEverything();
				for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
				{
					Collider* unit = (*it)->GetCollider();

					if (unit != nullptr)
					{
						if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h && ((*it)->GetType() == entity_type::player || (*it)->GetType() == entity_type::ally || (*it)->GetType() == entity_type::building))
						{
							(*it)->SetSelected(true);
						}
					}
					if ((*it)->GetSelected())
					{
						if ((*it)->GetType() == building && (*it)->name == barracks)
						{
							App->entity->UnselectEverything();
							(*it)->SetSelected(true);
							barracks_ui_window->SetEnabledAndChilds(true);
							break;
						}
						else if ((*it)->GetType() == building && (*it)->name == broken_building)
						{
							App->entity->UnselectEverything();
							(*it)->SetSelected(true);
							if (App->questmanager->GetCurrentQuest()->id != quest_id::quest_beggar)
							{
								unit_scroll->SetEnabled(true);
								brokenbuilding_ui_window->SetEnabledAndChilds(true);
								if (App->scene->scene_test->create_blacksmith == false)
								{
									create_building_button3->SetEnabled(false);
									blacksmith_img->SetEnabled(false);
								}
							}
							break;
						}
						else if ((*it)->GetType() == building && (*it)->name == blacksmiths)
						{
							App->entity->UnselectEverything();
							(*it)->SetSelected(true);

							choose_ability_b->SetEnabled(true);
							choose_ability_b_txt->SetEnabled(true);

							choose_ability_uw->SetEnabled(true);
							choose_ability_uw_txt->SetEnabled(true);

							break;
						}
						else
						{
							App->entity->selected.push_back((Unit*)*it);
							break;
						}
					}
				}
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_down)
			{
				iPoint mouse;
				App->input->GetMouseWorld(mouse.x, mouse.y);
				iPoint mouse_pathfinding;
				App->input->GetMousePosition(mouse_pathfinding.x, mouse_pathfinding.y);
				iPoint p = App->render->ScreenToWorld(mouse_pathfinding.x, mouse_pathfinding.y);
				p = App->map->WorldToMap(p.x, p.y);
				bool mouse_over_entity = false;

				for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
				{
					Collider* unit = (*it)->GetCollider();

					if (unit != nullptr)
					{
						if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h)
						{
							switch ((*it)->GetType())
							{
							case entity_type::ally:
							case entity_type::npc:
							case entity_type::player:
								MoveToTile(p);
								mouse_over_entity = true;
								break;
							case entity_type::enemy:
							case entity_type::enemy_boss:
								SetAttackingEnemy((Unit*)*it);
								mouse_over_entity = true;
								break;
							case entity_type::building:
							case entity_type::ally_building:
								mouse_over_entity = true;
								break;
							case entity_type::object:
								SetPickingObject((Object*)*it);
								mouse_over_entity = true;
								break;
							case entity_type::enemy_building:
								SetAttackingBuilding((Building*)*it);
								mouse_over_entity = true;
								break;
							default:
								break;
							}
						}
					}

					if (mouse_over_entity)
						continue;
				}

				if (!mouse_over_entity) {
					MoveToTile(p);
				}
			}
		}
		if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_K) == key_down)
		{

			iPoint mouse;
			App->input->GetMouseWorld(mouse.x, mouse.y);

			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
			{
				Collider* unit = (*it)->GetCollider();

				if (unit != nullptr)
				{
					if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h)
						if ((*it)->life > 0) (*it)->KillEntity();
				}
			}
		}

		if (change_button_config)
		{
			SDL_Scancode code = App->input->ReturnKey();
			if (code != SDL_SCANCODE_0)
			{
				bool key_repeated = false;
				for (int i = 0; i < CONTROLS_SIZE; i++)
				{
					if (App->input->controls[i] == code)
					{
						key_repeated = true;
						continue;
					}
				}

				if (battlecry_button_options->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[BATTLECRY] = code;
					battlecry_button_options->SetImage("standard");
					change_button_config = false;

					battlecry_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[BATTLECRY]);
					battlecry_txt2->SetText(battlecry_txt2->str);

					battlecry_key->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[BATTLECRY]);
					battlecry_key->SetText(battlecry_key->str);
				}
				else if (whirlwind_button_options->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[WHIRLWIND] = code;
					whirlwind_button_options->SetImage("standard");
					change_button_config = false;

					whirlwind_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[WHIRLWIND]);
					whirlwind_txt2->SetText(whirlwind_txt2->str);

					whirlwind_key->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[WHIRLWIND]);
					whirlwind_key->SetText(whirlwind_key->str);
				}
				else if (charge_button_options->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[CHARGE] = code;
					charge_button_options->SetImage("standard");
					change_button_config = false;

					charge_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[CHARGE]);
					charge_txt2->SetText(charge_txt2->str);

					charge_key->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[CHARGE]);
					charge_key->SetText(charge_key->str);
				}
				else if (move_map_up_button->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[MOVE_MAP_UP] = code;
					move_map_up_button->SetImage("standard");
					change_button_config = false;

					move_map_up_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_UP]);
					move_map_up_txt2->SetText(move_map_up_txt2->str);
				}
				else if (move_map_down_button->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[MOVE_MAP_DOWN] = code;
					move_map_down_button->SetImage("standard");
					change_button_config = false;

					move_map_down_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_DOWN]);
					move_map_down_txt2->SetText(move_map_down_txt2->str);
				}
				else if (move_map_left_button->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[MOVE_MAP_LEFT] = code;
					move_map_left_button->SetImage("standard");
					change_button_config = false;

					move_map_left_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_LEFT]);
					move_map_left_txt2->SetText(move_map_left_txt2->str);
				}
				else if (move_map_right_button->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[MOVE_MAP_RIGHT] = code;
					move_map_right_button->SetImage("standard");
					change_button_config = false;

					move_map_right_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[MOVE_MAP_RIGHT]);
					move_map_right_txt2->SetText(move_map_right_txt2->str);
				}
				else if (create_group_button->CompareState("clicked") && !key_repeated)
				{
					App->input->controls[CREATE_GROUP] = code;
					create_group_button->SetImage("standard");
					change_button_config = false;

					create_group_txt2->str = App->input->GetKeyString((SDL_Scancode)App->input->controls[CREATE_GROUP]);
					create_group_txt2->SetText(create_group_txt2->str);
				}
			}
		}
	}
	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;

	CheckMouseEntity();

	if (hero != nullptr)
		UpdateAttributes();
	
	if (draw_buff == true)
	{
		if (create_new_buff_particle == true) {
			DrawBuff();
			create_new_buff_particle = false;
		}
	}

	if (battlecry_timer.ReadSec() <= COOLDOWN_BATTLECRY && active_ability == battlecry_active)
	{
		DrawCD(1); // 1 == Battlecry
	}

	if (undying_will_timer.ReadSec() <= COOLDOWN_UNDYING_WILL && active_ability == undying_will_active)
	{
		DrawCD(4); // 4 == Undying Will
	}

	if (whirlwind_timer.ReadSec() <= COOLDOWN_WHIRLWIND)
	{
		DrawCD(2); // 2 == Whirlwind
	}

	if (charge_timer.ReadSec() <= COOLDOWN_WHIRLWIND)
	{
		DrawCD(3); // 3 == Charge
	}

	return ret;
}

bool Player::CleanUp()
{
	bool ret = true;
	return ret;
}

bool Player::Load(pugi::xml_node& data)
{
	pugi::xml_node pos = data.child("Hero").child("Position");
	pugi::xml_node stats = data.child("Hero").child("Stats");
	pugi::xml_node misc = data.child("Hero").child("Misc");
	pugi::xml_node allies = data.child("Allies");

	hero = (Hero*)App->entity->CreateEntity(entity_name::hero, entity_type::player, { 0, 0 });

	hero->position.create(pos.attribute("x").as_int(), pos.attribute("y").as_int());

	hero->life = stats.attribute("current_hp").as_int();
	hero->max_life = stats.attribute("max_hp").as_int();
	hero->damage = stats.attribute("damage").as_int();
	hero->armor = stats.attribute("armor").as_int();
	hero->pierce_armor = stats.attribute("pierce_armor").as_int();
	
	hero->levelup_points = misc.attribute("level_up_Points").as_int();
	App->scene->scene_test->gold = misc.attribute("gold").as_int();
	App->scene->scene_test->current_human_resources = misc.attribute("human_resources").as_int();
	App->scene->scene_test->human_resources_max = misc.attribute("max_human_resources").as_int();

	for (pugi::xml_node ally = allies.child("Ally"); ally != NULL; ally = ally.next_sibling()) {
		int _name = ally.attribute("name").as_int();
		entity_name name;

		switch (_name)
		{
		case barbarian:
			name = barbarian;
			break;
		case swordsman:
			name = swordsman;
			break;
		}

		if (ally.child("Life").attribute("current").as_int() > 0)
		{
			Entity* entity = App->entity->CreateEntity(name, entity_type::ally, { 0, 0 });
			entity->position.create(ally.child("Position").attribute("x").as_int(), ally.child("Position").attribute("y").as_int());
			entity->life = ally.child("Life").attribute("current").as_int();
			entity->max_life = ally.child("Life").attribute("max").as_int();
		}
	}

	int ability = data.child("First_Ability").attribute("ability").as_int();

	switch (ability)
	{
	case battlecry_active:
		active_ability = battlecry_active;
		break;
	case undying_will_active:
		active_ability = undying_will_active;
		break;
	case not_chosen:
		active_ability = not_chosen;
		break;
	default:
		break;
	}

	return true;
}

bool Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node _hero = data.append_child("Hero");
	pugi::xml_node pos = _hero.append_child("Position");
	pugi::xml_node stats = _hero.append_child("Stats");
	pugi::xml_node misc = _hero.append_child("Misc");
	pugi::xml_node allies = data.append_child("Allies");

	pos.append_attribute("x") = hero->position.x;
	pos.append_attribute("y") = hero->position.y;

	stats.append_attribute("current_hp") = hero->life;
	stats.append_attribute("max_hp") = hero->max_life;
	stats.append_attribute("damage") = hero->damage;
	stats.append_attribute("armor") = hero->armor;
	stats.append_attribute("pierce_armor") = hero->pierce_armor;

	misc.append_attribute("level_up_Points") = hero->levelup_points;
	misc.append_attribute("gold") = App->scene->scene_test->gold;
	misc.append_attribute("human_resources") = App->scene->scene_test->current_human_resources;
	misc.append_attribute("max_human_resources") = App->scene->scene_test->human_resources_max;

	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
	{
		if ((*it)->type == entity_type::ally)
		{
			pugi::xml_node ally = allies.append_child("Ally");
			ally.append_attribute("name") = (*it)->name;
	
			ally.append_child("Life").append_attribute("current") = (*it)->life;
			ally.child("Life").append_attribute("max") = (*it)->max_life;

			ally.append_child("Position").append_attribute("x") = (*it)->position.x;
			ally.child("Position").append_attribute("y") = (*it)->position.y;
		}
	}

	data.append_child("First_Ability").append_attribute("ability") = active_ability;

	return true;
}

void Player::MoveToTile(iPoint tile)
{
	for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
		(*it)->path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint((*it)->position), tile);
		(*it)->state = entity_state::entity_move;
		(*it)->attacked_unit = nullptr;
		(*it)->attacked_building = nullptr;
	}

	if (App->entity->selected.size())
		App->particle->CreateParticle(particle_type::cursor, 0, App->map->MapToWorldPoint(tile));
}

void Player::SetAttackingEnemy(Unit* enemy)
{
	if (enemy->life > 0)
	{
		for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++)
		{
			(*it)->SetAttackingUnit(enemy);
			(*it)->state = entity_state::entity_move_to_enemy;
		}
	}
}

void Player::SetAttackingBuilding(Building* building)
{
	if (building->life > 0)
	{
		for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++)
		{
			(*it)->SetAttackingBuilding(building);
			(*it)->state = entity_state::entity_move_to_building;
		}
	}
}

void Player::UpdateAttributes()
{
	std::stringstream life;
	life << hero->life;
	life_txt->SetText(life.str());
	
	std::stringstream damage;
	damage << hero->damage;
	damage_txt->SetText(damage.str());
	
	std::stringstream armor;
	armor << hero->armor;
	armor_txt->SetText(armor.str());
	
	std::stringstream pierce_armor;
	pierce_armor << hero->pierce_armor;
	pierce_armor_txt->SetText(pierce_armor.str());

	if (hero->levelup_points > 0 && !levelup_window->enabled)
	{
		levelup_window->SetEnabledAndChilds(true);
	}
	else if (levelup_window->enabled)
	{
		if (hero->levelup_points == 0)
		{
			levelup_window->SetEnabledAndChilds(false);
		}
		else {
			std::stringstream levelup_points;
			levelup_points << "points left: " << hero->levelup_points;
			level_points_txt->SetText(levelup_points.str());

			if (life_button->MouseClickEnterLeft())
			{
				hero->levelup_points -= 1;
				hero->life += 20;
				hero->max_life += 20;
			}
			else if (damage_button->MouseClickEnterLeft())
			{
				hero->levelup_points -= 1;
				hero->damage += 1;
			}
			else if (armor_button->MouseClickEnterLeft())
			{
				hero->levelup_points -= 1;
				hero->armor += 1;
			}
			else if (pierce_armor_button->MouseClickEnterLeft())
			{
				hero->levelup_points -= 1;
				hero->pierce_armor += 1;
			}
		}
	}
}

void Player::SetPickingObject(Object* object)
{
	if (hero->GetSelected() && hero->is_holding_object == false)
	{
		hero->SetPickObject(object);
		hero->state = entity_state::entity_pick_object;
	}
}

void Player::SetHero(Hero * hero)
{
	this->hero = hero;
}

Hero* Player::GetHero()
{
	return hero;
}

void Player::Battlecry()
{
	buffed_list.clear();
	std::list<iPoint> frontier;
	std::list<iPoint> visited;

	visited.push_back(App->map->WorldToMapPoint(GetHero()->position));
	frontier.push_back(App->map->WorldToMapPoint(GetHero()->position));

	for (int i = 0; i < BATTLECRY_RANGE; ++i)
	{
		for (int j = frontier.size(); j > 0; j--)
		{
			iPoint neighbors[4];
			neighbors[0] = frontier.front() + iPoint(1, 0);
			neighbors[1] = frontier.front() + iPoint(-1, 0);
			neighbors[2] = frontier.front() + iPoint(0, 1);
			neighbors[3] = frontier.front() + iPoint(0, -1);
			frontier.pop_front();

			for (int k = 0; k < 4; k++)
			{
				if (neighbors[k].x >= 0 && neighbors[k].y >= 0)
				{
					Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
					if (found != nullptr && found->life > 0 && found->type == ally && !found->buffed)
					{
						buffed_list.push_back(found);
						found->buffed = true;
					}
					else {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it)
						{
							if (neighbors[k] == *it)
							{
								is_visited = true;
								break;
							}
						}
						if (!is_visited)
						{
							frontier.push_back(neighbors[k]);
							visited.push_back(neighbors[k]);
						}
					}
				}
			}
		}
	}

	BattlecryModifier(BATTLECRY_BUFF);
	draw_buff = true;
}

void Player::BattlecryModifier(int damage_buff)
{
	for (std::list<Unit*>::iterator it = buffed_list.begin(); it != buffed_list.end(); it++) 
	{
		(*it)->damage += damage_buff;
	}
}

void Player::Whirlwind()
{
	std::list<iPoint> frontier;
	std::list<iPoint> visited;

	visited.push_back(App->map->WorldToMapPoint(GetHero()->position));
	frontier.push_back(App->map->WorldToMapPoint(GetHero()->position));

	for (int i = 0; i < WHIRLWIND_RANGE; ++i)
	{
		for (int j = frontier.size(); j > 0; j--)
		{
			iPoint neighbors[4];
			neighbors[0] = frontier.front() + iPoint(1, 0);
			neighbors[1] = frontier.front() + iPoint(-1, 0);
			neighbors[2] = frontier.front() + iPoint(0, 1);
			neighbors[3] = frontier.front() + iPoint(0, -1);
			frontier.pop_front();

			for (int k = 0; k < 4; k++)
			{
				if (neighbors[k].x >= 0 && neighbors[k].y >= 0)
				{
					Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
					if (found != nullptr && found->life > 0 && found->type == enemy && found->damaged_by_whirlwind == false)
					{
						found->life -= WHIRLWIND_DAMAGE;
						if (found->life <= 0)
							found->state = entity_death;
						found->damaged_by_whirlwind = true;
					}
					else {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it)
						{
							if (neighbors[k] == *it)
							{
								is_visited = true;
								break;
							}
						}
						if (!is_visited)
						{
							frontier.push_back(neighbors[k]);
							visited.push_back(neighbors[k]);
						}
					}
				}
			}
		}
	}
}

void Player::Charge()
{
	std::list<iPoint> visited;
	iPoint mouse;
	//Collider* col;  //Using this collider and the lines commented below the ability will trigger if the player clicks on the collision of the enemy instead of the tile
	App->input->GetMouseWorld(mouse.x, mouse.y);

	visited.push_back(App->map->WorldToMapPoint(GetHero()->position));

	for (int i = 0; i < CHARGE_RANGE; ++i)
	{
		iPoint neighbors[8];
		neighbors[0] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 0);
		neighbors[1] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 0);
		neighbors[2] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, 1 + i);
		neighbors[3] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, -1 - i);
		neighbors[4] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 1 + i);
		neighbors[5] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, -1 - i);
		neighbors[6] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 1 + i);
		neighbors[7] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, -1 - i);

		for (int k = 0; k < 8; k++)
		{
			if (neighbors[k].x >= 0 && neighbors[k].y >= 0)
			{
				Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];

				if (found != nullptr && found->life > 0 && (found->type == enemy || found->type == enemy_boss) && App->map->WorldToMapPoint(mouse) == found->position_map)
				{
					GetHero()->speed += CHARGE_SPEED;
					GetHero()->damage += CHARGE_DAMAGE;
					charge_speed_buff = true;
					charge_damage_buff = true;

					switch (k)
					{
					case 0:
						MoveToTile(found->position_map + iPoint(-1, 0));
						break;
					case 1:
						MoveToTile(found->position_map + iPoint(1, 0));
						break;
					case 2:
						MoveToTile(found->position_map + iPoint(0, -1));
						break;
					case 3:
						MoveToTile(found->position_map + iPoint(0, 1));
						break;
					case 4:
						MoveToTile(found->position_map + iPoint(-1, -1));
						break;
					case 5:
						MoveToTile(found->position_map + iPoint(-1, 1));
						break;
					case 6:
						MoveToTile(found->position_map + iPoint(1, -1));
						break;
					case 7:
						MoveToTile(found->position_map + iPoint(1, 1));
						break;
					}

					charge_ability->SetImage("clicked");
					charge_cd->SetEnabled(true);
					charge_timer.Start();
					App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_r);
					draw_charge_range = false;
				}
				else
				{
					bool is_visited = false;
					for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it)
					{
						if (neighbors[k] == *it)
						{
							is_visited = true;
							break;
						}
					}
					if (!is_visited)
					{
						visited.push_back(neighbors[k]);
					}
				}
			}
		}
	}

	if (charge_ability->CompareState("standard")) {
		App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_r);
		clicked_charge_1 = false;
		clicked_charge_2 = false;
	}

}

void Player::CheckAbilityRange(int range)
{
	if (draw_battlecry_range == true || draw_whirlwind_range == true)
	{
		std::list<iPoint> frontier;

		if (GetHero()->position_map != range_visited.front())
		{
			range_visited.clear();
			range_visited.push_back(GetHero()->position_map);
			frontier.push_back(GetHero()->position_map);
			for (int i = 0; i < range; ++i) {
				for (int j = frontier.size(); j > 0; j--) {
					iPoint neighbors[4];
					neighbors[0] = frontier.front() + iPoint(1, 0);
					neighbors[1] = frontier.front() + iPoint(-1, 0);
					neighbors[2] = frontier.front() + iPoint(0, 1);
					neighbors[3] = frontier.front() + iPoint(0, -1);
					frontier.pop_front();

					for (int k = 0; k < 4; k++)
					{
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = range_visited.begin(); it != range_visited.end(); ++it)
						{
							if (neighbors[k] == *it)
							{
								is_visited = true;
								break;
							}
						}
						if (!is_visited)
						{
							frontier.push_back(neighbors[k]);
							range_visited.push_back(neighbors[k]);
						}
					}
				}
			}
		}
		for (std::list<iPoint>::iterator it = range_visited.begin(); it != range_visited.end(); it++)
		{
			App->scene->LayerBlit(200, App->scene->scene_test->debug_tex, App->map->MapToWorldPoint(*it), { 0, 0, 64, 64 });
		}
	}
}

void Player::CheckStraightAbilityRange(int range)
{
	if (draw_battlecry_range == true || draw_charge_range == true)
	{
		std::list<iPoint> visited;

		visited.push_back(App->map->WorldToMapPoint(GetHero()->position));

		for (int i = 0; i < range; ++i)
		{
			iPoint neighbors[8];
			neighbors[0] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 0);
			neighbors[1] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 0);
			neighbors[2] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, 1 + i);
			neighbors[3] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, -1 - i);
			neighbors[4] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 1 + i);
			neighbors[5] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, -1 - i);
			neighbors[6] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 1 + i);
			neighbors[7] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, -1 - i);

			for (int k = 0; k < 8; k++)
			{
				bool is_visited = false;
				for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it)
				{
					if (neighbors[k] == *it)
					{
						is_visited = true;
						break;
					}
				}
				if (!is_visited) {
					visited.push_back(neighbors[k]);
				}
			}
		}
		for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); it++)
		{
			App->scene->LayerBlit(200, App->scene->scene_test->debug_tex, App->map->MapToWorldPoint(*it), { 0, 0, 64, 64 });
		}
	}
}

void Player::DrawBuff()
{
	if (buffed_list.empty() != true) {
		for (std::list<Unit*>::iterator it = buffed_list.begin(); it != buffed_list.end(); it++) {
			(*it)->buff_particle = (BattlecryBuff*)App->particle->CreateParticle(particle_type::battlecrybuff, 0, { (*it)->position.x, (*it)->position.y - 15});
		}
	}
}

void Player::StopBuff(int modifier)
{
	draw_buff = false;
	BattlecryModifier(modifier);

	for (std::list<Unit*>::iterator it = buffed_list.begin(); it != buffed_list.end(); it++)
	{
		(*it)->buffed = false;
	}
	buffed_list.clear();
}

void Player::DrawCD(int ability_number)
{
	std::stringstream oss;

	if (ability_number == 1)
	{
		int timer = COOLDOWN_BATTLECRY - battlecry_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		battlecry_cd->SetText(txt);
	}

	if (ability_number == 2)
	{
		int timer = COOLDOWN_WHIRLWIND - whirlwind_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		whirlwind_cd->SetText(txt);
	}

	if (ability_number == 3)
	{
		int timer = COOLDOWN_CHARGE - charge_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		charge_cd->SetText(txt);
	}

	if (ability_number == 4)
	{
		int timer = COOLDOWN_UNDYING_WILL - undying_will_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		undying_will_cd->SetText(txt);
	}
}

void Player::CheckMouseEntity()
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);
	iPoint mouse_pathfinding;
	App->input->GetMousePosition(mouse_pathfinding.x, mouse_pathfinding.y);
	iPoint p = App->render->ScreenToWorld(mouse_pathfinding.x, mouse_pathfinding.y);
	p = App->map->WorldToMap(p.x, p.y);
	bool mouse_over_entity = false;

	if (!pause_window->enabled)
	{
		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
		{
			Collider* unit = (*it)->GetCollider();

			if (unit != nullptr)
			{
				if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h)
				{
					switch ((*it)->GetType())
					{
					case entity_type::ally:
					case entity_type::npc:
					case entity_type::player:
						mouse_over_entity = true;
						break;
					case entity_type::building:
					case entity_type::ally_building:
						App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_build_r);
						mouse_over_entity = true;
						break;
					case entity_type::object:
						App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_object_r);
						mouse_over_entity = true;
						break;
					case entity_type::enemy:
					case entity_type::enemy_building:
					case entity_type::enemy_boss:
						App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_attack_r);
						mouse_over_entity = true;
						break;
					default:
						break;
					}
				}
			}

			if (mouse_over_entity)
				continue;
		}

		if (!mouse_over_entity)
			App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_r);
	}
	else 
		App->scene->scene_test->SetCurrentCursor(App->scene->scene_test->cursor_ui_r);

}
