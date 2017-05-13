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
#include "BrokenBuilding.h"
#include "SceneTest.h"
#include "j1Scene.h"
#include "QuestManager.h"
#include "Tower.h"

j1Entity::j1Entity()
{
	name = "entities";
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

	if (loaded && !App->player->pause_status)
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
	if (col1 != nullptr && col2 != nullptr)
		col2->parent->OnColl(col2->parent, col1->parent);
}

bool j1Entity::Load(pugi::xml_node& data)
{
	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
		(*it)->to_delete = true;
	}
	App->collisions->colliders.clear();

	pugi::xml_node enemies = data.child("Enemies");
	pugi::xml_node npcs = data.child("NPCs");
	pugi::xml_node ally_buildings = data.child("Ally_Buildings");
	pugi::xml_node enemy_buildings = data.child("Enemy_Buildings");
	pugi::xml_node objects = data.child("Objects");
	pugi::xml_node player = data.parent().child("player");

	for (pugi::xml_node enemy = enemies.child("Enemy"); enemy != NULL; enemy = enemy.next_sibling()) {
		int _name = enemy.attribute("name").as_int();
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

		Entity* entity = App->entity->CreateEntity(name, entity_type::enemy, { 0, 0 });
		entity->position.create(enemy.child("Position").attribute("x").as_int(), enemy.child("Position").attribute("y").as_int());
	}
	for (pugi::xml_node npc = npcs.child("NPC"); npc != NULL; npc = npc.next_sibling()) {
		int _name = npc.attribute("name").as_int();
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

		Entity* entity = App->entity->CreateEntity(name, entity_type::npc, { 0, 0 });
		entity->position.create(npc.child("Position").attribute("x").as_int(), npc.child("Position").attribute("y").as_int());
	}
	for (pugi::xml_node ally_b = ally_buildings.child("Ally_Building"); ally_b != NULL; ally_b = ally_b.next_sibling()) {
		int _name = ally_b.attribute("name").as_int();
		entity_name name;

		switch (_name)
		{
		case barracks:
			name = barracks;
			break;
		case basic_building:
			name = basic_building;
			break;
		}
		
		Entity* entity = App->entity->CreateBuildingEntity(name, entity_type::ally_building, { 0, 0 }, ally_b.child("Rect").attribute("number").as_int());
		entity->position.create(ally_b.child("Position").attribute("x").as_int(), ally_b.child("Position").attribute("y").as_int());
	}
	for (pugi::xml_node enemy_b = enemy_buildings.child("Enemy_Building"); enemy_b != NULL; enemy_b = enemy_b.next_sibling()) {
		int _name = enemy_b.attribute("name").as_int();
		entity_name name;

		switch (_name)
		{
		case barracks:
			name = barracks;
			break;
		case basic_building:
			name = basic_building;
			break;
		}

		Entity* entity = App->entity->CreateBuildingEntity(name, entity_type::enemy_building, { 0, 0 }, enemy_b.child("Rect").attribute("number").as_int());
		entity->position.create(enemy_b.child("Position").attribute("x").as_int(), enemy_b.child("Position").attribute("y").as_int());

	}
	for (pugi::xml_node object = objects.child("Object"); object != NULL; object = object.next_sibling()) {
		int _name = object.attribute("name").as_int();
		entity_name name;

		switch (_name)
		{
		case provisions:
			name = provisions;
			break;
		}

		Object* entity = (Object*)App->entity->CreateEntity(name, entity_type::object, { 0, 0 });
		entity->position.create(object.child("Position").attribute("x").as_int(), object.child("Position").attribute("y").as_int());

		entity->pickable = object.child("Properties").attribute("Pickable").as_bool();
		entity->is_carried = object.child("Properties").attribute("IsCarried").as_bool();
	}

	App->player->Load(player);

	loaded = true;

	return true;
}

bool j1Entity::Save(pugi::xml_node& data) const
{
	pugi::xml_node enemies = data.append_child("Enemies");
	pugi::xml_node npcs = data.append_child("NPCs");
	pugi::xml_node ally_buildings = data.append_child("Ally_Buildings");
	pugi::xml_node enemy_buildings = data.append_child("Enemy_Buildings");
	pugi::xml_node objects = data.append_child("Objects");
	pugi::xml_node player = data.parent().append_child("player");

	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
		if ((*it)->type == entity_type::enemy) {
			pugi::xml_node enemy = enemies.append_child("Enemy");
			enemy.append_attribute("name") = (*it)->name;

			enemy.append_child("Position").append_attribute("x") = (*it)->position.x;
			enemy.child("Position").append_attribute("y") = (*it)->position.y;
		}
		else if ((*it)->type == entity_type::ally_building) {
			pugi::xml_node building = ally_buildings.append_child("Ally_Building");
			building.append_attribute("name") = (*it)->name;

			building.append_child("Position").append_attribute("x") = (*it)->position.x;
			building.child("Position").append_attribute("y") = (*it)->position.y;

			building.append_child("Rect").append_attribute("number") = ((Building*)*it)->building_rect_number;
		}
		else if ((*it)->type == entity_type::enemy_building) {
			pugi::xml_node building = enemy_buildings.append_child("Enemy_Building");
			building.append_attribute("name") = (*it)->name;

			building.append_child("Position").append_attribute("x") = (*it)->position.x;
			building.child("Position").append_attribute("y") = (*it)->position.y;

			building.append_child("Rect").append_attribute("number") = ((Building*)*it)->building_rect_number;
		}
		else if ((*it)->type == entity_type::object) {
			pugi::xml_node object = objects.append_child("Object");
			object.append_attribute("name") = (*it)->name;

			object.append_child("Position").append_attribute("x") = (*it)->position.x;
			object.child("Position").append_attribute("y") = (*it)->position.y;

			object.append_child("Properties").append_attribute("Pickable") = ((Object*)*it)->pickable;
			object.child("Properties").append_attribute("IsCarried") = ((Object*)*it)->is_carried;
		}
		else if ((*it)->type == entity_type::npc) {
			pugi::xml_node npc = npcs.append_child("NPC");
			npc.append_attribute("name") = (*it)->name;

			npc.append_child("Position").append_attribute("x") = (*it)->position.x;
			npc.child("Position").append_attribute("y") = (*it)->position.y;
		}
	}

	App->player->Save(player);

	return true;
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
	case broken_building:
		ret = new BrokenBuilding(type);
		break;
	case towers:
		ret = new Tower(type);
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->LoadEntity(pos, name);
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
		ret->LoadEntity(pos, name);
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

		if ((*it)->GetType() == entity_type::player || (*it)->GetType() == entity_type::ally)
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
			{
				if (App->player->barracks_ui_window->enabled)
					App->player->barracks_ui_window->SetEnabledAndChilds(false);
				if (App->player->brokenbuilding_ui_window->enabled)
					App->player->brokenbuilding_ui_window->SetEnabledAndChilds(false);
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
	if (App->player->brokenbuilding_ui_window->enabled)
		App->player->brokenbuilding_ui_window->SetEnabledAndChilds(false);

	selected.clear();
}

void j1Entity::AddGroup()
{
	if (App->input->controls[CREATE_GROUP] == key_repeat) {
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