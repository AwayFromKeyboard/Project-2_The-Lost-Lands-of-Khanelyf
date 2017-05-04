#include "Swordsman.h"
#include "Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "CollisionFilters.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "Functions.h"
#include "j1Textures.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "Log.h"
#include "j1Collisions.h"

Swordsman::Swordsman(entity_type _type)
{
	type = _type;
}

Swordsman::~Swordsman()
{
}

bool Swordsman::LoadEntity(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Units.xml", doc);
	for (pugi::xml_node unit = doc.child("units").child("unit"); unit; unit = unit.next_sibling("unit"))
	{
		if (TextCmp(unit.attribute("type").as_string(), "Swordsman_enemy") && type == entity_type::enemy)
		{
			node = unit;
			break;
		}
		else if (TextCmp(unit.attribute("type").as_string(), "Swordsman_ally") && type == entity_type::ally)
		{
			node = unit;
			break;
		}
		else if (TextCmp(unit.attribute("type").as_string(), "Swordsman_npc") && type == entity_type::npc)
		{
			node = unit;
			break;
		}
	}
	if (node)
	{
		position = { pos.x, pos.y };
		collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box").attribute("w").as_int(), node.child("collision_box").attribute("h").as_int() }, COLLIDER_UNIT, App->entity);
		collision->offset_x = node.child("collision_box").attribute("offset_x").as_int();
		collision->offset_y = node.child("collision_box").attribute("offset_y").as_int();
		collision->parent = this;

		cost = node.child("cost").attribute("value").as_int(0);
		human_cost = node.child("human_cost").attribute("value").as_int(0);
		gold_drop = node.child("gold_drop").attribute("value").as_int(0);
		speed = node.child("speed").attribute("value").as_float();
		damage = node.child("damage").attribute("value").as_int();
		armor = node.child("armor").attribute("value").as_int();
		pierce_armor = node.child("pierce_armor").attribute("value").as_int();
		range = node.child("range").attribute("value").as_int();
		life = node.child("life").attribute("value").as_int();
		radius_of_action = node.child("radius_of_action").attribute("value").as_int(0);

		entity_texture = App->tex->LoadTexture(node.child("texture").attribute("value").as_string());
		node = node.child("animations");
		animator->LoadAnimationsFromUnitsXML(node, this);

		i_offset.create(node.child("idle").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());
		m_offset.create(node.child("move").attribute("offset_x").as_int(), node.child("move").attribute("offset_y").as_int());
		a_offset.create(node.child("attack").attribute("offset_x").as_int(), node.child("attack").attribute("offset_y").as_int());
		d_offset.create(node.child("death").attribute("offset_x").as_int(), node.child("death").attribute("offset_y").as_int());
		de_offset.create(node.child("decompose").attribute("offset_x").as_int(), node.child("decompose").attribute("offset_y").as_int());

		flip_i_offset = node.child("idle").attribute("offset_flip").as_int();
		flip_m_offset = node.child("move").attribute("offset_flip").as_int();
		flip_a_offset = node.child("attack").attribute("offset_flip").as_int();
		flip_d_offset = node.child("death").attribute("offset_flip").as_int();
		flip_de_offset = node.child("decompose").attribute("offset_flip").as_int();

		current_animation = &i_south;
		direction = { 0, 1 };

		state = entity_state::entity_idle;
	}
	else LOG("\nERROR, no node found\n");

	return ret;
}