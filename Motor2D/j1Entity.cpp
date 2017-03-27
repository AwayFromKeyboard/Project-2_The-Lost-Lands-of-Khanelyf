#include "j1Entity.h"
#include "Entity.h"
#include "Hero.h"
#include "Log.h"
#include "j1Collisions.h"

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

	
	return ret;
}

bool j1Entity::PostUpdate()
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PostUpdate();

	App->collisions->DebugDraw();

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
		RELEASE(*it);
	}
	return ret;
}

void j1Entity::OnCollision(Collider* col1, Collider* col2)
{
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		(*it)->OnColl(col1, col2);
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



