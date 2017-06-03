#include "Building.h"
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Scene.h"
#include "Barracks.h"
#include "Functions.h"
#include "QuestManager.h"
#include "j1App.h"
#include "Player.h"

Building::Building()
{
}

Building::~Building()
{
}

bool Building::LoadEntity()
{
	bool ret = true;

	return ret;
}

bool Building::Start()
{
	bool ret = true;

	layer = 5;
	max_life = life;

	if (name == entity_name::basic_building && type == entity_type::ally_building) {
		App->scene->scene_test->human_resources_max++;
	}

	CheckIDInRect();

	return ret;
}

bool Building::PreUpdate()
{
	bool ret = true;

	if (state == entity_state::entity_idle && name != entity_name::broken_building) {
		if (type == entity_type::building)
			LifeBar({ 185, 5 }, { -100, -100 });
		else {
			if (name == entity_name::towers)
				LifeBar({ 125, 5 }, { -60, -85 });
			else if (name == entity_name::blacksmiths)
				LifeBar({ 145,5 }, { -80,-115 });
			else if (name == entity_name::barracks)
				LifeBar({ 195,5 }, { -90,-170 });
			else
				LifeBar({ 125, 5 }, { -65, -60 });
		}

	}

	if (life > 0) {
		iPoint position_map = App->map->WorldToMapPoint(position);
		App->map->entity_matrix[position_map.x][position_map.y] = this;
	}
	
	return ret;
}

bool Building::Update(float dt)
{
	if (collision != nullptr)
		collision->SetPos(position.x + collision->offset_x, position.y + collision->offset_y);

	switch (state) {
	case entity_death:

		if (name == entity_name::basic_building && type == entity_type::ally_building) {
			App->scene->scene_test->human_resources_max--;
		}

		if(collision != nullptr)
			App->collisions->EraseCollider(collision);
		to_delete = true;
		if (type == entity_type::enemy_building) {
			if (id == quest_4) {
				if (App->questmanager->GetCurrentQuest()->id == quest_id::quest_conquer)
					App->questmanager->GetCurrentQuest()->progress++;
				else App->scene->scene_test->progress_quest_4++;
			}
			if (name == basic_building)
				App->entity->CreateBuildingEntity(basic_building, ally_building, position, building_rect_number);
			else if (name == towers)
				App->entity->CreateEntity(towers, ally_building, position);
			else if (name == blacksmiths)
				App->entity->CreateEntity(blacksmiths, building, position);
		}
		if (type == entity_type::ally_building || type == entity_type::building && name != entity_name::broken_building) {

			if (type == entity_type::building && name == entity_name::blacksmiths)
			{
				App->entity->CreateEntity(broken_building, building, iPoint(position.x - 40, position.y - 50));

				App->player->choose_ability_b->click_through = true;
				App->player->choose_ability_b->enabled = false;
				App->player->choose_ability_b_txt->enabled = false;

				App->player->choose_ability_uw->click_through = true;
				App->player->choose_ability_uw->enabled = false;
				App->player->choose_ability_uw_txt->enabled = false;

				App->scene->scene_test->create_blacksmith = true;
				App->player->blacksmith_alive = false;
			}
			else
				App->entity->CreateEntity(broken_building, building, position);

			int i = 0;

			for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
			{
				if ((*it) != this && (*it)->type == entity_type::ally_building || (*it) != this && (*it)->type == entity_type::building && (*it)->name != entity_name::broken_building)
				{
					i++;
				}
			}

			if (i == 0)
				App->player->lose_status = true;

		}
		if (type == entity_type::building && name == entity_name::barracks) {
			if (App->player->barracks_ui_window->enabled)
				App->player->barracks_ui_window->SetEnabledAndChilds(false);
			App->scene->scene_test->create_barrack = true;
		}
		break;
	}

	return true;
}

bool Building::Draw(float dt)
{
	bool ret = true;

	if (state == entity_idle)
		App->scene->LayerBlit(layer, entity_texture, { position.x - offset.x, position.y - offset.y }, tex_rect);

	return ret;
}

bool Building::PostUpdate()
{
	bool ret = true;

	if (GetSelected())
		App->render->DrawCircle(position.x + App->render->camera.x, position.y + App->render->camera.y, 2, 255, 255, 255);

	return ret;
}

bool Building::CleanUp()
{
	
	bool ret = true;

	return ret;
}

bool Building::Load(pugi::xml_node &)
{
	return true;
}

bool Building::Save(pugi::xml_node &) const
{
	return true;
}

void Building::OnColl(Entity * en1, Entity * en2)
{

	if (en1 == this) {
		if (en2->position.y > collision->rect.y +  collision->rect.h/2) {
			layer = 6;
			en2->layer = 5;
		}
		else {
			layer = 5;
			en2->layer = 6;
		}
	}

}

Collider * Building::GetCollider()
{
	return collision;
}

entity_type Building::GetType()
{
	return type;
}

entity_name Building::GetName()
{
	return name;
}
