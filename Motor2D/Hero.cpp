#include "Hero.h"
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

Hero::Hero()
{
}

Hero::~Hero()
{
}

bool Hero::LoadEntity()
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Units.xml", doc);
	for (pugi::xml_node unit = doc.child("units").child("unit"); unit; unit = unit.next_sibling("unit"))
	{
		if (TextCmp(unit.attribute("type").as_string(), "Hero"))
		{
			node = unit;
			break;
		}
	}
	if (node)
	{
		game_object = new GameObject(iPoint(150, 150), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);


		game_object->CreateCollision(COLLISION_ADJUSTMENT, 20, 20, fixture_type::f_t_null);
		game_object->SetListener((j1Module*)App->entity);
		game_object->SetFixedRotation(true);
		
		type = player;

		cost = node.child("cost").attribute("value").as_int();
		speed = node.child("speed").attribute("value").as_float();
		damage = node.child("damage").attribute("value").as_int();
		armor = node.child("armor").attribute("value").as_int();
		pierce_armor = node.child("pierce_armor").attribute("value").as_int(); 
		range = node.child("range").attribute("value").as_int();
		life = node.child("life").attribute("value").as_int();

		std::string texture = node.child("texture").attribute("value").as_string();
		game_object->SetTexture(App->tex->LoadTexture(texture.c_str()));
		node = node.child("animations");
		game_object->LoadAnimationsFromUnitsXML(node, this);

		i_offset.create(node.child("idle").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());
		m_offset.create(node.child("move").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());
		a_offset.create(node.child("attack").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());
		d_offset.create(node.child("death").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());
		de_offset.create(node.child("decompose").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());

		current_animation = &i_south;
		offset = i_offset;
		direction = { 0, 1 };
		App->entity->unit_game_objects_list.push_back(game_object);
	}
	else LOG("\nERROR, no node found\n");
	
	return ret;
}

bool Hero::Start()
{
	bool ret = true;

	return ret;
}

bool Hero::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Hero::CleanUp()
{
	bool ret = true;

	for (std::list<GameObject*>::iterator it = App->entity->unit_game_objects_list.begin(); it != App->entity->unit_game_objects_list.end(); it++) {
		if (*it == game_object)
		{	
			App->entity->unit_game_objects_list.erase(it);
			RELEASE(*it);
		}
	}
	
	return ret;
}

void Hero::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}