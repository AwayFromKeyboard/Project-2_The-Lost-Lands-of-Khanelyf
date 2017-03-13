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
		CheckDirection();
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
	SetDirection();

	fPoint pos = game_object->fGetPos();

	pos.x += direction.x * speed;
	pos.y += direction.y * speed;

	game_object->SetPos(pos);

	if (path.size() == 0)
	{
		state = unit_idle;
		has_destination = false;
	}
}

void Unit::SetDirection()
{
	if (path.size() == 0){
		return;
	}

	iPoint position = game_object->GetPos();
	switch (destination) {
	case south:
		break;
	case north:
		position.y += offset.y;
		break;
	case north_east:
		position.y += offset.y;
	case south_east:
	case east:
		position.x -= offset.x;
		break;
	case north_west:
		position.y += offset.y;
	case south_west:
	case west:
		position.x += offset.x;
		break;
	}

	iPoint position_m = App->map->WorldToMapPoint(position);

	if (position_m == path.front()) {
			path.erase(path.begin());
			SetDirection();
			return;
	}

	iPoint destination_w(App->map->MapToWorld(path.front().x, path.front().y));

	direction = fPoint(path.front().x - position_m.x, path.front().y - position_m.y);

	LookAtMovement();

	has_destination = true;
}

void Unit::LookAtMovement()
{
	if (direction.x > 0) 
	{
		if (direction.y > 0)
		{
			direction = { 0,1 };
			current_animation = &m_south;
			destination = south;
			flip = false;
		}

		else if (direction.y < 0)
		{
			direction = { 1,0 };
			current_animation = &m_west;
			destination = east;
			flip = true;
		}
		else
		{
			direction = { +1,+0.5 };
			current_animation = &m_south_west;
			destination = south_east;
			flip = true;
		}

	}
	else if (direction.x < 0) 
	{
		if (direction.y > 0)
		{
			direction = { -1,0 };
			current_animation = &m_west;
			destination = west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			direction = { 0,-1 };
			current_animation = &m_north;
			destination = north;
			flip = false;
		}

		else
		{
			direction = { -1,-0.5 };
			current_animation = &m_north_west;
			destination = north_west;
			flip = false;
		}
	}
	else 
	{
		if (direction.y > 0)
		{
			direction = { -1,0.5 };
			current_animation = &m_south_west;
			destination = south_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			direction = { 1,-0.5 };
			current_animation = &m_north_west;
			destination = north_east;
			flip = true;
		}

	}
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

