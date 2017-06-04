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
#include "j1Collisions.h"
#include "QuestManager.h"
#include "Barracks.h"
#include "BasicBuilding.h"
#include "Building.h"
#include "Minimap.h"
#include "Object.h"
#include "Player.h"
#include "Boss_Axe_Knight.h"
#include "BrokenBuilding.h"
#include "ParticleManager.h"
#include "Fire.h"
#include "Escorted_NPC.h"

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

bool SceneTest::Start()
{
	if (App->map->Load("map.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	this->quest_0 = { App->map->MapToWorldPoint({ 14, 70 }).x, 	App->map->MapToWorldPoint({ 14, 70 }).y, App->map->MapToWorldPoint({ 37, 71 }).x - App->map->MapToWorldPoint({ 14, 70 }).x, App->map->MapToWorldPoint({ 37, 71 }).y - App->map->MapToWorldPoint({ 14, 70 }).y };
	this->quest_2 = { App->map->MapToWorldPoint({ 55, 50 }).x, 	App->map->MapToWorldPoint({ 55, 50 }).y, App->map->MapToWorldPoint({ 107, 89 }).x - App->map->MapToWorldPoint({ 55, 50 }).x, App->map->MapToWorldPoint({ 107, 89 }).y - App->map->MapToWorldPoint({ 55, 50 }).y };
	this->quest_3 = { App->map->MapToWorldPoint({ 7, 22 }).x, 	App->map->MapToWorldPoint({ 7, 22 }).y, App->map->MapToWorldPoint({ 21, 22 }).x - App->map->MapToWorldPoint({ 7, 22 }).x, App->map->MapToWorldPoint({ 21, 22 }).y - App->map->MapToWorldPoint({ 7, 22 }).y };
	this->quest_4 = { App->map->MapToWorldPoint({ 7, 20 }).x, 	App->map->MapToWorldPoint({ 7, 20 }).y, App->map->MapToWorldPoint({ 17, 20 }).x - App->map->MapToWorldPoint({ 7, 20 }).x, App->map->MapToWorldPoint({ 17, 20 }).y - App->map->MapToWorldPoint({ 7, 20 }).y };
	
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
	cursor_r = { 1, 7, 37, 40 };
	cursor_attack_r = { 115, 7, 37, 40 };
	cursor_build_r = { 153, 7, 37, 40 };
	cursor_object_r = { 39, 7, 37, 40 };
	cursor_ui_r = { 77, 7, 37, 40 };

	current_cursor_r = cursor_r;
	cursor = (UI_Image*)cursor_window->CreateImage(iPoint(0, 0), cursor_r, true);

	general_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(0, 0), App->win->_GetWindowSize().x, App->win->_GetWindowSize().y, 3);
	ui_r = { 0, 88, 1680, 1050 };
	general_ui_image = (UI_Image*)general_ui_window->CreateImage(iPoint(0, 0), ui_r);

	InitCameraMovement();

	App->map->GetEntitiesSpawn();

	gold = 0;

	gold_txt = (UI_Text*)general_ui_window->CreateText({ 33, 1 }, App->font->default_15);
	human_resources_txt = (UI_Text*)general_ui_window->CreateText({ general_ui_window->GetRect().w / 15, 1 }, App->font->default_15);

	App->audio->PlayMusic("audio/music/main_game.ogg");
	App->audio->PauseMusic();

	main_menu_window = (UI_Window*)App->gui->UI_CreateWin({ 0,0 }, 0, 0, 99);
	main_menu_background = (UI_Image*)main_menu_window->CreateImage({ 0, 0 }, { 0, 1139, 1680, 1050 });
	main_menu_background->change_click_through = true;
	
	int distance = App->win->_GetWindowSize().y / 4 - App->win->_GetWindowSize().y / 8;
	int y_position = App->win->_GetWindowSize().y / 8;
	int text_offset = 20;
	SDL_Rect standard_button = { 0, 2190, 283, 109 };
	SDL_Rect hover_button = { 331, 2190, 283, 109 };
	SDL_Rect click_button = { 662, 2190, 283, 109 };

	new_game_button = (UI_Button*)main_menu_window->CreateButton({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8, y_position }, 283, 109);
	new_game_button->change_click_through = true;
	new_game_button->AddImage("standard", standard_button);
	new_game_button->AddImage("hover", hover_button);
	new_game_button->AddImage("click", click_button);
	new_game_button->SetImage("standard");
	new_game_txt = (UI_Text*)main_menu_window->CreateText({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 + text_offset, y_position + text_offset }, App->font->default_48, 0, false, 0, 0, 0);
	new_game_txt->click_through = true;
	new_game_txt->SetText("New Game");
	y_position += distance;

	load_game_button = (UI_Button*)main_menu_window->CreateButton({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8, y_position }, 283, 109);
	load_game_button->change_click_through = true;
	load_game_button->AddImage("standard", standard_button);
	load_game_button->AddImage("hover", hover_button);
	load_game_button->AddImage("click", click_button);
	load_game_button->SetImage("standard");
	load_game_txt = (UI_Text*)main_menu_window->CreateText({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 + text_offset, y_position + text_offset }, App->font->default_48, 0, false, 0, 0, 0);
	load_game_txt->click_through = true;
	load_game_txt->SetText("Load Game");
	y_position += distance;

	credits_button = (UI_Button*)main_menu_window->CreateButton({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8, y_position }, 283, 109);
	credits_button->change_click_through = true;
	credits_button->AddImage("standard", standard_button);
	credits_button->AddImage("hover", hover_button);
	credits_button->AddImage("click", click_button);
	credits_button->SetImage("standard");
	credits_txt = (UI_Text*)main_menu_window->CreateText({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 + text_offset, y_position + text_offset }, App->font->default_48, 0, false, 0, 0, 0);
	credits_txt->click_through = true;
	credits_txt->SetText("Credits");

	credits_window = (UI_Window*)App->gui->UI_CreateWin({ 0, 0 }, 0, 0, 99);
	credits_colored_rect = (UI_ColoredRect*)credits_window->CreateColoredRect({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 - 300, y_position }, 300, 200, { 0, 0, 0, 125 });
	credits_colored_rect->click_through = true;
	int text_y_position = y_position + 5;
	int text_x_position = -295 + App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 ;
	int text_spacing = 20;
	credits_Dani = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_Dani->SetText("Team Leader: Daniel Garcia");
	credits_Dani->click_through = true;
	text_y_position += text_spacing;
	credits_Andreu = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_Andreu->SetText("Manager: Andreu Sacasas");
	credits_Andreu->click_through = true;
	text_y_position += text_spacing;
	credits_Eric = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_Eric->SetText("Designer: Eric Abad");
	credits_Eric->click_through = true;
	text_y_position += text_spacing;
	credits_Sergi = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_Sergi->SetText("QA: Sergi Parra");
	credits_Sergi->click_through = true;
	text_y_position += text_spacing;
	credits_Simon = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_Simon->SetText("Lead Programmer: Simon Stoyanov");
	credits_Simon->click_through = true;
	text_y_position += text_spacing * 2;
	credits_misc = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_misc->SetText("Game made for educational purposes");
	credits_misc->click_through = true;
	text_y_position += text_spacing;
	credits_misc2 = (UI_Text*)credits_window->CreateText({ text_x_position, text_y_position }, App->font->default_15);
	credits_misc2->SetText("University: UPC Tech Talent Center");
	credits_misc2->click_through = true;
	credits_window->SetEnabledAndChilds(false);
	y_position += distance;

	trailer_button = (UI_Button*)main_menu_window->CreateButton({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8, y_position }, 283, 109);
	trailer_button->change_click_through = true;
	trailer_button->AddImage("standard", standard_button);
	trailer_button->AddImage("hover", hover_button);
	trailer_button->AddImage("click", click_button);
	trailer_button->SetImage("standard");
	trailer_txt = (UI_Text*)main_menu_window->CreateText({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 + text_offset, y_position + text_offset }, App->font->default_42, 0, false, 0, 0, 0);
	trailer_txt->click_through = true;
	trailer_txt->SetText("Watch Trailer");
	y_position += distance;

	exit_game_button = (UI_Button*)main_menu_window->CreateButton({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8, y_position }, 283, 109);
	exit_game_button->change_click_through = true;
	exit_game_button->AddImage("standard", standard_button);
	exit_game_button->AddImage("hover", hover_button);
	exit_game_button->AddImage("click", click_button);
	exit_game_button->SetImage("standard");
	exit_game_txt = (UI_Text*)main_menu_window->CreateText({ App->win->_GetWindowSize().x / 2 + App->win->_GetWindowSize().x / 8 + text_offset, y_position + text_offset }, App->font->default_48, 0, false, 0, 0, 0);
	exit_game_txt->click_through = true;
	exit_game_txt->SetText("Exit Game");

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

	if (new_game_button->MouseEnter())
		new_game_button->SetImage("hover");
	else if (new_game_button->MouseClickEnterLeft())
		new_game_button->SetImage("click");
	else if (new_game_button->MouseClickOutLeft())
		main_menu_window->SetEnabledAndChilds(false);
	else if (new_game_button->MouseOut())
		new_game_button->SetImage("standard");

	if (load_game_button->MouseEnter())
		load_game_button->SetImage("hover");
	else if (load_game_button->MouseClickEnterLeft())
		load_game_button->SetImage("click");
	else if (load_game_button->MouseClickOutLeft()) {
		App->LoadGame("Save_File.xml");
		App->player->loaded = true;
		main_menu_window->SetEnabledAndChilds(false);
		App->player->choose_ability_b->enabled = false;
		App->player->choose_ability_b_txt->enabled = false;
		App->player->choose_ability_uw->enabled = false;
		App->player->choose_ability_uw_txt->enabled = false;
	}
	else if (load_game_button->MouseOut())
		load_game_button->SetImage("standard");

	if (credits_button->MouseEnter()) {
		credits_button->SetImage("hover");
		credits_window->SetEnabledAndChilds(true);
	}
	else if (credits_button->MouseOut()) {
		credits_button->SetImage("standard");
		credits_window->SetEnabledAndChilds(false);
	}

	if (trailer_button->MouseEnter() || trailer_button->MouseClickOutLeft())
		trailer_button->SetImage("hover");
	else if (trailer_button->MouseClickEnterLeft())
		trailer_button->SetImage("click");
	else if (trailer_button->MouseOut())
		trailer_button->SetImage("standard");

	if (exit_game_button->MouseEnter() || exit_game_button->MouseClickOutLeftIntern()) {
		exit_game_button->SetImage("hover");
	}
	else if (exit_game_button->MouseClickEnterLeft()){
		exit_game_button->SetImage("click");
		App->stop_exe = true;
	}
	else if (exit_game_button->MouseOut())
		exit_game_button->SetImage("standard");

	if (enemy_waves_active)
	{
		if (start_waves_timer)
		{
			enemy_waves_timer.Start();
			start_waves_timer = false;
			enemy_wave_number = enemy_waves::first;
		}

		if (enemy_wave_number == enemy_waves::first && enemy_waves_timer.ReadSec() >= 60) {
			enemy_wave_number = enemy_waves::second;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
		}
		else if (enemy_wave_number == enemy_waves::second && enemy_waves_timer.ReadSec() >= 180) {
			enemy_wave_number = enemy_waves::third;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
		}
		else if (enemy_wave_number == enemy_waves::third && enemy_waves_timer.ReadSec() >= 300) {
			enemy_wave_number = enemy_waves::fourth;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
			Barbarian* barb3 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(92, 3));
			barb3->move_to_ally_building = true;
			barb3->stay_still = false;
		}
		else if (enemy_wave_number == enemy_waves::fourth && enemy_waves_timer.ReadSec() >= 420) {
			enemy_wave_number = enemy_waves::fifth;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
			Barbarian* barb3 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(92, 3));
			barb3->move_to_ally_building = true;
			barb3->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(91, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;

		}
		else if (enemy_wave_number == enemy_waves::fifth && enemy_waves_timer.ReadSec() >= 540) {
			enemy_wave_number = enemy_waves::sixth;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
			Barbarian* barb3 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(92, 3));
			barb3->move_to_ally_building = true;
			barb3->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(91, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;
		}
		else if (enemy_wave_number == enemy_waves::sixth && enemy_waves_timer.ReadSec() >= 660) {
			enemy_wave_number = enemy_waves::seventh;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
			Barbarian* barb3 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(92, 3));
			barb3->move_to_ally_building = true;
			barb3->stay_still = false;
			Barbarian* barb4 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(91, 3));
			barb4->move_to_ally_building = true;
			barb4->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(90, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;
		}
		else if (enemy_wave_number == enemy_waves::seventh && enemy_waves_timer.ReadSec() >= 780) {
			enemy_wave_number = enemy_waves::eighth;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(93, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;
			Swordsman* sword1 = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(92, 3));
			sword1->move_to_ally_building = true;
			sword1->stay_still = false;
		}
		else if (enemy_wave_number == enemy_waves::eighth && enemy_waves_timer.ReadSec() >= 900) {
			enemy_wave_number = enemy_waves::ninth;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(92, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;
			Swordsman* sword1 = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(91, 3));
			sword1->move_to_ally_building = true;
			sword1->stay_still = false;

		}
		else if (enemy_wave_number == enemy_waves::ninth && enemy_waves_timer.ReadSec() >= 1020) {
			enemy_wave_number = enemy_waves::tenth;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Barbarian* barb2 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(93, 3));
			barb2->move_to_ally_building = true;
			barb2->stay_still = false;
			Barbarian* barb3 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(92, 3));
			barb3->move_to_ally_building = true;
			barb3->stay_still = false;
			Barbarian* barb4 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(91, 3));
			barb4->move_to_ally_building = true;
			barb4->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(90, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;
			Swordsman* sword1 = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(89, 3));
			sword1->move_to_ally_building = true;
			sword1->stay_still = false;
		}

		else if (enemy_wave_number == enemy_waves::tenth && enemy_waves_timer.ReadSec() >= 1020) {
			enemy_wave_number = enemy_waves::none;

			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(95, 3));
			barb->move_to_ally_building = true;
			barb->stay_still = false;
			Barbarian* barb1 = (Barbarian*)App->entity->CreateEntity(barbarian, enemy, App->map->MapToWorld(94, 3));
			barb1->move_to_ally_building = true;
			barb1->stay_still = false;
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(93, 3));
			sword->move_to_ally_building = true;
			sword->stay_still = false;
			Swordsman* sword1 = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(92, 3));
			sword1->move_to_ally_building = true;
			sword1->stay_still = false;
			Swordsman* sword2 = (Swordsman*)App->entity->CreateEntity(swordsman, enemy, App->map->MapToWorld(91, 3));
			sword2->move_to_ally_building = true;
			sword2->stay_still = false;

		}
	}

	return true;
}

bool SceneTest::Update(float dt)
{
	iPoint mouse;
	App->input->GetMouseWorld(mouse.x, mouse.y);
	
	UpdateCameraMovement();

	if (App->player->pause_status || App->player->audio_muted) {
		
		App->audio->PauseMusic();
	}else
		App->audio->ResumeMusic();
	
	App->map->Draw();

	cursor->Set(iPoint(mouse.x, mouse.y), current_cursor_r);

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

void SceneTest::OnColl(Collider* col1, Collider* col2)
{
}

void SceneTest::CheckUnitCreation(iPoint p)
{
	std::stringstream oss;
	oss << gold;
	gold_txt->SetText(oss.str());

	std::stringstream oss2;
	oss2 << current_human_resources << "/" << human_resources_max;
	human_resources_txt->SetText(oss2.str());

	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_A) == key_down)
	{
		Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_S) == key_down)
	{
		Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_B) == key_down)
	{
		Barracks* barrack = (Barracks*)App->entity->CreateEntity(barracks, building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_W) == key_down)
	{
		BossAxeKnight* boss_axe_knight = (BossAxeKnight*)App->entity->CreateEntity(boss, enemy_boss, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_U) == key_down)
	{
		BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, enemy_building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y), 1);
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_I) == key_down)
	{
		BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, enemy_building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y), 2);
	}
	else if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_O) == key_down)
	{
		BasicBuilding* basicbuilding = (BasicBuilding*)App->entity->CreateBuildingEntity(basic_building, enemy_building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y), 3);
	}
	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_J) == key_down)
	{
		Entity* object_entity = App->entity->CreateEntity(provisions, object, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	if (App->debug_mode && App->input->GetKey(SDL_SCANCODE_N) == key_down)
	{
		BrokenBuilding* brokenbuilding = (BrokenBuilding*)App->entity->CreateEntity(broken_building, building, App->map->MapToWorld(p.x + TROOP_OFFSET, p.y));
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == key_down && App->debug_mode)
	{
		App->player->GetHero()->levelup_points += 5;
	}

	if (App->questmanager->GetCurrentQuest()->id == quest_id::quest_escort && !escortedNPC_created) {
		escortedNPC_created = true;
		EscortedNPC* escorted_npc = (EscortedNPC*)App->entity->CreateEntity(npc_escort, npc, App->map->MapToWorld(90, 70));
	}

	if (App->questmanager->GetCurrentQuest()->id == quest_id::quest_provisions && !mission_provision_created) {
		mission_provision_created = true;
		Entity* object_entity = App->entity->CreateEntity(provisions, object, App->map->MapToWorld(40, 6));
	}

}

void SceneTest::IncreaseGold(int gold)
{
	this->gold += gold;
}

UI_Image * SceneTest::GetCursor()
{
	return cursor;
}

void SceneTest::SetCurrentCursor(SDL_Rect new_cursor)
{
	current_cursor_r = new_cursor;
}