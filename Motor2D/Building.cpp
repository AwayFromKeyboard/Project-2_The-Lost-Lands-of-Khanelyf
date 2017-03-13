#include "Building.h"
#include "j1Entity.h"
#include "Scene.h"

Bulding::Bulding()
{
}

Bulding::~Bulding()
{
}

bool Bulding::LoadEntity()
{
	return true;
}

bool Bulding::Start()
{
	return true;
}

bool Bulding::PreUpdate()
{
	return true;
}

bool Bulding::Update(float dt)
{
	return true;
}

bool Bulding::Draw(float dt)
{
	return true;
}

bool Bulding::PostUpdate()
{
	return true;
}

bool Bulding::CleanUp()
{
	for (std::list<GameObject*>::iterator it = App->entity->building_game_objects_list.begin(); it != App->entity->building_game_objects_list.end(); it++) {
		RELEASE(*it);
	}
	App->entity->building_game_objects_list.clear();

	return true;
}

bool Bulding::Load(pugi::xml_node &)
{
	return true;
}

bool Bulding::Save(pugi::xml_node &) const
{
	return true;
}

void Bulding::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
