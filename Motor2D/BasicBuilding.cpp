#include "BasicBuilding.h"
#include "j1App.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "Scene.h"
#include "j1Map.h"
#include "Entity.h"
#include "j1Collisions.h"
#include "SceneTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "CollisionFilters.h"
#include "Functions.h"
#include "Log.h"
#include "j1Gui.h"

BasicBuilding::BasicBuilding(entity_type _type, int _building_rect_number)
{
	type = _type;
	building_rect_number = _building_rect_number;
}

BasicBuilding::~BasicBuilding()
{
}

bool BasicBuilding::LoadEntity(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Buildings.xml", doc);
	for (pugi::xml_node building = doc.child("buildings").child("building"); building; building = building.next_sibling("building"))
	{
		if (TextCmp(building.attribute("type").as_string(), "BasicBuilding_enemy") && type == entity_type::enemy_building)
		{
			node = building;
			break;
		}
		else if (TextCmp(building.attribute("type").as_string(), "BasicBuilding_ally") && type == entity_type::ally_building)
		{
			node = building;
			break;
		}
	}
	if (node)
	{
		position = { pos.x, pos.y };
		cost = node.child("cost").attribute("value").as_int();

		switch (building_rect_number) {
		case 1:

			collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box1").attribute("w").as_int(), node.child("collision_box1").attribute("h").as_int() }, COLLIDER_UNIT, App->collisions);
			collision->offset_x = node.child("collision_box1").attribute("offset_x").as_int();
			collision->offset_y = node.child("collision_box1").attribute("offset_y").as_int();

			tex_rect = { node.child("rect1").attribute("x").as_int(), node.child("rect1").attribute("y").as_int(), node.child("rect1").attribute("w").as_int(), node.child("rect1").attribute("h").as_int() };
			offset = iPoint(node.child("offset1").attribute("offset_x").as_int(), node.child("offset1").attribute("offset_y").as_int());

			break;

		case 2:

			collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box2").attribute("w").as_int(), node.child("collision_box2").attribute("h").as_int() }, COLLIDER_UNIT, App->collisions);
			collision->offset_x = node.child("collision_box2").attribute("offset_x").as_int();
			collision->offset_y = node.child("collision_box2").attribute("offset_y").as_int();

			tex_rect = { node.child("rect2").attribute("x").as_int(), node.child("rect2").attribute("y").as_int(), node.child("rect2").attribute("w").as_int(), node.child("rect2").attribute("h").as_int() };
			offset = iPoint(node.child("offset2").attribute("offset_x").as_int(), node.child("offset2").attribute("offset_y").as_int());

			break;

		case 3:

			collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box3").attribute("w").as_int(), node.child("collision_box3").attribute("h").as_int() }, COLLIDER_UNIT, App->collisions);
			collision->offset_x = node.child("collision_box3").attribute("offset_x").as_int();
			collision->offset_y = node.child("collision_box3").attribute("offset_y").as_int();

			tex_rect = { node.child("rect3").attribute("x").as_int(), node.child("rect3").attribute("y").as_int(), node.child("rect3").attribute("w").as_int(), node.child("rect3").attribute("h").as_int() };
			offset = iPoint(node.child("offset3").attribute("offset_x").as_int(), node.child("offset3").attribute("offset_y").as_int());

			break;

		}

		entity_texture = App->tex->LoadTexture(node.child("texture").attribute("value").as_string());
		life = node.child("life").attribute("value").as_int();
		state = entity_idle;

	}
	else LOG("\nERROR, no node found\n");

	return ret;
}