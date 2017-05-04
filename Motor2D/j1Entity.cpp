#include "j1Entity.h"
#include "Entity.h"
#include "Hero.h"
#include "Barbarian.h"
#include "Swordsman.h"
#include "Log.h"
#include "j1Input.h"
#include "j1Collisions.h"
#include "Barracks.h"
#include "BasicBuilding.h"
#include "j1Gui.h"
#include "Player.h"
#include "Provisions.h"

j1Entity::j1Entity()
{
	name = "entity";
}

j1Entity::~j1Entity()
{
}

bool j1Entity::Awake(pugi::xml_node &)
{
	bool ret = true;

	return ret;
}

bool j1Entity::Start()
{
	bool ret = true;

	return ret;
}

bool j1Entity::PreUpdate()
{
	bool ret = true;

	for(list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PreUpdate();

	AddGroup();
	
	return ret;
}

bool j1Entity::Update(float dt)
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		ret = (*it)->Update(dt);
		(*it)->Draw(dt);
	}

	ManageGroup();

	return ret;
}

bool j1Entity::PostUpdate()
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end();) {
		if ((*it)->to_delete == true) {
			list<Entity*>::iterator it_next = std::next(it);
			DeleteEntity(*it);
			it = it_next;
		}
		else {
			ret = (*it)->PostUpdate();
			++it;
		}
	}

	App->collisions->DebugDraw();

	return ret;
}

bool j1Entity::CleanUp()
{
	bool ret = true;

	selected.clear();

	for (std::list<SelectedList>::iterator it = lists_selected.begin(); it != lists_selected.end(); it++) {
		for (std::list<Unit*>::iterator it2 = (*it).group.begin(); it2 != (*it).group.end(); it2++) {
			(*it).group.erase(it2);
		}
		(*it).group.clear();
		(*it).key_id = 0;
	}
	lists_selected.clear();

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end();)
	{
		list<Entity*>::iterator it_next = ++it;
		--it;
		DeleteEntity(*it);
		it = it_next;
	}

	return ret;
}

void j1Entity::OnCollision(Collider* col1, Collider* col2)
{
	col2->parent->OnColl(col2->parent, col1->parent);
}

Entity* j1Entity::CreateEntity(entity_name name, entity_type type, iPoint pos)
{
	Entity* ret = nullptr;

	switch (name)
	{
	case hero:
		ret = new Hero(type);
		break;
	case barbarian:
		ret = new Barbarian(type);
		break;
	case swordsman:
		ret = new Swordsman(type);
		break;
	case barracks:
		ret = new Barracks(type);
		break;
	case provisions:
		ret = new Provisions(type);
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->LoadEntity(pos);
		ret->Start();
		entity_list.push_back(ret);
	}
	else
		LOG("Entity creation returned nullptr");

	return ret;
}

Entity * j1Entity::CreateBuildingEntity(entity_name name, entity_type type, iPoint pos, int building_rect_number)
{
	Entity* ret = nullptr;

	switch (name)
	{
	case basic_building:
		ret = new BasicBuilding(type, building_rect_number);
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->LoadEntity(pos);
		ret->Start();
		entity_list.push_back(ret);
	}
	else
		LOG("Entity creation returned nullptr");

	return ret;
}

void j1Entity::DeleteEntity(Entity* entity)
{
	entity->CleanUp();
	entity_list.remove(entity);
	RELEASE(entity);
}

void j1Entity::SelectInQuad(const SDL_Rect&  select_rect)
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{

		iPoint unit = (*it)->position;

		if ((*it)->GetType() == entity_type::player || (*it)->GetType() == entity_type::ally || (*it)->GetType() == entity_type::building)
		{
			if (unit.x > select_rect.x && unit.x < select_rect.w && unit.y > select_rect.y && unit.y < select_rect.h)
			{
				(*it)->SetSelected(true);
			}
			else if (unit.x < select_rect.x && unit.x > select_rect.w && unit.y < select_rect.y && unit.y > select_rect.h)
			{
				(*it)->SetSelected(true);
			}
			else if (unit.x > select_rect.x && unit.x < select_rect.w && unit.y < select_rect.y && unit.y > select_rect.h)
			{
				(*it)->SetSelected(true);
			}
			else if (unit.x < select_rect.x && unit.x > select_rect.w && unit.y > select_rect.y && unit.y < select_rect.h)
			{
				(*it)->SetSelected(true);
			}

			if ((*it)->GetSelected())
				if ((*it)->GetType() == building) {
					App->entity->UnselectEverything();
					App->player->barracks_ui_window->SetEnabledAndChilds(true);
					(*it)->SetSelected(true);
				}
				else {
					if (App->player->barracks_ui_window->enabled)
						App->player->barracks_ui_window->SetEnabledAndChilds(false);
					selected.push_back((Unit*)*it);
				}
		}
	}
}

void j1Entity::UnselectEverything()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if ((*it)->GetSelected())
		(*it)->SetSelected(false);
	}
	for (std::list<Unit*>::iterator it = selected.begin(); it != selected.end();) { 
		if(!selected.empty())
			it = selected.erase(it);
		else
			it++;
	}
	if (App->player->barracks_ui_window->enabled)
		App->player->barracks_ui_window->SetEnabledAndChilds(false);

	selected.clear();
}

void j1Entity::AddGroup()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == key_repeat) {
		SelectedList new_group;
		new_group.key_id = NULL;

		if (App->input->GetKey(SDL_SCANCODE_1) == key_down) new_group.key_id = SDL_SCANCODE_1;
		if (App->input->GetKey(SDL_SCANCODE_2) == key_down) new_group.key_id = SDL_SCANCODE_2;
		if (App->input->GetKey(SDL_SCANCODE_3) == key_down) new_group.key_id = SDL_SCANCODE_3;
		if (App->input->GetKey(SDL_SCANCODE_4) == key_down) new_group.key_id = SDL_SCANCODE_4;
		if (App->input->GetKey(SDL_SCANCODE_5) == key_down) new_group.key_id = SDL_SCANCODE_5;
		if (App->input->GetKey(SDL_SCANCODE_6) == key_down) new_group.key_id = SDL_SCANCODE_6;
		if (App->input->GetKey(SDL_SCANCODE_7) == key_down) new_group.key_id = SDL_SCANCODE_7;
		if (App->input->GetKey(SDL_SCANCODE_8) == key_down) new_group.key_id = SDL_SCANCODE_8;
		if (App->input->GetKey(SDL_SCANCODE_9) == key_down) new_group.key_id = SDL_SCANCODE_9;
		if (App->input->GetKey(SDL_SCANCODE_0) == key_down) new_group.key_id = SDL_SCANCODE_0;

		if (selected.size() > 0 && new_group.key_id != NULL) {
			for (std::list<SelectedList>::iterator it = lists_selected.begin(); it != lists_selected.end(); it++) {
				if ((*it).key_id == new_group.key_id) {
					lists_selected.erase(it);
				}
			}
			for (std::list<Unit*>::iterator it = selected.begin(); it != selected.end(); it++) {
				new_group.group.push_back(*it);
			}
			lists_selected.push_back(new_group);
		}
	}
}

void j1Entity::ManageGroup()
{
	for (std::list<SelectedList>::iterator it = lists_selected.begin(); it != lists_selected.end(); it++) {
		if (App->input->GetKey((*it).key_id) == key_down) {
			UnselectEverything();
			selected.clear();
			for (std::list<Unit*>::iterator it2 = (*it).group.begin(); it2 != (*it).group.end(); it2++) {
				selected.push_back(*it2);
				(*it2)->SetSelected(true);
			}
		}
	}
}