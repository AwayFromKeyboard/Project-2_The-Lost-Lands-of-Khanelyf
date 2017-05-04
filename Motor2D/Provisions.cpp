#include "Provisions.h"
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
#include "Player.h"

Provisions::Provisions(entity_type _type)
{
	type = _type;
}

Provisions::~Provisions()
{
}

bool Provisions::LoadEntity(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Objects.xml", doc);
	for (pugi::xml_node object = doc.child("objects").child("object"); object; object = object.next_sibling("object"))
	{
		if (TextCmp(object.attribute("type").as_string(), "Provisions"))
		{
			node = object;
			break;
		}
	}
	if (node)
	{
		position = { pos.x, pos.y };
		collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box").attribute("w").as_int(), node.child("collision_box").attribute("h").as_int() }, COLLIDER_UNIT, App->collisions);
		collision->offset_x = node.child("collision_box").attribute("offset_x").as_int();
		collision->offset_y = node.child("collision_box").attribute("offset_y").as_int();
		collision->parent = this;

		tex_rect = { 0, 0, node.child("rect").attribute("w").as_int(), node.child("rect").attribute("h").as_int() };

		offset = iPoint(node.child("offset").attribute("offset_x").as_int(), node.child("offset").attribute("offset_y").as_int());

		entity_texture = App->tex->LoadTexture(node.child("texture").attribute("value").as_string());

		state = entity_idle;
	}
	else LOG("\nERROR, no node found\n");

	return ret;
}
