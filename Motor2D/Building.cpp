#include "Building.h"
#include "j1Entity.h"
#include "Scene.h"

Building::Building()
{
}

Building::~Building()
{
}

bool Building::LoadEntity()
{
	return true;
}

bool Building::Start()
{
	return true;
}

bool Building::PreUpdate()
{
	return true;
}

bool Building::Update(float dt)
{
	return true;
}

bool Building::Draw(float dt)
{
	return true;
}

bool Building::PostUpdate()
{
	return true;
}

bool Building::CleanUp()
{
	for (std::list<GameObject*>::iterator it = App->entity->building_game_objects_list.begin(); it != App->entity->building_game_objects_list.end(); it++) {
		RELEASE(*it);
	}
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
