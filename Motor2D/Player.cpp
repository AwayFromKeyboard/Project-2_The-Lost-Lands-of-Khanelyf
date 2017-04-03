#include "Player.h"
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
#include "Log.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	bool ret = true;
	return ret;
}

bool Player::PreUpdate()
{
	bool ret = true;

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

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) {
		iPoint mouse;
		App->input->GetMouseWorld(mouse.x, mouse.y);
		App->entity->UnselectEverything();
		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
			Collider* unit = (*it)->GetCollider();
			
			if (mouse.x > unit->rect.x && mouse.x < unit->rect.x + unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h && ((*it)->GetType() == entity_type::player || (*it)->GetType() == entity_type::ally)) {
				(*it)->SetSelected(true);
			}
			if ((*it)->GetSelected())
				App->entity->selected.push_back((Unit*)*it);
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
	for (std::list<Unit*>::iterator it = App->entity->selected.begin(); it != App->entity->selected.end(); it++) {
		(*it)->SetAttackingUnit(enemy);
		(*it)->state = unit_state::unit_move_to_enemy;
	}
}