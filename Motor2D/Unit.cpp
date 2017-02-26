#include "Log.h"
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

void Unit::FollowPath(float dt) 
{
	if (has_destination)
	{
		SetDirection();

		iPoint position = game_object->GetPos();
		fPoint fposition; fposition.x = position.x, fposition.y = position.y;

		fposition.x += (direction.x * 100) * dt;
		fposition.y += (direction.y * 100) * dt;

		fposition.x = roundf(fposition.x);
		fposition.y = roundf(fposition.y);

		position.x = fposition.x;
		position.y = fposition.y;

		game_object->SetPos(fPoint(position.x, position.y));

		iPoint destination_w = App->map->MapToWorld(destination.x, destination.y);

		if (position.DistanceNoSqrt(destination_w) <= 8)
		{
			if (path.size() > 0)
			{
				destination = path.front();
				path.erase(path.begin());
				SetDirection();
			}
			else
				has_destination = false;
				end_movement = true;
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
			has_destination = false;
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

	iPoint destination_w = App->map->MapToWorld(destination.x, destination.y);

	direction.x = destination_w.x - position.x;
	direction.y = destination_w.y - position.y;
	
	direction.Normalize();

	has_destination = true;
}
