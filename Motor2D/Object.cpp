#include "Object.h"
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Scene.h"
#include "Functions.h"

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

	return ret;
}

bool Object::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Object::Update(float dt)
{
	collision->SetPos(position.x + collision->offset_x, position.y + collision->offset_y);

	switch (state) {

	case entity_carried:
		
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

void Object::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
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
