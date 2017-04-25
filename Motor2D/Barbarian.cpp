#include "Barbarian.h"
#include "GameObject.h"
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

Barbarian::Barbarian(entity_type _type)
{
	type = _type;
}

Barbarian::~Barbarian()
{
}

bool Barbarian::LoadEntity(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Units.xml", doc);
	for (pugi::xml_node unit = doc.child("units").child("unit"); unit; unit = unit.next_sibling("unit"))
	{
		if (TextCmp(unit.attribute("type").as_string(), "Barbarian_enemy") && type == entity_type::enemy)
		{
			node = unit;
			break;
		}
		else if (TextCmp(unit.attribute("type").as_string(), "Barbarian_ally") && type == entity_type::ally)
		{
			node = unit;
			break;
		}
		else if (TextCmp(unit.attribute("type").as_string(), "Barbarian_npc") && type == entity_type::npc)
		{
			node = unit;
			break;
		}
	}
	if (node)
	{
		game_object = new GameObject(iPoint(150, 150), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

		pos2 = { pos.x, pos.y };
		collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box").attribute("w").as_int(), node.child("collision_box").attribute("h").as_int() }, COLLIDER_UNIT, App->collisions);
		collision->offset_x = node.child("collision_box").attribute("offset_x").as_int();
		collision->offset_y = node.child("collision_box").attribute("offset_y").as_int();

		game_object->SetListener((j1Module*)App->entity);
		game_object->SetFixedRotation(true);

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

		std::string texture = node.child("texture").attribute("value").as_string();
		game_object->SetTexture(App->tex->LoadTexture(texture.c_str()));
		node = node.child("animations");
		game_object->LoadAnimationsFromUnitsXML(node, this);

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
		App->entity->unit_game_objects_list.push_back(game_object);

		state = entity_state::entity_idle;
	}
	else LOG("\nERROR, no node found\n");

	return ret;
}

void Barbarian::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}