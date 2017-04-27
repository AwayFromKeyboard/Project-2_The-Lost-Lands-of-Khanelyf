#include "Minimap.h"
#include "j1Scene.h"
#include "j1Scene.h"
#include "Entity.h"

Minimap::Minimap()
{
	name = "Minimap";
}

Minimap::~Minimap()
{
}

bool Minimap::Start()
{
	bool ret = true;
	
	QuadColor red(255, 0, 0, 255);
	enemies = red;
	QuadColor green(0, 255, 0, 255);
	allies = green;
	QuadColor blue(0, 0, 255, 255);
	buildings = blue;

	return ret;
}

bool Minimap::PreUpdate()
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
		if ((*it)->type == entity_type::player) {
			App->scene->LayerDrawQuad({ 50, 50, 50, 50 }, allies.r, allies.g, allies.b, allies.a, true, false, 5);
		}
	}

	return ret;
}

bool Minimap::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Minimap::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Minimap::CleanUp()
{
	bool ret = true;

	return ret;
}
