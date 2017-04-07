#ifndef _Test_
#define _Test_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

class GameObject;
class Hero : public Unit
{
public:
	Hero(entity_type _type);
	~Hero();

	// Load animations
	bool LoadEntity();

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	iPoint mouse_position;
	int levelup_points = 0;
};

#endif