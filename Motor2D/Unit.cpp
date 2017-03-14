#include "Log.h"
#include "Unit.h"
#include "j1Entity.h"
#include "Scene.h"
#include "GameObject.h"
#include "j1Map.h"
#include "Hero.h"
#include "Entity.h"
#include "Animation.h"
#include "j1Collisions.h"

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
	collision_box->rect = { game_object->GetPos().x - offset.x, game_object->GetPos().y - offset.y, current_animation->GetAnimationFrame(dt).w, current_animation->GetAnimationFrame(dt).h };

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

void Unit::OnColl(Collider* col1, Collider* col2)
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

		pos.x += direction.x * speed;
		pos.y += direction.y * speed;

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
