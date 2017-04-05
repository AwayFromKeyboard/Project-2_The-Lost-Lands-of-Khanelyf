#include "Building.h"
#include "GameObject.h"
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Scene.h"
#include "Barracks.h"
#include "Functions.h"

Building::Building()
{
}

Building::~Building()
{
}

bool Building::LoadEntity()
{
	bool ret = true;

	return ret;
}

bool Building::Start()
{
	bool ret = true;

	return ret;
}

bool Building::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Building::Update(float dt)
{
	position = { game_object->GetPos().x, game_object->GetPos().y };
	collision->SetPos(position.x + collision->offset_x, position.y + collision->offset_y);

	return true;
}

bool Building::Draw(float dt)
{
	bool ret = true;

	App->scene->LayerBlit(6, game_object->GetTexture(), { game_object->GetPos().x - offset.x, game_object->GetPos().y - offset.y }, tex_rect);

	return ret;
}

bool Building::PostUpdate()
{
	bool ret = true;

	if (GetSelected())
		App->render->DrawCircle(game_object->GetPos().x + App->render->camera.x, game_object->GetPos().y + App->render->camera.y, 2, 255, 255, 255);

	if (to_delete)
	{
		App->entity->DeleteEntity(this);
	}

	return ret;
}

bool Building::CleanUp()
{
	for (std::list<GameObject*>::iterator it = App->entity->building_game_objects_list.begin(); it != App->entity->building_game_objects_list.end(); it++)
		RELEASE(*it);
	App->entity->building_game_objects_list.clear();

	return true;
}

bool Building::Load(pugi::xml_node &)
{
	return true;
}

bool Building::Save(pugi::xml_node &) const
{
	return true;
}

void Building::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

GameObject * Building::GetGameObject()
{
	return game_object;
}

Collider * Building::GetCollider()
{
	return collision;
}

entity_type Building::GetType()
{
	return type;
}
