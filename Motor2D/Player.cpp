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
#include "j1Window.h"

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

	//player abilities

	player_abilities = (UI_Window*)App->gui->UI_CreateWin(iPoint(400, 200), 200, 60, 12);

	//Battlecry

	battlecry_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9), 60, 60);
	battlecry_ability->AddImage("standard", { 645, 60, 25, 25 });
	battlecry_ability->SetImage("standard");
	battlecry_ability->AddImage("clicked", { 670, 60, 25, 25 });

	battlecry_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 9 }, App->font->default_15);
	battlecry_cd->SetEnabled(false);

	//Whirlwind

	whirlwind_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12), 60, 60);
	whirlwind_ability->AddImage("standard", { 645, 60, 25, 25 });
	whirlwind_ability->SetImage("standard");
	whirlwind_ability->AddImage("clicked", { 670, 60, 25, 25 });

	whirlwind_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 12 }, App->font->default_15);
	whirlwind_cd->SetEnabled(false);

	//Charge

	charge_ability = (UI_Button*)player_abilities->CreateButton(iPoint(App->win->_GetWindowSize().x / 17 + App->win->_GetWindowSize().x / 400, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18), 60, 60);
	charge_ability->AddImage("standard", { 645, 60, 25, 25 });
	charge_ability->SetImage("standard");
	charge_ability->AddImage("clicked", { 670, 60, 25, 25 });

	charge_cd = (UI_Text*)player_abilities->CreateText({ App->win->_GetWindowSize().x / 16, App->win->_GetWindowSize().y - App->win->_GetWindowSize().y / 18 }, App->font->default_15);
	charge_cd->SetEnabled(false);

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

	//player abilities

	//Battlecry

	if (battlecry_ability->MouseEnter() || App->input->GetKey(SDL_SCANCODE_X) == key_repeat) {
		draw_battlecry_range = true;
		CheckAbilityRange(BATTLECRY_RANGE);
	}
	else if (battlecry_ability->MouseOut() || App->input->GetKey(SDL_SCANCODE_X) == key_up) {
		draw_battlecry_range = false;
	}
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

	if (whirlwind_ability->MouseEnter() || App->input->GetKey(SDL_SCANCODE_C) == key_repeat) {
		draw_whirlwind_range = true;
		CheckAbilityRange(WHIRLWIND_RANGE);
	}
	else if (whirlwind_ability->MouseOut() || App->input->GetKey(SDL_SCANCODE_C) == key_up) {
		draw_whirlwind_range = false;
	}
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

	if (charge_ability->MouseEnter() || App->input->GetKey(SDL_SCANCODE_V) == key_repeat) {
		draw_charge_range = true;
		CheckStraightAbilityRange(CHARGE_RANGE);
	}
	else if (charge_ability->MouseOut() || App->input->GetKey(SDL_SCANCODE_V) == key_up) {
		draw_charge_range = false;
	}
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

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down && App->gui->GetMouseHover() == nullptr && App->input->GetKey(SDL_SCANCODE_X) != key_repeat && App->input->GetKey(SDL_SCANCODE_C) != key_repeat && App->input->GetKey(SDL_SCANCODE_V) != key_repeat) {
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
				Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
				if (found != nullptr && found->life > 0 && found->type == enemy) {
					found->life -= WHIRLWIND_DAMAGE;
					if (found->life <= 0)
						found->state = unit_death;
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

void Player::CheckAbilityRange(int range)
{
	if (draw_battlecry_range == true || draw_whirlwind_range == true)
	{
		std::list<iPoint> frontier;
		std::list<iPoint> visited;

		visited.push_back(App->map->WorldToMapPoint(GetHero()->position));
		frontier.push_back(App->map->WorldToMapPoint(GetHero()->position));

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
		for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); it++) {
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
	//		App->scene->LayerBlit(5, (*it)->GetGameObject()->GetTexture(), { (*it)->position.x + 10, (*it)->position.y + 10 }, (*it)->current_animation->GetAnimationFrame(1) );
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
