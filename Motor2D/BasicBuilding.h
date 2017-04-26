#ifndef _BASICBUILDING_H_
#define _BASICBUILDING_H_

#include "Building.h"

class BasicBuilding : public Building
{

public:
	BasicBuilding(entity_type _type);
	~BasicBuilding();

	bool LoadEntity();

	bool Start();

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
};

#endif