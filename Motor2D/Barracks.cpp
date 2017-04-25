#include "Barracks.h"
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

Barracks::Barracks(entity_type _type)
{
	type = _type;
}

Barracks::~Barracks()
{
}

bool Barracks::LoadEntity(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Buildings.xml", doc);
	for (pugi::xml_node building = doc.child("buildings").child("building"); building; building = building.next_sibling("building"))
	{
		if (TextCmp(building.attribute("type").as_string(), "Barracks"))
		{
			node = building;
			break;
		}
	}
	if (node)
	{
		position = {pos.x, pos.y};
		collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box").attribute("w").as_int(), node.child("collision_box").attribute("h").as_int() }, COLLIDER_UNIT, App->collisions);
		collision->offset_x = node.child("collision_box").attribute("offset_x").as_int();
		collision->offset_y = node.child("collision_box").attribute("offset_y").as_int();

		cost = node.child("cost").attribute("value").as_int();
		tex_rect = { 0, 0, node.child("rect").attribute("w").as_int(), node.child("rect").attribute("h").as_int() };

		offset = iPoint(node.child("offset").attribute("offset_x").as_int(), node.child("offset").attribute("offset_y").as_int());

		entity_texture = App->tex->LoadTexture(node.child("texture").attribute("value").as_string());
		
		life = node.child("life").attribute("value").as_int();

		state = entity_idle;
	}
	else LOG("\nERROR, no node found\n");

	return ret;
}

bool Barracks::Start()
{
	bool ret = true;

	return ret;
}

void Barracks::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
