#include "Minimap.h"
#include "j1Scene.h"
#include "Entity.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Map.h"

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


	rect = { App->win->_GetWindowSize().x - 292,  App->win->_GetWindowSize().y - 138, 271, 138 };
	scale = { 1, 1 };

	return ret;
}

bool Minimap::PreUpdate()
{
	bool ret = true;





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

	DrawMinimap();

	return ret;
}

bool Minimap::CleanUp()
{
	bool ret = true;

	return ret;
}

iPoint Minimap::WorldToMinimap(const iPoint &world_pos) const
{
	iPoint mini_map_pos;

	//iPoint map_pos = App->map->WorldToMapPoint(world_pos);
	iPoint upper_left_corner = App->map->MapToWorldPoint({ -54, 54 });
	mini_map_pos.x = rect.x + (world_pos.x * 0.1f);
	mini_map_pos.y = rect.y + (world_pos.y * 0.1f);

	return mini_map_pos;
}

void Minimap::DrawMinimap() {
	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
		if ((*it)->type == entity_type::player) {
			App->render->DrawQuad({ WorldToMinimap((*it)->position).x, 500, 5, 5 }, allies.r, allies.g, allies.b, allies.a, true, false);
		}
	}
}