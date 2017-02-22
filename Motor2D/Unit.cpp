#include "Unit.h"

Unit::Unit()
{
}

Unit::~Unit()
{
}

bool Unit::LoadEntity()
{
	return false;
}

bool Unit::Start()
{
	return false;
}

bool Unit::PreUpdate()
{
	return false;
}

bool Unit::Update(float dt)
{
	return false;
}

bool Unit::Draw(float dt)
{
	return false;
}

bool Unit::PostUpdate()
{
	return false;
}

bool Unit::CleanUp()
{
	return false;
}

bool Unit::Load(pugi::xml_node &)
{
	return false;
}

bool Unit::Save(pugi::xml_node &) const
{
	return false;
}

void Unit::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
