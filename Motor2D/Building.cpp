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

	return ret;
}

bool Building::PreUpdate()
{
	bool ret = true;

	if (state == entity_state::entity_idle && name != entity_name::broken_building) {
		if (type == entity_type::building)
			LifeBar({ 185, 5 }, { -100, -100 });
		else
			LifeBar({ 125, 5 }, { -65, -60 });
	}
	
	return ret;
}

bool Building::Update(float dt)
{
	collision->SetPos(position.x + collision->offset_x, position.y + collision->offset_y);

	switch (state) {
	case entity_death:
		if(collision != nullptr)
			App->collisions->EraseCollider(collision);
		to_delete = true;
		if (type == entity_type::enemy_building) {
			if (App->questmanager->GetCurrentQuest()->id == quest_id::quest_conquer)
				App->questmanager->GetCurrentQuest()->progress++;
			App->entity->CreateBuildingEntity(basic_building, ally_building, position, building_rect_number);
		}
		if (type == entity_type::ally_building || type == entity_type::building && name != entity_name::broken_building) {
			App->entity->CreateEntity(broken_building, building, position);
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
