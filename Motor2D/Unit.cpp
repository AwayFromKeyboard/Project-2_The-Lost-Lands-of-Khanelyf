#include "Log.h"
#include "Unit.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "Scene.h"
#include "GameObject.h"
#include "j1Map.h"
#include "Hero.h"
#include "Entity.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "j1Scene.h"

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
	
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_down && GetSelected()) {
		path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint(game_object->GetPos()), p);
	}

	if (path.size() > 0)
	{
		state = unit_move;
	}
	else
	{
		state = unit_idle;
	}

	return ret;
}

bool Unit::Update(float dt)
{
	position = { game_object->GetPos().x - 7, game_object->GetPos().y };
	switch (state) {
	case unit_idle:

		idle_collision->print_collider = true;
		walk_collision->print_collider = false;
		attack_collision->print_collider = false;

		idle_collision->SetPos(position.x, position.y);
		offset = i_offset;
		CheckDirection();
		break;

	case unit_move:
		
		FollowPath(dt);
		idle_collision->print_collider = false;
		walk_collision->print_collider = true;
		attack_collision->print_collider = false;
		
		walk_collision->SetPos(position.x, position.y);
		break;

	case unit_attack:
		if (attacked_unit != nullptr)
		{
			if (IsInRange(attacked_unit))
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
		}
		
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
		CheckDeathDirection();
		if (current_animation->GetFrameIndex() == 14)
		{
			death_timer.Start();
			current_animation->SetSpeed(0);
			state = unit_decompose;

			App->collisions->EraseCollider(idle_collision);
			App->collisions->EraseCollider(walk_collision);
			App->collisions->EraseCollider(attack_collision);
		}
		break;
	case unit_decompose:
		if (death_timer.ReadSec() > 2)
		{
			offset = de_offset;
			CheckDecomposeDirection();
			if (current_animation->GetFrameIndex() == 4) {
				current_animation->SetSpeed(0);
				to_delete = true;
			}
		}
		break;
	}

	return true;
}

bool Unit::Draw(float dt)
{
	bool ret = true;

	if (flip) {
		App->scene->LayerBlit(5, game_object->GetTexture(), { game_object->GetPos().x - offset.x, game_object->GetPos().y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
	}
	else
		App->scene->LayerBlit(5, game_object->GetTexture(), { game_object->GetPos().x - offset.x, game_object->GetPos().y - offset.y }, current_animation->GetAnimationFrame(dt));

	return ret;
}

bool Unit::PostUpdate()
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

bool Unit::CleanUp()
{
	bool ret = true;

	App->entity->unit_game_objects_list.remove(game_object);
	RELEASE(game_object);

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

void Unit::OnColl(Collider* col1, Collider* col2)
{
	if (col1 != nullptr && (col2->type == COLLIDER_UNIT))
	{
		
	}
}

GameObject * Unit::GetGameObject()
{
	return game_object;
}

Collider * Unit::GetCollider()
{
	return idle_collision;
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

	if (attacked_entity == nullptr) return false;

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
			direction = { 0,1 };
			current_animation = &a_south;
			flip = false;
		}

		else if (direction.y < 0)
		{
			direction = { 1,0 };
			current_animation = &a_west;
			flip = true;
		}
		else
		{
			direction = { +1,+0.5 };
			current_animation = &a_south_west;
			flip = true;
		}

	}
	else if (direction.x < 0)
	{
		if (direction.y > 0)
		{
			direction = { -1,0 };
			current_animation = &a_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			direction = { 0,-1 };
			current_animation = &a_north;
			flip = false;
		}

		else
		{
			direction = { -1,-0.5 };
			current_animation = &a_north_west;
			flip = false;
		}
	}
	else
	{
		if (direction.y > 0)
		{
			direction = { -1,0.5 };
			current_animation = &a_south_west;
			flip = false;
		}
		else if (direction.y < 0)
		{
			direction = { 1,-0.5 };
			current_animation = &a_north_west;
			flip = true;
		}
	}
}

void Unit::UnitAttack()
{
	LookAtAttack();

	if (current_animation->Finished())
	{
		attacked_unit->life -= damage;
		current_animation->Reset();
		if (attacked_unit->life <= 0)
		{
			state = unit_idle;
			attacked_unit->state = unit_death;
			attacked_unit->offset = attacked_unit->d_offset;
		}
	}
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

void Unit::CheckDeathDirection()
{
	if (direction.x == 1)
	{
		if (direction.y == 0)
		{
			current_animation = &d_west;
			flip = true;
		}
		else if (direction.y == 0.5)
		{
			current_animation = &d_south_west;
			flip = true;
		}
		else if (direction.y == -0.5)
		{
			current_animation = &d_north_west;
			flip = true;
		}
	}
	else if (direction.x == 0)
	{
		if (direction.y == 1)
		{
			current_animation = &d_south;
			flip = false;
		}
		else if (direction.y == -1)
		{
			current_animation = &d_north;
			flip = false;
		}
	}
	else if (direction.x == -1)
	{
		if (direction.y == 0)
		{
			current_animation = &d_west;
			flip = false;
		}
		else if (direction.y == 0.5)
		{
			current_animation = &d_south_west;
			flip = false;
		}
		else if (direction.y == -0.5)
		{
			current_animation = &d_north_west;
			flip = false;
		}
	}
}

void Unit::CheckDecomposeDirection()
{
	if (direction.x == 1)
	{
		if (direction.y == 0)
		{
			current_animation = &de_west;
			flip = true;
		}
		else if (direction.y == 0.5)
		{
			current_animation = &de_south_west;
			flip = true;
		}
		else if (direction.y == -0.5)
		{
			current_animation = &de_north_west;
			flip = true;
		}
	}
	else if (direction.x == 0)
	{
		if (direction.y == 1)
		{
			current_animation = &de_south;
			flip = false;
		}
		else if (direction.y == -1)
		{
			current_animation = &de_north;
			flip = false;
		}
	}
	else if (direction.x == -1)
	{
		if (direction.y == 0)
		{
			current_animation = &de_west;
			flip = false;
		}
		else if (direction.y == 0.5)
		{
			current_animation = &de_south_west;
			flip = false;
		}
		else if (direction.y == -0.5)
		{
			current_animation = &de_north_west;
			flip = false;
		}
	}
}

