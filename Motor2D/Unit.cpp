#include "Log.h"
#include "Unit.h"
#include "Building.h"
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
		break;
	case unit_move:
		FollowPath(dt);
		break;
	case unit_attack:
		if (attacked_unit != nullptr && IsInRange(attacked_unit))
		{
			att_state = attack_unit;
			offset = a_offset;
		}
		else if (!IsInRange(attacked_unit))
		{
			state = unit_idle;
			current_animation = &i_north;
			att_state = attack_null;
			offset = i_offset;
			attacked_unit = nullptr;
			break;
		}
		/*if (attacked_building != nullptr && IsInRange(attacked_building)) att_state = attack_building;
		else if (!IsInRange(attacked_unit))
		*/
		switch (att_state) {
		case attack_unit:
			UnitAttack();
			break;
		case attack_building:
			BuildingAttack();
			break;
		}

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

bool Unit::IsInRange(Entity* attacked_entity)
{
	bool ret = true;

	iPoint attacked_pos = attacked_entity->GetGameObject()->GetPos();
	iPoint pos = game_object->GetPos();
	attacked_pos = App->map->WorldToMapPoint(attacked_pos);
	pos = App->map->WorldToMapPoint(pos);

	direction.x = attacked_pos.x - pos.x;
	direction.y = attacked_pos.y - pos.y;

	if (std::abs(direction.x) > range || std::abs(direction.y) > range) ret = false;

	return ret;
}

void Unit::LookAtAttack()
{
	if (direction.x > 0)
	{
		if (direction.y > 0)
		{
			current_animation = &a_south;
			flip = false;
		}

		else if (direction.y < 0)
		{
			current_animation = &a_west;
			flip = true;
		}
		else
		{
			current_animation = &a_south_west;
			flip = true;
		}

	}
	else if (direction.x < 0)
	{
		if (direction.y > 0)
		{
			current_animation = &a_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			current_animation = &a_north;
			flip = false;
		}

		else
		{
			current_animation = &a_north_west;
			flip = false;
		}
	}
	else
	{
		if (direction.y > 0)
		{
			current_animation = &a_south_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			current_animation = &a_north_west;
			flip = true;
		}
	}
}

void Unit::UnitAttack()
{
	LookAtAttack();

}

void Unit::BuildingAttack()
{
	LookAtAttack();
}

void Unit::SetAttackingUnit(Unit * att_unit)
{
	attacked_unit = att_unit;
}

void Unit::SetAttackingBuilding(Building * att_building)
{
	attacked_building = att_building;
}
