#ifndef _Test_
#define _Test_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

class GameObject;
class Hero : public Unit
{
public:
	Hero();
	~Hero();

	// Load animations
	bool LoadEntity();

	// Start variables
	bool Start();

	// CleanUp
	bool CleanUp();

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	iPoint mouse_position;

	
};

#endif