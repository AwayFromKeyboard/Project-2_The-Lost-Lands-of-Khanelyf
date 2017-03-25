#include "j1Entity.h"
#include "Entity.h"
#include "Hero.h"
#include "Log.h"
#include "GameObject.h"
#include "j1Input.h"

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

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PostUpdate();


	return ret;
}

bool j1Entity::CleanUp()
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		ret = (*it)->CleanUp();
	}
	for (std::list<GameObject*>::iterator it = App->entity->unit_game_objects_list.begin(); it != App->entity->unit_game_objects_list.end(); it++) {
		App->entity->unit_game_objects_list.erase(it);
		RELEASE(*it);
	}
	for (std::list<Entity*>::iterator it = selected.begin(); it != selected.end(); it++) {
		selected.erase(it);
	}
	selected.clear();
	for (std::list<SelectedList>::iterator it = lists_selected.begin(); it != lists_selected.end(); it++) {
		for (std::list<Entity*>::iterator it2 = (*it).group.begin(); it2 != (*it).group.end(); it2++) {
			(*it).group.erase(it2);
		}
		(*it).group.clear();
		(*it).key_id = 0;
	}
	lists_selected.clear();

	return ret;
}

void j1Entity::OnCollision(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		(*it)->OnColl(bodyA, bodyB, fixtureA, fixtureB);
}

Entity* j1Entity::CreateEntity(entity_name entity)
{
	Entity* ret = nullptr;

	switch (entity)
	{
	case player:
		ret = new Hero();
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->LoadEntity();
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
		iPoint unit = (*it)->GetGameObject()->GetPos();
		
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
			selected.push_back(*it);
	}
}

void j1Entity::UnselectEverything()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if ((*it)->GetSelected())
		(*it)->SetSelected(false);
	}
	for (std::list<Entity*>::iterator it = selected.begin(); it != selected.end(); it++) {
		selected.erase(it);
	}
	selected.clear();
}

void j1Entity::AddGroup()
{
	if (selected.size() > 0 && App->input->GetNumPressed() >= 30 && App->input->GetNumPressed() <= 39) {
		SelectedList new_group;
		new_group.key_id = App->input->GetNumPressed();
		for (std::list<SelectedList>::iterator it = lists_selected.begin(); it != lists_selected.end(); it++) {
			if ((*it).key_id == new_group.key_id) {
				lists_selected.erase(it);
			}
		}
		for (std::list<Entity*>::iterator it = selected.begin(); it != selected.end(); it++) {
			new_group.group.push_back(*it);
		}
		lists_selected.push_back(new_group);
	}
}

void j1Entity::ManageGroup()
{
	for (std::list<SelectedList>::iterator it = lists_selected.begin(); it != lists_selected.end(); it++) {
		if (App->input->GetKey((*it).key_id) == key_down) {
			UnselectEverything();
			for (std::list<Entity*>::iterator it2 = (*it).group.begin(); it2 != (*it).group.end(); it2++) {
				(*it2)->SetSelected(true);
			}
		}
	}
}