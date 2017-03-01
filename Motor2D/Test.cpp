#include "Test.h"
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

Test::Test()
{
}

Test::~Test()
{
}

bool Test::LoadEntity()
{
	bool ret = true;

	game_object = new GameObject(iPoint(150, 150), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	game_object->CreateCollision(COLLISION_ADJUSTMENT, 20, 54, fixture_type::f_t_null);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

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
		cost = node.child("cost").attribute("value").as_int();
		speed = node.child("speed").attribute("value").as_float();
		damage = node.child("damage").attribute("value").as_int();
		armor = node.child("armor").attribute("value").as_int();
		pierce_armor = node.child("pierce_armor").attribute("value").as_int();
		range = node.child("range").attribute("value").as_int();
		life = node.child("life").attribute("value").as_int();

		std::string texture = node.child("texture").attribute("value").as_string();
		game_object->SetTexture(App->tex->LoadTexture(texture.c_str()));
		game_object->LoadAnimationsFromUnitsXML(node);
	}
	else LOG("\nERROR, no node found\n");
	

	return ret;
}

bool Test::Start()
{
	bool ret = true;

	return ret;
}

bool Test::Draw(float dt)
{
	bool ret = true;

	if (flip) {
		App->scene->LayerBlit(5, game_object->GetTexture(), { game_object->GetPos().x - X_OFFSET, game_object->GetPos().y - Y_OFFSET }, game_object->GetCurrentAnimationRect(dt), -1.0, SDL_FLIP_HORIZONTAL);
	}
	else
		App->scene->LayerBlit(5, game_object->GetTexture(), { game_object->GetPos().x - X_OFFSET, game_object->GetPos().y - Y_OFFSET }, game_object->GetCurrentAnimationRect(dt));

	return ret;
}

bool Test::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Test::CleanUp()
{
	bool ret = true;

	for (std::list<GameObject*>::iterator it = App->entity->unit_game_objects_list.begin(); it != App->entity->unit_game_objects_list.end(); it++) {
		if ((*it) == game_object) {
			App->entity->unit_game_objects_list.erase(it++);
		}
	}

	return ret;
}

void Test::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}