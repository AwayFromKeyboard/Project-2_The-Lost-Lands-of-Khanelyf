#include "Log.h"
#include "Unit.h"
#include "j1Entity.h"
#include "Scene.h"
#include "GameObject.h"
#include "j1Map.h"
#include "Test.h"
#include "Entity.h"

Unit::Unit()
{
}

Unit::Unit(unit_type type)
{
	CreateUnit(type);
}

Unit::~Unit()
{
}

bool Unit::LoadEntity()
{
	bool ret = true;
	for (std::list<Entity*>::iterator it = unit_list.begin(); it != unit_list.end(); it++) {
		ret = (*it)->LoadEntity();
		game_object = (*it)->GetGameObject();
	}
	return ret;
}

bool Unit::Start()
{
	bool ret = true;
	for (std::list<Entity*>::iterator it = unit_list.begin(); it != unit_list.end(); it++) {
		ret = (*it)->Start();
	}
	return ret;
}

bool Unit::PreUpdate()
{
	bool ret = true;
	for (std::list<Entity*>::iterator it = unit_list.begin(); it != unit_list.end(); it++) {
		ret = (*it)->PreUpdate();
	}
	return ret;
}

bool Unit::Update(float dt)
{
	switch (state) {
	case unit_idle:
		break;
	case unit_move:
		FollowPath(dt);
		break;
	case unit_attack:
		break;
	case unit_death:
		break;
	case unit_decompose:
		break;
	}
	return true;
}

bool Unit::Draw(float dt)
{
	bool ret = true;
	for (std::list<Entity*>::iterator it = unit_list.begin(); it != unit_list.end(); it++) {
		ret = (*it)->Draw(dt);
	}
	return ret;
}

bool Unit::PostUpdate()
{
	bool ret = true;
	for (std::list<Entity*>::iterator it = unit_list.begin(); it != unit_list.end(); it++) {
		ret = (*it)->PostUpdate();
	}
	return ret;
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
	for (std::list<Entity*>::iterator it = unit_list.begin(); it != unit_list.end(); it++) {
		(*it)->CleanUp();
		RELEASE(*it);
	}
	unit_list.clear();
	App->entity->unit_game_objects_list.clear();

	return true;
}

Entity * Unit::CreateUnit(unit_type _type)
{
	Entity* ret = nullptr;
	switch (_type)
	{
	case unit_test:
		ret = new Test();
		type = _type;
		break;
	}

	if (ret != nullptr)
	{
		ret->Start();
		unit_list.push_back(ret);
	}
	else
		LOG("Unit creation returned nullptr");

	return ret;
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

GameObject * Unit::GetGameObject()
{
	return game_object;
}

void Unit::SetPath(vector<iPoint> _path)
{
	path = _path;
}

vector<iPoint> Unit::GetPath()const
{
	return path;
}

void Unit::FollowPath(float dt)
{
	if (has_destination)
	{
		SetDirection();

		fPoint pos = game_object->fGetPos();

		pos.x += direction.x * game_object->GetSpeed();
		pos.y += direction.y * game_object->GetSpeed();

		game_object->SetPos(pos);

		if (App->map->WorldToMapPoint(game_object->GetPos()) == destination) {
			if (path.size() > 0)
			{
				destination = path.front();
				path.erase(path.begin());
				SetDirection();
			}
			else
			{
				state = unit_idle;
				has_destination = false;
			}
		}
	}
	else
	{
		if (path.size() > 0)
		{
			destination = path.front();
			path.erase(path.begin());
			SetDirection();
		}
		else
		{
			has_destination = false;
			state = unit_idle;
		}
	}
}

void Unit::SetDirection()
{
	iPoint position = game_object->GetPos();
	iPoint position_m = App->map->WorldToMap(position.x, position.y);

	if (position_m == destination)
	{
		if (path.size() > 0)
		{
			destination = path.front();
			path.erase(path.begin());
			SetDirection();
		}
		return;
	}

	iPoint destination_w(App->map->MapToWorld(destination.x, destination.y));

	direction = fPoint(destination_w.x - position.x, destination_w.y - position.y);
	
	if (direction.x > 0) direction.x = 1;
	else if (direction.x < 0) direction.x = -1;
	if (direction.y > 0) direction.y = 1;
	else if (direction.y < 0) direction.y = -1;

	has_destination = true;
}
