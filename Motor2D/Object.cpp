#include "Object.h"
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Scene.h"
#include "Functions.h"
#include "Player.h"
#include "Hero.h"
#include "QuestManager.h"

Object::Object()
{
}

Object::~Object()
{
}

bool Object::LoadEntity()
{
	bool ret = true;

	return ret;
}

bool Object::Start()
{
	bool ret = true;

	CheckIDInRect();

	return ret;
}

bool Object::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Object::Update(float dt)
{

	switch (state) {

	case entity_idle:
		collision->SetPos(position.x + collision->offset_x, position.y + collision->offset_y);
		if (death_timer.ReadSec() > 10 && pickable == false) {
			state = object_destroyed;
		}
		break;

	case object_picked:
		if (aux_collision_rect.x == 0 && aux_collision_rect.y == 0 && aux_collision_rect.w == 0 && aux_collision_rect.h == 0) {
			aux_collision_rect = collision->rect;
		}
		collision->rect = NULLRECT;
		break;

	case object_dropped:
		collision->rect = aux_collision_rect;
		aux_collision_rect = NULLRECT;
		position = App->player->GetHero()->position;
		state = entity_idle;
		if (App->questmanager->GetCurrentQuest()->type == quest_type::move_object && App->questmanager->GetCurrentQuest()->id == quest_id::quest_provisions &&  position.PointInRect(App->questmanager->provision_quest_rect.x, App->questmanager->provision_quest_rect.y, App->questmanager->provision_quest_rect.w, App->questmanager->provision_quest_rect.h))
		{
			pickable = false;
			App->questmanager->GetCurrentQuest()->progress++;
			death_timer.Start();
		}
		break;

	case object_destroyed:
		if (collision != nullptr)
			App->collisions->EraseCollider(collision);
		to_delete = true;
		break;

	}

	return true;
}

bool Object::Draw(float dt)
{
	bool ret = true;

	if (state == entity_idle)
		App->scene->LayerBlit(6, entity_texture, { position.x - offset.x, position.y - offset.y }, tex_rect);

	return ret;
}

bool Object::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Object::CleanUp()
{

	bool ret = true;

	return ret;
}

bool Object::Load(pugi::xml_node &)
{
	return true;
}

bool Object::Save(pugi::xml_node &) const
{
	return true;
}

void Object::OnColl(Entity * en1, Entity * en2)
{
}

Collider * Object::GetCollider()
{
	return collision;
}

entity_type Object::GetType()
{
	return type;
}

entity_name Object::GetName()
{
	return name;
}
