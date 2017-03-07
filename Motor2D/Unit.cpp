#include "Log.h"
#include "Unit.h"
#include "j1Entity.h"
#include "Scene.h"
#include "GameObject.h"
#include "j1Map.h"
#include "Hero.h"
#include "Entity.h"
#include "Animation.h"

Unit::Unit()
{
}

Unit::~Unit()
{
}

bool Unit::LoadEntity()
{
	bool ret = true;
	
	return ret;
}

bool Unit::Start()
{
	bool ret = true;
	
	return ret;
}

bool Unit::PreUpdate()
{
	bool ret = true;
	
	return ret;
}

bool Unit::Update(float dt)
{
	switch (state) {
	case unit_idle:
		CheckDirection();
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
	
	return ret;
}

bool Unit::PostUpdate()
{
	bool ret = true;
	
	return ret;
}

bool Unit::CleanUp()
{
	RELEASE(game_object);

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

void Unit::CheckDirection()
{
	if (direction.x == 1)
	{
		if (direction.y == 0)
		{
			current_animation = &i_west;
			flip = true;
		}
		else if (direction.y == 0.5)
		{
			current_animation = &i_south_west;
			flip = true;
		}
		else if (direction.y == -0.5)
		{
			current_animation = &i_north_west;
			flip = true;
		}
	}
	else if (direction.x == 0)
	{
		if (direction.y == 1)
		{
			current_animation = &i_south;
			flip = false;
		}
		else if (direction.y == -1)
		{
			current_animation = &i_north;
			flip = false;
		}
	}
	else if (direction.x == -1)
	{
		if (direction.y == 0)
		{
			current_animation = &i_west;
			flip = false;
		}
		else if (direction.y == 0.5)
		{
			current_animation = &i_south_west;
			flip = false;
		}
		else if (direction.y == -0.5)
		{
			current_animation = &i_north_west;
			flip = false;
		}
	}
}

void Unit::FollowPath(float dt)
{
	if (1)
	{
		SetDirection();

		fPoint pos = game_object->fGetPos();

		pos.x += direction.x * speed;
		pos.y += direction.y * speed;

		game_object->SetPos(pos);

		if (path.size() != 0)
		{
			if (App->map->WorldToMapPoint(game_object->GetPos()) == path.front()) {
				path.erase(path.begin());
			}
		}
		else
		{
			state = unit_idle;
			has_destination = false;
		}
	}
	/*else
	{
		if (path.size() > 0)
		{
			path.erase(path.begin());
			SetDirection();
		}
		else
		{
			has_destination = false;
			state = unit_idle;
		}
	}*/
}

void Unit::SetDirection()
{
	if (path.size() == 0){
		return;
	}
	iPoint position = game_object->GetPos();
	iPoint position_m = App->map->WorldToMap(position.x, position.y);

	if (path.size() != 0 && position_m == path.front())
	{
		path.erase(path.begin());
		SetDirection();
		return;
	}

	iPoint destination_w(App->map->MapToWorld(path.front().x, path.front().y));

	direction = fPoint(path.front().x - position_m.x, path.front().y - position_m.y);
	
	/*if (direction.x > 0) direction.x = 1;
	else if (direction.x < 0) direction.x = -1;
	if (direction.y > 0) direction.y = 1;
	else if (direction.y < 0) direction.y = -1;*/

	if (direction.x > 0) {
		if (direction.y > 0)
		{
			direction = { 0,1 };
			current_animation = &m_south;
			flip = false;
		}

		else if (direction.y < 0)
		{
			direction = { 1,0 };
			current_animation = &m_west;
			flip = true;
		}
		else
		{
			direction = { +1,+0.5 };
			current_animation = &m_south_west;
			flip = true;
		}

	}
	else if (direction.x < 0){
		if (direction.y > 0)
		{
			direction = { -1,0 };
			current_animation = &m_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			direction = { 0,-1 };
			current_animation = &m_north;
			flip = false;
		}
			
		else
		{
			direction = { -1,-0.5 };
			current_animation = &m_north_west;
			flip = false;
		}
	}
	else {
		if (direction.y > 0)
		{
			direction = { -1,0.5 };
			current_animation = &m_south_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			direction = { 1,-0.5 };
			current_animation = &m_north_west;
			flip = true;
		}
			
	}

	has_destination = true;
}
