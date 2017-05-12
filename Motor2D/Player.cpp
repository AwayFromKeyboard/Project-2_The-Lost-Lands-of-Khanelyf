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
#include "BasicBuilding.h"
#include "Functions.h"
#include "QuestManager.h"

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
	
	back= (UI_Button*)pause_window->CreateButton({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 17), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 17) }, 186, 31);
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
	create_group_txt->SetText("Create unit group - Left Shift");
	create_group_txt->click_through = true;

	move_map_right_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 9) }, App->font->default_15);
	move_map_right_txt->SetText("Move screen right - Right Arr.");
	move_map_right_txt->click_through = true;

	move_map_left_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 7) + (App->win->_GetWindowSize().y / 95) }, App->font->default_15);
	move_map_left_txt->SetText("Move screen left - Left Arrow");
	move_map_left_txt->click_through = true;

	move_map_down_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 7) - (App->win->_GetWindowSize().y / 90) }, App->font->default_15);
	move_map_down_txt->SetText("Move screen down - Down A.");
	move_map_down_txt->click_through = true;

	move_map_up_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 6) - (App->win->_GetWindowSize().y / 90) }, App->font->default_15);
	move_map_up_txt->SetText("Move screen up - Up Arrow");
	move_map_up_txt->click_through = true;

	battlecry_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) }, App->font->default_15);
	battlecry_txt->SetText("Battlecry ---------------------- X");
	battlecry_txt->click_through = true;

	whirlwind_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 4) + (App->win->_GetWindowSize().y / 40) }, App->font->default_15);
	whirlwind_txt->SetText("Whirlwind -------------------- C");
	whirlwind_txt->click_through = true;

	charge_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 16), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 5) }, App->font->default_15);
	charge_txt->SetText("Charge ------------------------ V");
	charge_txt->click_through = true;

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

	pause_menu_txt= (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x /40), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 3) + (App->win->_GetWindowSize().y / 60) }, App->font->default,0,false,0,0,0);
	pause_menu_txt->SetText("PAUSE");
	
	quit_txt= (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 22), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 8) - (App->win->_GetWindowSize().y / 180) }, App->font->default);
	quit_txt->SetText("Quit");
	quit_txt->click_through = true;
	
	back_txt = (UI_Text*)pause_window->CreateText({ (App->win->_GetWindowSize().x / 2) - (App->win->_GetWindowSize().x / 20), (App->win->_GetWindowSize().y / 2) - (App->win->_GetWindowSize().y / 16) }, App->font->default);
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
	life_button = (UI_Button*)levelup_window->CreateButton({ 169, 937 }, 10, 10);
	life_button->AddImage("standard", { 792, 73, 10, 10 });
	life_button->SetImage("standard");
	damage_button = (UI_Button*)levelup_window->CreateButton({ 169, 953 }, 10, 10);
	damage_button->AddImage("standard", { 792, 73, 10, 10 });
	damage_button->SetImage("standard");
	armor_button = (UI_Button*)levelup_window->CreateButton({ 169, 969 }, 10, 10);
	armor_button->AddImage("standard", { 792, 73, 10, 10 });
	armor_button->SetImage("standard");
	pierce_armor_button = (UI_Button*)levelup_window->CreateButton({ 169, 985 }, 10, 10);
	pierce_armor_button->AddImage("standard", { 792, 73, 10, 10 });
	pierce_armor_button->SetImage("standard");
	level_points_txt = (UI_Text*)levelup_window->CreateText({ 150, 1017 }, App->font->default_10);
	levelup_window->SetEnabledAndChilds(false);

	barracks_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(280, 200), 225, 144, 11);
	brokenbuilding_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(480, 200), 225, 144, 11);

	//Buttons for barracks
	create_unit_button = (UI_Button*)barracks_ui_window->CreateButton(iPoint(285, 500), 60, 60);
	create_unit_button->AddImage("standard", { 705, 0, 60, 60 });
	create_unit_button->SetImage("standard");
	create_unit_button->AddImage("clicked", { 645, 0, 60, 60 });

	create_unit_button2 = (UI_Button*)barracks_ui_window->CreateButton(iPoint(384, 500), 60, 60);
	create_unit_button2->AddImage("standard", { 705, 0, 60, 60 });
	create_unit_button2->SetImage("standard");
	create_unit_button2->AddImage("clicked", { 645, 0, 60, 60 });

	barbarian_img = (UI_Button*)barracks_ui_window->CreateButton(iPoint(297, 510), 37, 36);
	barbarian_img->AddImage("standard", { 765, 0, 37, 36 });
	barbarian_img->SetImage("standard");
	barbarian_img->click_through = true;

	swordsman_img = (UI_Button*)barracks_ui_window->CreateButton(iPoint(395, 510), 37, 36);
	swordsman_img->AddImage("standard", { 765, 36, 37, 36 });
	swordsman_img->SetImage("standard");
	swordsman_img->click_through = true;

	barracks_ui_window->SetEnabledAndChilds(false);

	//Buttons for brokenbuilding

	create_building_button = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(485, 500), 60, 60);
	create_building_button->AddImage("standard", { 705, 0, 60, 60 });
	create_building_button->SetImage("standard");
	create_building_button->AddImage("clicked", { 645, 0, 60, 60 });

	create_building_button2 = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(584, 500), 60, 60);
	create_building_button2->AddImage("standard", { 705, 0, 60, 60 });
	create_building_button2->SetImage("standard");
	create_building_button2->AddImage("clicked", { 645, 0, 60, 60 });

	barrack_img = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(497, 510), 37, 36);
	barrack_img->AddImage("standard", { 808, 48, 39, 38 });
	barrack_img->SetImage("standard");
	barrack_img->click_through = true;

	house_img = (UI_Button*)brokenbuilding_ui_window->CreateButton(iPoint(595, 515), 37, 36);
	house_img->AddImage("standard", { 847, 52, 37, 33 });
	house_img->SetImage("standard");
	house_img->click_through = true;

	brokenbuilding_ui_window->SetEnabledAndChilds(false);

	//player abilities

	player_abilities = (UI_Window*)App->gui->UI_CreateWin(iPoint(400, 200), 200, 60, 12);

	//Battlecry

	battlecry_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9), 60, 60);
	battlecry_ability->AddImage("standard", { 645, 60, 25, 25 });
	battlecry_ability->SetImage("standard");
	battlecry_ability->AddImage("clicked", { 670, 60, 25, 25 });

	battlecry_key = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 19, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_15);
	battlecry_key->SetText("X");

	battlecry_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_15);
	battlecry_cd->SetEnabled(false);

	//drop object interface

	inventory = (UI_Window*)App->gui->UI_CreateWin(iPoint(1000, 200), 25, 25, 13);
	item_drop = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 2, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 2), 60, 60);
	item_drop->AddImage("standard", { 645, 60, 25, 25 });
	item_drop->SetImage("standard");
	item_drop->SetEnabled(false);

	//Whirlwind

	whirlwind_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12), 60, 60);
	whirlwind_ability->AddImage("standard", { 695, 60, 25, 25 });
	whirlwind_ability->SetImage("standard");
	whirlwind_ability->AddImage("clicked", { 720, 60, 25, 25 });

	whirlwind_key = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 19, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12 }, App->font->default_15);
	whirlwind_key->SetText("C");

	whirlwind_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12 }, App->font->default_15);
	whirlwind_cd->SetEnabled(false);

	//Charge

	charge_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18), 60, 60);
	charge_ability->AddImage("standard", { 802, 0, 25, 25 });
	charge_ability->SetImage("standard");
	charge_ability->AddImage("clicked", { 827, 60, 25, 25 });

	charge_key = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 19, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18 }, App->font->default_15);
	charge_key->SetText("V");

	charge_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18 }, App->font->default_15);
	charge_cd->SetEnabled(false);

	return ret;
}

bool Player::PreUpdate()
{
	bool ret = true;

	if (hero->life <= 0) {
		App->stop_exe = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == key_down)
	{
		
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

			//quit options menu
			audio_button->SetEnabled(false);
			controls_button->SetEnabled(false);
			backpause->SetEnabled(false);

			controls_txt->SetEnabled(false);
			backpause_txt->SetEnabled(false);
			audio_txt->SetEnabled(false);
		}
		else
			pause_status = !pause_status;
	}

	if (audio_button->MouseClickEnterLeft())
	{
		audio_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
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
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (whirlwind_button_options->MouseClickEnterLeft())
	{
		whirlwind_button_options->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (charge_button_options->MouseClickEnterLeft())
	{
		charge_button_options->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (move_map_up_button->MouseClickEnterLeft())
	{
		move_map_up_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (move_map_down_button->MouseClickEnterLeft())
	{
		move_map_down_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (move_map_left_button->MouseClickEnterLeft())
	{
		move_map_left_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (move_map_right_button->MouseClickEnterLeft())
	{
		move_map_right_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}
	else if (create_group_button->MouseClickEnterLeft())
	{
		create_group_button->SetImage("clicked");
		button_clicked.Start();
		button_on_clicked = true;
	}

	if (!pause_status && (pause_window->enabled))
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

		//active options button
		audio_button->SetEnabled(true);
		controls_button->SetEnabled(true);
		backpause->SetEnabled(true);

		controls_txt->SetEnabled(true);
		backpause_txt->SetEnabled(true);
		audio_txt->SetEnabled(true);
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

		//quit options menu
		audio_button->SetEnabled(false);
		controls_button->SetEnabled(false);
		backpause->SetEnabled(false);

		controls_txt->SetEnabled(false);
		backpause_txt->SetEnabled(false);
		audio_txt->SetEnabled(false);
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
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && battlecry_button_options->CompareState("clicked"))
	{
		button_on_clicked = false;
		battlecry_button_options->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && whirlwind_button_options->CompareState("clicked"))
	{
		button_on_clicked = false;
		whirlwind_button_options->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && charge_button_options->CompareState("clicked"))
	{
		button_on_clicked = false;
		charge_button_options->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && move_map_up_button->CompareState("clicked"))
	{
		button_on_clicked = false;
		move_map_up_button->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && move_map_down_button->CompareState("clicked"))
	{
		button_on_clicked = false;
		move_map_down_button->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && move_map_left_button->CompareState("clicked"))
	{
		button_on_clicked = false;
		move_map_left_button->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && move_map_right_button->CompareState("clicked"))
	{
		button_on_clicked = false;
		move_map_right_button->SetImage("standard");
	}
	else if (button_clicked.ReadSec() >= 0.1 && button_on_clicked == true && create_group_button->CompareState("clicked"))
	{
		button_on_clicked = false;
		create_group_button->SetImage("standard");
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
	if (pause_status && !pause_window->enabled) {
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
				
	}

	else if ((!pause_status && pause_window->enabled)) {
		pause_window->SetEnabledAndChilds(false);
	}
	
	if (backpause->MouseEnter())
		backpause->SetImage("hovered");
	else if (backpause->MouseOut() && backpause->CompareState("hovered"))
		backpause->SetImage("standard");
	
	if (controls_button->MouseEnter())
		controls_button->SetImage("hovered");
	else if (controls_button->MouseOut() && controls_button->CompareState("hovered"))
		controls_button->SetImage("standard");
	
	if (audio_button->MouseEnter())
		audio_button->SetImage("hovered");
	else if (audio_button->MouseOut() && audio_button->CompareState("hovered"))
		audio_button->SetImage("standard");

	if (backoptions->MouseEnter())
		backoptions->SetImage("hovered");
	else if (backoptions->MouseOut() && backoptions->CompareState("hovered"))
		backoptions->SetImage("standard");

	if (battlecry_button_options->MouseEnter())
		battlecry_button_options->SetImage("hovered");
	else if (battlecry_button_options->MouseOut() && battlecry_button_options->CompareState("hovered"))
		battlecry_button_options->SetImage("standard");

	if (whirlwind_button_options->MouseEnter())
		whirlwind_button_options->SetImage("hovered");
	else if (whirlwind_button_options->MouseOut() && whirlwind_button_options->CompareState("hovered"))
		whirlwind_button_options->SetImage("standard");

	if (charge_button_options->MouseEnter())
		charge_button_options->SetImage("hovered");
	else if (charge_button_options->MouseOut() && charge_button_options->CompareState("hovered"))
		charge_button_options->SetImage("standard");

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
	if (back->MouseEnter())
		back->SetImage("hovered");
	else if (back->MouseOut() && back->CompareState("hovered"))
		back->SetImage("standard");

	//savebutton
	if (save->MouseEnter())
		save->SetImage("hovered");
	else if (save->MouseOut() && save->CompareState("hovered"))
		save->SetImage("standard");
	
	//load button
	if (load->MouseEnter())
		load->SetImage("hovered");
	else if (load->MouseOut() && load->CompareState("hovered"))
		load->SetImage("standard");
	
	// options button

	if (options->MouseEnter())
		options->SetImage("hovered");
	else if (options->MouseOut() && options->CompareState("hovered"))
		options->SetImage("standard");

	//quitbutton
	if (quit_game->MouseEnter())
		quit_game->SetImage("hovered");
	else if (quit_game->MouseOut())
		quit_game->SetImage("standard");

	if (quit_game->MouseClickEnterLeft()) {
		quit_game->SetImage("clicked");
		App->stop_exe = true;
	}
	
	if (!pause_status) {
		if (App->input->GetKey(SDL_SCANCODE_Z) == key_down && App->debug_mode)
			hero->levelup_points += 5;

		//Barracks create unit buttons
		if (create_unit_button->MouseClickEnterLeft() && create_barbarian == true) {
			create_unit_button->SetImage("clicked");

			if (App->scene->scene_test->gold >= 5 && App->scene->scene_test->current_human_resources <= App->scene->scene_test->human_resources_max - 1) {
				Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally, iPoint(barracks_position.x + 100, barracks_position.y + 100));
				App->scene->scene_test->gold -= barb->cost;
				App->scene->scene_test->current_human_resources += barb->human_cost;
			}
		}
		if (create_unit_button->MouseClickOutLeft()) {
			create_unit_button->SetImage("standard");
		}

		if (create_unit_button2->MouseClickEnterLeft() && create_swordsman == true) {
			create_unit_button2->SetImage("clicked");

			if (App->scene->scene_test->gold >= 10 && App->scene->scene_test->current_human_resources <= App->scene->scene_test->human_resources_max - 2) {
				Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally, iPoint(barracks_position.x + 100, barracks_position.y + 100));
				App->scene->scene_test->gold -= sword->cost;
				App->scene->scene_test->current_human_resources += sword->human_cost;
			}
		}

		if (create_unit_button2->MouseClickOutLeft()) {
			create_unit_button2->SetImage("standard");
		}

		//Brokenbuilding create building buttons
		if (create_building_button->MouseClickEnterLeft() && create_building_button->CompareState("standard") && (App->scene->scene_test->gold >= 90 || App->debug_mode)) {
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
					brokenbuilding_ui_window->SetEnabledAndChilds(false);
					App->scene->scene_test->create_barrack = false;
					if (App->questmanager->GetCurrentQuest()->type == quest_type::create && App->questmanager->GetCurrentQuest()->id == quest_id::quest_leader) {
						App->questmanager->GetCurrentQuest()->progress++;
					}
				}
			}
			
		}
		if (create_building_button->MouseClickOutLeft()) {
			create_building_button->SetImage("standard");
		}

		if (create_building_button2->MouseClickEnterLeft() && create_building_button2->CompareState("standard") && (App->scene->scene_test->gold >= 30 || App->debug_mode)) {
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
					brokenbuilding_ui_window->SetEnabledAndChilds(false);
				}
			}

		}

		if (create_building_button2->MouseClickOutLeft()) {
			create_building_button2->SetImage("standard");
		}

		//player abilities
		if (!hero->is_holding_object)
		{
			if (App->input->GetKey(SDL_SCANCODE_X) == key_repeat && battlecry_ability->CompareState("standard")) {
				draw_battlecry_range = true;
				CheckAbilityRange(BATTLECRY_RANGE);
			}
			else if (App->input->GetKey(SDL_SCANCODE_C) == key_repeat && whirlwind_ability->CompareState("standard")) {
				draw_whirlwind_range = true;
				CheckAbilityRange(WHIRLWIND_RANGE);
			}
			else if (App->input->GetKey(SDL_SCANCODE_V) == key_repeat && charge_ability->CompareState("standard")) {
				draw_charge_range = true;
				CheckStraightAbilityRange(CHARGE_RANGE);
			}

			if (App->input->GetKey(SDL_SCANCODE_X) == key_up) {
				draw_battlecry_range = false;
				range_visited.clear();
			}
			if (App->input->GetKey(SDL_SCANCODE_C) == key_up) {
				draw_whirlwind_range = false;
				range_visited.clear();
			}

			//player abilities
			if (!hero->is_holding_object)
			{
				if (App->input->GetKey(SDL_SCANCODE_X) == key_repeat) {
					draw_battlecry_range = true;
					CheckAbilityRange(BATTLECRY_RANGE);
				}
				else if (App->input->GetKey(SDL_SCANCODE_C) == key_repeat) {
					draw_whirlwind_range = true;
					CheckAbilityRange(WHIRLWIND_RANGE);
				}
				else if (App->input->GetKey(SDL_SCANCODE_V) == key_repeat) {
					draw_charge_range = true;
					CheckStraightAbilityRange(CHARGE_RANGE);
				}

				if (App->input->GetKey(SDL_SCANCODE_X) == key_up) {
					draw_battlecry_range = false;
				}
				if (App->input->GetKey(SDL_SCANCODE_C) == key_up) {
					draw_whirlwind_range = false;
				}
				if (App->input->GetKey(SDL_SCANCODE_V) == key_up) {
					draw_charge_range = false;
				}

				//Battlecry


				if ((App->input->GetKey(SDL_SCANCODE_X) == key_repeat && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) && battlecry_ability->CompareState("standard")) {
					battlecry_ability->SetImage("clicked");
					Battlecry();
					battlecry_cd->SetEnabled(true);
					battlecry_timer.Start();
				}
				if (battlecry_timer.ReadSec() >= COOLDOWN_BATTLECRY) {
					battlecry_cd->SetEnabled(false);
					battlecry_ability->SetImage("standard");
				}
				else if (battlecry_timer.ReadSec() >= DURATION_BATTLECRY) {
					StopBuff(-BATTLECRY_BUFF);
				}

				// Whirlwind

				if ((App->input->GetKey(SDL_SCANCODE_C) == key_repeat && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) && whirlwind_ability->CompareState("standard")) {
					whirlwind_ability->SetImage("clicked");
					Whirlwind();
					whirlwind_cd->SetEnabled(true);
					whirlwind_timer.Start();
				}
				if (whirlwind_timer.ReadSec() >= COOLDOWN_WHIRLWIND) {
					whirlwind_cd->SetEnabled(false);
					whirlwind_ability->SetImage("standard");
				}

				//Charge

				if ((App->input->GetKey(SDL_SCANCODE_V) == key_repeat && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) && charge_ability->CompareState("standard")) {
					Charge();
				}

				if (charge_timer.ReadSec() >= 1 && charge_speed_buff == true)
				{
					GetHero()->speed -= CHARGE_SPEED;
					charge_speed_buff = false;
				}

				if (charge_timer.ReadSec() >= 3 && charge_damage_buff == true)
				{
					GetHero()->damage -= CHARGE_DAMAGE;
					charge_damage_buff = false;
				}

				if (charge_timer.ReadSec() >= COOLDOWN_CHARGE) {
					charge_cd->SetEnabled(false);
					charge_ability->SetImage("standard");
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
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down && App->gui->GetMouseHover() == nullptr && App->input->GetKey(SDL_SCANCODE_X) != key_repeat && App->input->GetKey(SDL_SCANCODE_C) != key_repeat && App->input->GetKey(SDL_SCANCODE_V) != key_repeat) {
			iPoint mouse;
			App->input->GetMouseWorld(mouse.x, mouse.y);
			App->entity->UnselectEverything();
			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
				Collider* unit = (*it)->GetCollider();

				if (unit != nullptr)
				{
					if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h && ((*it)->GetType() == entity_type::player || (*it)->GetType() == entity_type::ally || (*it)->GetType() == entity_type::building)) {
						(*it)->SetSelected(true);
					}
				}
				if ((*it)->GetSelected()) {
					if ((*it)->GetType() == building && (*it)->name == barracks) {
						App->entity->UnselectEverything();
						(*it)->SetSelected(true);
						barracks_ui_window->SetEnabledAndChilds(true);
						break;
					}
					else if ((*it)->GetType() == building && (*it)->name == broken_building)
					{
						App->entity->UnselectEverything();
						(*it)->SetSelected(true);
						if (App->questmanager->GetCurrentQuest()->id != quest_id::quest_beggar) {
							brokenbuilding_ui_window->SetEnabledAndChilds(true);
						}
						break;
					}
					else {
						App->entity->selected.push_back((Unit*)*it);
					}
				}
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_down) {
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
					if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h) {
						switch ((*it)->GetType())
						{
						case entity_type::ally:
							MoveToTile(p);
							mouse_over_entity = true;
							break;
						case entity_type::enemy:
							SetAttackingEnemy((Unit*)*it);
							mouse_over_entity = true;
							break;
						case entity_type::npc:
							MoveToTile(p);
							mouse_over_entity = true;
							break;
						case entity_type::player:
							MoveToTile(p);
							mouse_over_entity = true;
							break;
						case entity_type::building:
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
	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_K) == key_down) {

		iPoint mouse;
		App->input->GetMouseWorld(mouse.x, mouse.y);

		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
			Collider* unit = (*it)->GetCollider();

			if (unit != nullptr)
			{
				if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h)
					if ((*it)->life > 0) (*it)->KillEntity();
			}
		}
	}

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;

	if (hero != nullptr) {
		UpdateAttributes();
	}
	
	if (draw_buff == true)
		DrawBuff();

	if (battlecry_timer.ReadSec() <= COOLDOWN_BATTLECRY) {
		DrawCD(1); // 1 == Battlecry
	}

	if (whirlwind_timer.ReadSec() <= COOLDOWN_WHIRLWIND) {
		DrawCD(2); // 2 == Whirlwind
	}

	if (charge_timer.ReadSec() <= COOLDOWN_WHIRLWIND) {
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

		Entity* entity = App->entity->CreateEntity(name, entity_type::ally, { 0, 0 });
		entity->position.create(ally.child("Position").attribute("x").as_int(), ally.child("Position").attribute("y").as_int());
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

	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
		if ((*it)->type == entity_type::ally) {
			pugi::xml_node ally = allies.append_child("Ally");
			ally.append_attribute("name") = (*it)->name;
	
			ally.append_child("Position").append_attribute("x") = (*it)->position.x;
			ally.child("Position").append_attribute("y") = (*it)->position.y;
		}
	}

	return true;
}

void Player::MoveToTile(iPoint tile) {
	for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
		(*it)->path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint((*it)->position), tile);
		(*it)->state = entity_state::entity_move;
		(*it)->attacked_unit = nullptr;
		(*it)->attacked_building = nullptr;
	}
}

void Player::SetAttackingEnemy(Unit* enemy) {
	if (enemy->life > 0) {
		for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
			(*it)->SetAttackingUnit(enemy);
			(*it)->state = entity_state::entity_move_to_enemy;
		}
	}
}

void Player::SetAttackingBuilding(Building* building) {
	if (building->life > 0) {
		for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
			(*it)->SetAttackingBuilding(building);
			(*it)->state = entity_state::entity_move_to_building;
		}
	}
}

void Player::UpdateAttributes() {
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

	if (hero->levelup_points > 0 && !levelup_window->enabled) {
		levelup_window->SetEnabledAndChilds(true);
	}
	else if (levelup_window->enabled) {
		if (hero->levelup_points == 0) {
			levelup_window->SetEnabledAndChilds(false);
		}
		else {
			std::stringstream levelup_points;
			levelup_points << "points left: " << hero->levelup_points;
			level_points_txt->SetText(levelup_points.str());

			if (life_button->MouseClickEnterLeft()) {
				hero->levelup_points -= 1;
				hero->life += 20;
				hero->max_life += 20;
			}
			else if (damage_button->MouseClickEnterLeft()) {
				hero->levelup_points -= 1;
				hero->damage += 1;
			}
			else if (armor_button->MouseClickEnterLeft()) {
				hero->levelup_points -= 1;
				hero->armor += 1;
			}
			else if (pierce_armor_button->MouseClickEnterLeft()) {
				hero->levelup_points -= 1;
				hero->pierce_armor += 1;
			}
		}
	}
}

void Player::SetPickingObject(Object* object)
{
	if (hero->GetSelected() && hero->is_holding_object == false) {
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

void Player::Battlecry() {
	buffed_list.clear();
	std::list<iPoint> frontier;
	std::list<iPoint> visited;

	visited.push_back(App->map->WorldToMapPoint(GetHero()->position));
	frontier.push_back(App->map->WorldToMapPoint(GetHero()->position));

	for (int i = 0; i < BATTLECRY_RANGE; ++i) {
		for (int j = frontier.size(); j > 0; j--) {
			iPoint neighbors[4];
			neighbors[0] = frontier.front() + iPoint(1, 0);
			neighbors[1] = frontier.front() + iPoint(-1, 0);
			neighbors[2] = frontier.front() + iPoint(0, 1);
			neighbors[3] = frontier.front() + iPoint(0, -1);
			frontier.pop_front();

			for (int k = 0; k < 4; k++) {
				if (neighbors[k].x >= 0 && neighbors[k].y >= 0)
				{
					Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
					if (found != nullptr && found->life > 0 && found->type == ally && !found->buffed) {
						buffed_list.push_back(found);
						found->buffed = true;
					}
					else {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
							if (neighbors[k] == *it) {
								is_visited = true;
								break;
							}
						}
						if (!is_visited) {
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

	for (int i = 0; i < WHIRLWIND_RANGE; ++i) {
		for (int j = frontier.size(); j > 0; j--) {
			iPoint neighbors[4];
			neighbors[0] = frontier.front() + iPoint(1, 0);
			neighbors[1] = frontier.front() + iPoint(-1, 0);
			neighbors[2] = frontier.front() + iPoint(0, 1);
			neighbors[3] = frontier.front() + iPoint(0, -1);
			frontier.pop_front();

			for (int k = 0; k < 4; k++) {
				if (neighbors[k].x >= 0 && neighbors[k].y >= 0) {
					Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
					if (found != nullptr && found->life > 0 && found->type == enemy && found->damaged_by_whirlwind == false) {
						found->life -= WHIRLWIND_DAMAGE;
						if (found->life <= 0)
							found->state = entity_death;
						found->damaged_by_whirlwind = true;
					}
					else {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
							if (neighbors[k] == *it) {
								is_visited = true;
								break;
							}
						}
						if (!is_visited) {
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

	for (int i = 0; i < CHARGE_RANGE; ++i) {
		iPoint neighbors[8];
		neighbors[0] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 0);
		neighbors[1] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 0);
		neighbors[2] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, 1 + i);
		neighbors[3] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, -1 - i);
		neighbors[4] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 1 + i);
		neighbors[5] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, -1 - i);
		neighbors[6] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 1 + i);
		neighbors[7] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, -1 - i);

		for (int k = 0; k < 8; k++) {
			if (neighbors[k].x >= 0 && neighbors[k].y >= 0) {
				Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];

				//if (found != nullptr)
				//	col = found->GetCollider();
				//if (found != nullptr && found->life > 0 && found->type == enemy && mouse.x > col->rect.x && mouse.x < col->rect.x + col->rect.w && mouse.y > col->rect.y && mouse.y < col->rect.y + col->rect.h) {

				if (found != nullptr && found->life > 0 && found->type == enemy && App->map->WorldToMapPoint(mouse) == found->position_map)
				{
					GetHero()->speed += CHARGE_SPEED;
					GetHero()->damage += CHARGE_DAMAGE;
					charge_speed_buff = true;
					charge_damage_buff = true;

					switch (k) {
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

				}
				else
				{
					bool is_visited = false;
					for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
						if (neighbors[k] == *it) {
							is_visited = true;
							break;
						}
					}
					if (!is_visited) {
						visited.push_back(neighbors[k]);
					}
				}
			}
		}
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

					for (int k = 0; k < 4; k++) {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = range_visited.begin(); it != range_visited.end(); ++it) {
							if (neighbors[k] == *it) {
								is_visited = true;
								break;
							}
						}
						if (!is_visited) {
							frontier.push_back(neighbors[k]);
							range_visited.push_back(neighbors[k]);
						}
					}
				}
			}
		}
		for (std::list<iPoint>::iterator it = range_visited.begin(); it != range_visited.end(); it++) {
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

		for (int i = 0; i < range; ++i) {
			iPoint neighbors[8];
			neighbors[0] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 0);
			neighbors[1] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 0);
			neighbors[2] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, 1 + i);
			neighbors[3] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(0, -1 - i);
			neighbors[4] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, 1 + i);
			neighbors[5] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(1 + i, -1 - i);
			neighbors[6] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, 1 + i);
			neighbors[7] = App->map->WorldToMapPoint(GetHero()->position) + iPoint(-1 - i, -1 - i);

			for (int k = 0; k < 8; k++) {
				bool is_visited = false;
				for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
					if (neighbors[k] == *it) {
						is_visited = true;
						break;
					}
				}
				if (!is_visited) {
					visited.push_back(neighbors[k]);
				}
			}
		}
		for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); it++) {
			App->scene->LayerBlit(200, App->scene->scene_test->debug_tex, App->map->MapToWorldPoint(*it), { 0, 0, 64, 64 });
		}
	}
}

void Player::DrawBuff()
{
	//if (buffed_list.empty() != true) {
	//	for (std::list<Unit*>::iterator it = buffed_list.begin(); it != buffed_list.end(); it++) {
	//		App->scene->LayerBlit(5, (*it)->entity_texture, { (*it)->position.x + 10, (*it)->position.y + 10 }, (*it)->current_animation->GetAnimationFrame(1) );
	//	}
	//}
}

void Player::StopBuff(int modifier)
{
	draw_buff = false;
	BattlecryModifier(modifier);

	for (std::list<Unit*>::iterator it = buffed_list.begin(); it != buffed_list.end(); it++) {
		(*it)->buffed = false;
	}
	buffed_list.clear();
}

void Player::DrawCD(int ability_number)
{
	std::stringstream oss;

	if (ability_number == 1) {
		int timer = COOLDOWN_BATTLECRY - battlecry_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		battlecry_cd->SetText(txt);
	}

	if (ability_number == 2) {
		int timer = COOLDOWN_WHIRLWIND - whirlwind_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		whirlwind_cd->SetText(txt);
	}

	if (ability_number == 3) {
		int timer = COOLDOWN_CHARGE - charge_timer.ReadSec() + 1;
		oss << timer;
		std::string txt = oss.str();
		charge_cd->SetText(txt);
	}
}