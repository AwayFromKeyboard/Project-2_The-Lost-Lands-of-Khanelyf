#include "Unit.h"
#include "j1Entity.h"
#include "Scene.h"
#include "GameObject.h"
#include "j1Map.h"

Unit::Unit()
{
}

Unit::~Unit()
{
}

bool Unit::LoadEntity()
{
	return true;
}

bool Unit::Start()
{
	return true;
}

bool Unit::PreUpdate()
{
	return true;
}

bool Unit::Update(float dt)
{
	return true;
}

bool Unit::Draw(float dt)
{
	
	return true;
}

bool Unit::PostUpdate()
{
	return true;
}

bool Unit::CleanUp()
{
	for (std::vector<iPoint>::iterator it = path.begin(); it != path.end(); it++) {
		path.erase(it);
	}
	path.clear();
	for (std::list<GameObject*>::iterator it = App->entity->unit_game_objects_list.begin(); it != App->entity->unit_game_objects_list.end(); it++) {
		RELEASE(*it);
	}
	App->entity->unit_game_objects_list.clear();

	return true;
}

bool Unit::Load(pugi::xml_node &)
{
	return true;
}

bool Unit::Save(pugi::xml_node &) const
{
	return true;
}

void Unit::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

void Unit::SetPath(vector<iPoint> _path)
{
	path = _path;
}

vector<iPoint> Unit::GetPath()const
{
	return path;
}