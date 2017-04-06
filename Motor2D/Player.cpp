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
#include "GameObject.h"
#include "Defs.h"
#include "Hero.h"
#include "Log.h"
#include "Barbarian.h"
#include "Swordsman.h"
#include "SceneTest.h"
#include "j1Scene.h"
#include "Barracks.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	bool ret = true;

	attributes_window = (UI_Window*)App->gui->UI_CreateWin({ 0, 0 }, 0, 0, 10);
	life_txt = (UI_Text*)attributes_window->CreateText({ 165, 487 }, App->font->default_15);
	damage_txt = (UI_Text*)attributes_window->CreateText({ 165, 503 }, App->font->default_15);
	armor_txt = (UI_Text*)attributes_window->CreateText({ 165, 518 }, App->font->default_15);
	pierce_armor_txt = (UI_Text*)attributes_window->CreateText({ 165, 533 }, App->font->default_15);
	
	levelup_window = (UI_Window*)App->gui->UI_CreateWin({ 0, 0 }, 0, 0, 10);
	life_button = (UI_Button*)levelup_window->CreateButton({ 148, 493 }, 10, 10);
	life_button->AddImage("standard", { 792, 73, 10, 10 });
	life_button->SetImage("standard");
	damage_button = (UI_Button*)levelup_window->CreateButton({ 148, 509 }, 10, 10);
	damage_button->AddImage("standard", { 792, 73, 10, 10 });
	damage_button->SetImage("standard");
	armor_button = (UI_Button*)levelup_window->CreateButton({ 148, 524 }, 10, 10);
	armor_button->AddImage("standard", { 792, 73, 10, 10 });
	armor_button->SetImage("standard");
	pierce_armor_button = (UI_Button*)levelup_window->CreateButton({ 148, 539 }, 10, 10);
	pierce_armor_button->AddImage("standard", { 792, 73, 10, 10 });
	pierce_armor_button->SetImage("standard");
	level_points_txt = (UI_Text*)levelup_window->CreateText({ 130, 478 }, App->font->default_10);
	levelup_window->SetEnabledAndChilds(false);

	barracks_ui_window = (UI_Window*)App->gui->UI_CreateWin(iPoint(280, 200), 225, 144, 99);

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

	return ret;
}

bool Player::PreUpdate()
{
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == key_down)
		App->debug_mode = !App->debug_mode;

	if (App->input->GetKey(SDL_SCANCODE_Z) == key_down && App->debug_mode)
		hero->levelup_points += 5;

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


	if (create_unit_button->MouseClickEnterLeft() && create_barbarian == true) {
		create_unit_button->SetImage("clicked");

		if (App->scene->scene_test->gold >= 5 && App->scene->scene_test->current_human_resources <= App->scene->scene_test->human_resources_max - 1) {
			Barbarian* barb = (Barbarian*)App->entity->CreateEntity(barbarian, ally);
			barb->game_object->SetPos(fPoint(barracks_position.x + 300, barracks_position.y)); // Barracks position
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
			Swordsman* sword = (Swordsman*)App->entity->CreateEntity(swordsman, ally);
			sword->game_object->SetPos(fPoint(barracks_position.x + 300, barracks_position.y)); // Barracks position
			App->scene->scene_test->gold -= sword->cost;
			App->scene->scene_test->current_human_resources += sword->human_cost;
		}
	}
	if (create_unit_button2->MouseClickOutLeft()) {
		create_unit_button2->SetImage("standard");
	}


	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down && App->gui->GetMouseHover() == nullptr) {
		iPoint mouse;
		App->input->GetMouseWorld(mouse.x, mouse.y);
		App->entity->UnselectEverything();
		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
			Collider* unit = (*it)->GetCollider();
			
			if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h && ((*it)->GetType() == entity_type::player || (*it)->GetType() == entity_type::ally || (*it)->GetType() == entity_type::building)) {
				(*it)->SetSelected(true);
			}
			if ((*it)->GetSelected()) {
				if ((*it)->GetType() == building) {
					App->entity->UnselectEverything();
					(*it)->SetSelected(true);
					barracks_ui_window->SetEnabledAndChilds(true);
					barracks_position = (*it)->GetGameObject()->GetPos();
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

		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
			Collider* unit = (*it)->GetCollider();

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
				default:
					break;
				}
			}

			if (mouse_over_entity)
				continue;
		}
		
		if (!mouse_over_entity) {
			MoveToTile(p);
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

	return ret;
}

bool Player::CleanUp()
{
	bool ret = true;
	return ret;
}

void Player::MoveToTile(iPoint tile) {
	for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
		(*it)->path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint((*it)->game_object->GetPos()), tile);
		(*it)->state = unit_state::unit_move;
		(*it)->attacked_unit = nullptr;
	}
}

void Player::SetAttackingEnemy(Unit* enemy) {
	if (enemy->life > 0) {
		for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
			(*it)->SetAttackingUnit(enemy);
			(*it)->state = unit_state::unit_move_to_enemy;
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

void Player::SetHero(Hero * hero)
{
	this->hero = hero;
}

Hero* Player::GetHero()
{
	return hero;
}
