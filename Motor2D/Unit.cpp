#include "Log.h"
#include "Unit.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "Scene.h"
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
#include "Object.h"
#include "Player.h"
#include "Building.h"

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
	life_up_timer.Start();
	max_life = life;
	
	layer = 5;

	return ret;
}

bool Unit::PreUpdate()
{
	bool ret = true;

	if (App->player->pause_status == false) {
		if ((attacked_unit == nullptr && attacked_building == nullptr) && life > 0 && (state != entity_state::entity_move_to_enemy && state != entity_state::entity_move_to_building && state != entity_state::entity_pick_object))
		{
			if (path.size() > 0)
			{
				state = entity_state::entity_move;
			}
			else
			{
				state = entity_state::entity_idle;
			}
		}

		if (state != entity_state::entity_death && state != entity_state::entity_decompose)
			LifeBar({ 50, 5 }, { -20, -35 });

		aux_pos = position;
		position_map = App->map->WorldToMapPoint(aux_pos);
		if (life > 0 && position_map.x >= 0 && position_map.y >= 0 && (App->pathfinding->IsWalkable(position_map) && (type != entity_type::building || type != entity_type::ally_building || type != entity_type::enemy_building))) {
			App->map->entity_matrix[position_map.x][position_map.y] = this;
		}
		else if (selected) {
			App->entity->selected.remove(this);
			selected = false;
		}
	}

	return ret;
}

bool Unit::Update(float dt)
{
	if (App->player->pause_status == false) {

		if (collision != nullptr)
			collision->SetPos(aux_pos.x + collision->offset_x, aux_pos.y + collision->offset_y);

		switch (state) {
		case entity_state::entity_idle:

			if (life < max_life) {
				if (life_up_timer.ReadSec() >= 1) {
					life += 1;
					life_up_timer.Start();
				}
			}
			CheckDirection();
			CheckSurroundings();
			has_moved = false;
			break;

		case entity_state::entity_move:
			FollowPath(dt);
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
				else {
					if (path.size() > 0) {
						FollowPath(dt);

						if (type == entity_type::enemy) {
							if (App->map->WorldToMapPoint(position).DistanceTo(App->map->WorldToMapPoint(attacked_unit->position)) > radius_of_action * 3 / 2) {
								state = entity_idle;
								attacked_unit = nullptr;
							}
						}
					}
				}
			}
		}
		break;

		case entity_state::entity_move_to_building:
		{
			if (attacked_building == nullptr || attacked_building->life <= 0)
				state = entity_idle;
			else {
				if (IsInRange(attacked_building)) {
					App->pathfinding->DeletePath(path_id);
					path.clear();
					state = entity_state::entity_attack;
					has_moved = false;
				}
				else if (!has_moved) {
					has_moved = true;
					App->pathfinding->DeletePath(path_id);
					path.clear();
					path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint(position), App->map->WorldToMapPoint(attacked_building->position));
				}
				else {
					if (path.size() > 0) {
						FollowPath(dt);
					}
				}
			}
		}
		break;

		case entity_state::entity_attack:
			if ((attacked_unit == nullptr || attacked_unit->life <= 0) && (attacked_building == nullptr || attacked_building->life <= 0) || is_holding_object) {
				attacked_unit = nullptr;
				attacked_building = nullptr;
				state = entity_idle;
			}
			else if (attacked_building == nullptr)
			{
				if (IsInRange(attacked_unit)) {
					att_state = attack_unit;
				}
				else {
					state = entity_state::entity_move_to_enemy;
					att_state = attack_null;
					break;
				}
			}
			else
			{
				if (IsInRange(attacked_building)) {
					att_state = attack_building;
				}
				else {
					state = entity_state::entity_move_to_building;
					att_state = attack_null;
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
			if (collision != nullptr) {
				App->collisions->EraseCollider(collision);
				collision = nullptr;
			}
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


		case entity_state::entity_pick_object:
			if (IsInRange(to_pick_object)) {
				App->pathfinding->DeletePath(path_id);
				path.clear();
				state = entity_state::entity_idle;
				if (to_pick_object->pickable == true)
					PickObject();
				has_moved = false;
			}
			else if (!has_moved) {
				has_moved = true;
				App->pathfinding->DeletePath(path_id);
				path.clear();
				path_id = App->pathfinding->CreatePath(App->map->WorldToMapPoint(position), App->map->WorldToMapPoint(to_pick_object->position));
			}
			else {
				if (path.size() > 0)
					FollowPath(dt);
			}
		}

		if (damaged_by_whirlwind == true && timer_whirlwind_start == true)
		{
			whirlwind_damage.Start();
			timer_whirlwind_start = false;
		}

		if (whirlwind_damage.ReadSec() >= 4 && damaged_by_whirlwind == true)
		{
			damaged_by_whirlwind = false;
			timer_whirlwind_start = true;
		}

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

		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_i_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_move:
		offset = m_offset;

		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_m_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_move_to_enemy:
		offset = m_offset;
		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_m_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_move_to_building:
		offset = m_offset;
		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_m_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_attack:
		offset = a_offset;
		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_a_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_death:
		offset = d_offset;
		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_d_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_decompose:
		offset = de_offset;
		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_de_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	case entity_pick_object:
		offset = m_offset;
		if (flip)
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x - flip_m_offset, position.y - offset.y }, current_animation->GetAnimationFrame(dt), -1.0, SDL_FLIP_HORIZONTAL);
		else
			App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, current_animation->GetAnimationFrame(dt));
		break;
	}

	return ret;
}

bool Unit::PostUpdate()
{
	bool ret = true;

	if (position_map.x >= 0 && position_map.y >= 0 && (App->pathfinding->IsWalkable(position_map) && (type != entity_type::building || type != entity_type::ally_building || type != entity_type::enemy_building)))
		App->map->entity_matrix[position_map.x][position_map.y] = nullptr;

	if (GetSelected())
		App->render->DrawCircle(position.x + App->render->camera.x, position.y + App->render->camera.y, 2, 255, 255, 255);

	return ret;
}

bool Unit::CleanUp()
{
	bool ret = true;

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
	path.erase(path.begin());
	w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	LookAtMovement();
}

vector<iPoint> Unit::GetPath()const
{
	return path;
}

void Unit::CheckDirection()
{
	switch (destination) {
	case east:
		current_animation = &i_west;
		flip = true;
		break;

	case south_east:
		current_animation = &i_south_west;
		flip = true;
		break;
	
	case north_east:
		current_animation = &i_north_west;
		flip = true;
		break;
	
	case south:
		current_animation = &i_south;
		flip = false;
		break;

	case north:
		current_animation = &i_north;
		flip = false;
		break;

	case west:
		current_animation = &i_west;
		flip = false;
		break;

	case south_west:
		current_animation = &i_south_west;
		flip = false;
		break;

	case north_west:
		current_animation = &i_north_west;
		flip = false;
		break;
	}	
}

void Unit::FollowPath(float dt)
{

	SetDirection();

	fPoint pos = fPoint(position.x, position.y);

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

	if (App->debug_mode) {
		for (std::vector<iPoint>::iterator it = path.begin(); it != path.end(); ++it) 
			App->scene->LayerBlit(200, App->scene->scene_test->debug_tex, App->map->MapToWorldPoint(*it), { 0, 0, 64, 32 });
	}


	switch (destination) {
	case north_east:
		aux_pos.y += offset.y;
	case south_east:
	case east:
		if (position.x >= App->map->MapToWorldPoint(path.front()).x + App->map->data.tile_width / 2) {
			position = App->map->MapToWorldPoint(path.front()) + iPoint(App->map->data.tile_width / 2, App->map->data.tile_height / 2);
			path.erase(path.begin());
			checked_next_tile = false;
			w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
			SetDirection();
			LookAtMovement();
			return;
		}
		aux_pos.x -= offset.x;
		break;

	case north_west:
		aux_pos.y += offset.y;
	case south_west:
	case west:
		if (position.x <= App->map->MapToWorldPoint(path.front()).x + App->map->data.tile_width / 2) {
			position = App->map->MapToWorldPoint(path.front()) + iPoint(App->map->data.tile_width / 2, App->map->data.tile_height / 2);
			path.erase(path.begin());
			checked_next_tile = false;
			w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
			SetDirection();
			LookAtMovement();
			return;
		}
		aux_pos.x += offset.x;
		break;

	case north:
		if (position.y <= App->map->MapToWorldPoint(path.front()).y + App->map->data.tile_height / 2) {
			position = App->map->MapToWorldPoint(path.front()) + iPoint(App->map->data.tile_width / 2, App->map->data.tile_height / 2);
			path.erase(path.begin());
			checked_next_tile = false;
			w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
			SetDirection();
			LookAtMovement();
			return;
		}
		aux_pos.y += offset.y;
		break;

	case south:
		if (position.y >= App->map->MapToWorldPoint(path.front()).y + App->map->data.tile_height / 2) {
			position = App->map->MapToWorldPoint(path.front()) + iPoint(App->map->data.tile_width / 2, App->map->data.tile_height / 2);
			path.erase(path.begin());
			checked_next_tile = false;
			w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
			SetDirection();
			LookAtMovement();
			return;
		}
		break;
	}

	//switch (destination) {
	//case south:
	//	if (position.y >= App->map->MapToWorldPoint(path.front()).y) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	break;
	//case north:
	//	if (position.y <= App->map->MapToWorldPoint(path.front()).y) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.y += offset.y;
	//	break;
	//case north_east:
	//	if (position.y <= App->map->MapToWorldPoint(path.front()).y) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.y += offset.y;
	//	aux_pos.x -= offset.x;
	//	break;
	//case south_east:
	//	if (position.y >= App->map->MapToWorldPoint(path.front()).y) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.x -= offset.x;
	//	break;
	//case east:
	//	if (position.x >= App->map->MapToWorldPoint(path.front()).x + App->map->data.tile_diagonal / 2) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.x -= offset.x;
	//	break;
	//case north_west:
	//	if (position.y <= App->map->MapToWorldPoint(path.front()).y) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.y += offset.y;
	//	aux_pos.x += offset.x;
	//	break;
	//case south_west:
	//	if (position.y <= App->map->MapToWorldPoint(path.front()).y) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.x += offset.x;
	//	break;
	//case west:
	//	if (position.x <= App->map->MapToWorldPoint(path.front()).x + App->map->data.tile_diagonal / 2) {
	//		path.erase(path.begin());
	//		checked_next_tile = false;
	//		w_direction = fPoint(path.front().x - position_map.x, path.front().y - position_map.y);
	//		SetDirection();
	//		LookAtMovement();
	//		return;
	//	}
	//	aux_pos.x += offset.x;
	//	break;
	//}

	

	if (!checked_next_tile && App->map->entity_matrix[path.front().x][path.front().y] != nullptr) {
		checked_next_tile = true;
		Unit* local = (Unit*)App->map->entity_matrix[path.front().x][path.front().y];
		if (local->type == entity_type::player || local->type == entity_type::ally) {
			if (local != this) {
				if (path.size() > 1)
					local->ForceMovement(position_map, path.at(1));
				else
					local->ForceMovement(position_map, position_map);
			}
		}

	}

	has_destination = true;
}

void Unit::LookAtMovement()
{
	if (w_direction.x > 0) 
	{
		if (w_direction.y > 0)
		{
			direction = { 0,1 };
			current_animation = &m_south;
			destination = south;
			flip = false;
		}

		else if (w_direction.y < 0)
		{
			direction = { 1,0 };
			current_animation = &m_west;
			destination = east;
			flip = true;
		}
		else
		{
			direction = { +1,0.5 };
			current_animation = &m_south_west;
			destination = south_east;
			flip = true;
		}

	}
	else if (w_direction.x < 0)
	{
		if (w_direction.y > 0)
		{
			direction = { -1,0 };
			current_animation = &m_west;
			destination = west;
			flip = false;
		}
		else if (w_direction.y < 0)
		{
			direction = { 0,-1 };
			current_animation = &m_north;
			destination = north;
			flip = false;
		}

		else
		{
			direction = { -1,-1 };
			current_animation = &m_north_west;
			destination = north_west;
			flip = false;
		}
	}
	else 
	{
		if (w_direction.y > 0)
		{
			direction = { -1,0.5 };
			current_animation = &m_south_west;
			destination = south_west;
			flip = false;
		}
		else if (w_direction.y < 0)
		{
			direction = { 1,-0.5 };
			current_animation = &m_north_west;
			destination = north_east;
			flip = true;
		}
	}
	//direction.Normalize();
	//iPoint direction_i = App->map->MapToWorldPoint(path.front()) - App->map->MapToWorldPoint(position_map);
	//direction = { (float)direction_i.x, (float)direction_i.y };
	direction.Normalize();
}

void Unit::ForceMovement(iPoint origin, iPoint next_position)
{
	PathNode current = { 0,0,position_map,nullptr };
	PathList walkable;
	current.FindWalkableAdjacents(walkable);
	bool found = false;

	while (walkable.node_list.size() != 0) {
		PathNode temp;
		temp = walkable.node_list.back();
		walkable.node_list.pop_back();
		if (temp.pos != next_position && temp.pos != origin) 			{
			path_id = App->pathfinding->CreatePath(position_map, temp.pos);
			found = true;
			break;
		}
	}
	if (!found)
		path_id = App->pathfinding->CreatePath(position, next_position);
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
					if (neighbors[k].x >= 0 && neighbors[k].y >= 0 && neighbors[k].x < App->map->data.width && neighbors[k].y < App->map->data.height)
					{
						Entity* found = (Entity*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
						if (found != nullptr && found->life > 0) {
							if ((App->pathfinding->IsWalkable(App->map->WorldToMapPoint(found->position)) && (found->type == entity_type::enemy || found->type == entity_type::ally || found->type == entity_type::player)) || (!App->pathfinding->IsWalkable(App->map->WorldToMapPoint(found->position)) && (found->type == entity_type::building || found->type == entity_type::ally_building || found->type == entity_type::enemy_building))) {

								switch (type) {
								case player:
								case ally:
									if (found->type == enemy) {
										attacked_unit = (Unit*)found;
										state = entity_move_to_enemy;
										return true;
									}
									if (found->type == entity_type::enemy_building) {
										attacked_building = (Building*)found;
										state = entity_move_to_building;
										return true;
									}
									break;
								case enemy:
									if (found->type == player || found->type == ally) {
										attacked_unit = (Unit*)found;
										state = entity_move_to_enemy;
										return true;
									}
									if (found->type == entity_type::ally_building || (found->type == entity_type::building && found->name == entity_name::barracks)) {
										attacked_building = (Building*)found;
										state = entity_move_to_building;
										return true;
									}
								}
							}
						}
						else {
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
	iPoint pos = position;
	attacked_pos = App->map->WorldToMapPoint(attacked_pos);
	pos = App->map->WorldToMapPoint(pos);

	direction.x = attacked_pos.x - pos.x;
	direction.y = attacked_pos.y - pos.y;

	if (attacked_entity->type == entity_type::enemy_building || attacked_entity->type == entity_type::ally_building || attacked_entity->type == entity_type::building)
	{
		if (std::abs(direction.x) > range + 2 || std::abs(direction.y) > range + 2) ret = false;
	}
	else if (attacked_entity->type != entity_type::object) {
		if (std::abs(direction.x) > range || std::abs(direction.y) > range) ret = false;
	}
	else {
		if (std::abs(direction.x) > range || std::abs(direction.y) > range) ret = false;
	}
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
	if (attacked_unit != nullptr) {
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
	else state = entity_idle;
}

void Unit::BuildingAttack()
{
	if (attacked_building != nullptr) {
		LookAtAttack();

		if (current_animation->GetFrameIndex() == 5 && shout_fx == true) {
			App->audio->PlayFx(RandomGenerate(App->scene->scene_test->swords_clash_id, App->scene->scene_test->swords_clash4_id));
			shout_fx = false;
		}

		if (current_animation->Finished())
		{
			attacked_building->life -= damage;
			current_animation->Reset();
			if (attacked_building->life <= 0)
			{
				//App->audio->PlayFx(RandomGenerate(App->scene->scene_test->death_id, App->scene->scene_test->death2_id)); need an audio for destroying a building
				state = entity_idle;
				attacked_building->state = entity_death;
				attacked_building = nullptr;
			}
			shout_fx = true;
		}
	}
	else state = entity_idle;
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

void Unit::SetPickObject(Object* object)
{
	to_pick_object = object;
}

void Unit::PickObject()
{
	App->player->item_drop->SetEnabled(true);
	to_pick_object->state = object_picked;
	speed -= 1;
	is_holding_object = true;
}

void Unit::DropObject()
{
	App->player->item_drop->SetEnabled(false);
	to_pick_object->state = object_dropped;
	speed += 1;
	is_holding_object = false;
	to_pick_object = nullptr;
}

bool Unit::IsInsideCircle(int x, int y)
{
	iPoint center = position;
	return (x - center.x) ^ 2 + (y - center.y) ^ 2 <= radius_of_action*radius_of_action;
}

