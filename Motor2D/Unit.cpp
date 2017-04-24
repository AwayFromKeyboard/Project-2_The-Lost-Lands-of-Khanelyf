#include "Log.h"
#include "Unit.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "Scene.h"
#include "GameObject.h"
#include "j1Map.h"
#include "Hero.h"
#include "Barbarian.h"
#include "Swordsman.h"
#include "Entity.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "j1Audio.h"
#include "Functions.h"
#include "QuestManager.h"

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
	AI_timer.Start();
	return ret;
}

bool Unit::PreUpdate()
{
	bool ret = true;

	if (attacked_unit == nullptr && life > 0 && state != entity_state::entity_move_to_enemy) 
	{
		if (path.size() > 0)
		{
			state = entity_move;
		}
		else
		{
			state = entity_idle;
		}
	}

	position_map = App->map->WorldToMapPoint(position);
	if (life > 0) {
		App->map->entity_matrix[position_map.x][position_map.y] = this;
	}
	else if (selected) {
		App->entity->selected.remove(this);
		selected = false;
	}

	return ret;
}

bool Unit::Update(float dt)
{
	collision->SetPos(position.x + collision->offset_x, position.y + collision->offset_y);
	
	switch (state) {
	case entity_state::entity_idle:
		CheckDirection();
		CheckSurroundings();
		break;

	case entity_state::entity_move:
		FollowPath(dt);
		//CheckSurroundings();
		break;

	case entity_state::entity_move_to_enemy:
	{
		if (attacked_unit == nullptr || attacked_unit->life <= 0)
			state = entity_idle;
		else {
			if (IsInRange(attacked_unit)) {
				App->pathfinding->DeletePath(path_id);
				path.clear();
				state = entity_state::entity_attack;
				has_moved = false;
			}
			else if (!has_moved) {
				has_moved = true;
				App->pathfinding->DeletePath(path_id);
				path.clear();
				path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint(position), App->map->WorldToMapPoint(attacked_unit->position));
			}
			else{
				if (path.size() > 0)
					FollowPath(dt);
			}
		}
	}
		break;

	case entity_state::entity_attack:
		if (attacked_unit == nullptr || attacked_unit->life <= 0) {
			attacked_unit == nullptr;
			state = entity_idle;
		}
		else
		{
			if (IsInRange(attacked_unit)) {
				att_state = attack_unit;
			}
			else {
				state = entity_state::entity_move_to_enemy;
				current_animation = &i_north;
				att_state = attack_null;
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

	case entity_state::entity_death:
		CheckDeathDirection();
		if(collision != nullptr)
			App->collisions->EraseCollider(collision);
		if (!current_animation->Finished())
			death_timer.Start();
		else if (death_timer.ReadSec() > 2)
		{
			state = entity_state::entity_decompose;
			if (type == entity_type::enemy) {
				App->scene->scene_test->IncreaseGold(gold_drop);
				if (App->questmanager->GetCurrentQuest()->type == quest_type::kill)
					App->questmanager->GetCurrentQuest()->progress++;
			}
		}
		break;

	case entity_state::entity_decompose:
		CheckDecomposeDirection();
			if (current_animation->Finished()) {
				to_delete = true;
		}
		break;
	}

	return true;
}

bool Unit::Draw(float dt)
{
	bool ret = true;


	switch (state)
	{
	case entity_idle:
		offset = i_offset;
		if(flip)
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x - flip_i_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_move:
		offset = m_offset;
		if(flip)
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x - flip_m_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_move_to_enemy:
		offset = m_offset;
		if (flip)
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x - flip_m_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_attack:
		offset = a_offset;
		if (flip)
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x - flip_a_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_death:
		offset = d_offset;
		if (flip)
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x - flip_d_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_decompose:
		offset = de_offset;
		if (flip)
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x - flip_de_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(5, game_object->GetTexture(), { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	}

	return ret;
}

bool Unit::PostUpdate()
{
	bool ret = true;

	App->map->entity_matrix[position_map.x][position_map.y] = nullptr;

	if (GetSelected())
		App->render->DrawCircle(position.x + App->render->camera.x, position.y + App->render->camera.y, 2, 255, 255, 255);

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
	return collision;
}

entity_type Unit::GetType()
{
	return type;
}

void Unit::SetSelected(bool _selected) {
	if (life > 0)
		selected = _selected;
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

	fPoint pos;
	pos.create(position.x, position.y);

	pos.x += direction.x * speed;
	pos.y += direction.y * speed;

	position.x = pos.x;
	position.y = pos.y;

	if (path.size() == 0)
	{
		state = entity_idle;
		has_destination = false;
	}
}

void Unit::SetDirection()
{
	if (path.size() == 0){
		return;
	}

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

	if (position_map == path.front()) {
			path.erase(path.begin());
			SetDirection();
			return;
	}

	iPoint destination_w(App->map->MapToWorld(path.front().x, path.front().y));

	direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);

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

bool Unit::CheckSurroundings() {
	if (AI_timer.ReadSec() >= 0.5) {
		AI_timer.Start();
		std::list<iPoint> frontier;
		std::list<iPoint> visited;


		visited.push_back(App->map->WorldToMapPoint(position));
		frontier.push_back(App->map->WorldToMapPoint(position));

		for (int i = 0; i < radius_of_action; ++i) {
			for (int j = frontier.size(); j > 0; j--) {
				iPoint neighbors[4];
				neighbors[0] = frontier.front() + iPoint(1, 0);
				neighbors[1] = frontier.front() + iPoint(-1, 0);
				neighbors[2] = frontier.front() + iPoint(0, 1);
				neighbors[3] = frontier.front() + iPoint(0, -1);
				frontier.pop_front();

				for (int k = 0; k < 4; k++) {
					Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
					if (found != nullptr && found->life > 0) {
						switch (type) {
						case player:
						case ally:
							if (found->type == enemy) {
								attacked_unit = found;
								state = entity_move_to_enemy;
								return true;
							}
							break;
						case enemy:
							if (found->type == player || found->type == ally) {
								attacked_unit = found;
								state = entity_move_to_enemy;
								return true;
							}
						}
					}
					else {
						if (App->pathfinding->IsWalkable(neighbors[k])) {
							bool is_visited = false;
							for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
								if (neighbors[k] == *it) {
									is_visited = true;
									break;
								}
							}
							if (!is_visited) {
								frontier.push_back(neighbors[k]);
								visited.push_back(neighbors[k]);
							}
						}
					}
				}
			}
		}
	}
}

bool Unit::IsInRange(Entity* attacked_entity)
{
	bool ret = true;

	if (attacked_entity == nullptr) return false;

	iPoint attacked_pos = attacked_entity->position;
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

	if (current_animation->GetFrameIndex() == 5 && shout_fx == true) {
		App->audio->PlayFx(RandomGenerate(App->scene->scene_test->get_hit_id, App->scene->scene_test->get_hit4_id));
		App->audio->PlayFx(RandomGenerate(App->scene->scene_test->swords_clash_id, App->scene->scene_test->swords_clash4_id));
		shout_fx = false;
	}

	if (current_animation->Finished())
	{
		attacked_unit->life -= damage;
		current_animation->Reset();
		if (attacked_unit->life <= 0)
		{
			App->audio->PlayFx(RandomGenerate(App->scene->scene_test->death_id, App->scene->scene_test->death2_id));
			state = entity_idle;
			attacked_unit->state = entity_death;
			attacked_unit = nullptr;
		}
		shout_fx = true;
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

bool Unit::IsInsideCircle(int x, int y)
{
	iPoint center = game_object->GetPos();
	return (x - center.x) ^ 2 + (y - center.y) ^ 2 <= radius_of_action*radius_of_action;
}

