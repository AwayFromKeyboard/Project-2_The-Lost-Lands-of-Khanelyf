#include "Building.h"
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Scene.h"
#include "Barracks.h"
#include "Functions.h"

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

	max_life = life;

	return ret;
}

bool Building::PreUpdate()
{
	bool ret = true;

	if (state == entity_state::entity_idle) {
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
			App->entity->CreateBuildingEntity(basic_building, ally_building, position, building_rect_number);
		}

		break;
	}

	return true;
}

bool Building::Draw(float dt)
{
	bool ret = true;

	if (state == entity_idle)
		App->scene->LayerBlit(6, entity_texture, { position.x - offset.x, position.y - offset.y }, tex_rect);

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

void Building::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
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
