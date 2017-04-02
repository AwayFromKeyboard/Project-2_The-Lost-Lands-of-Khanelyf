#ifndef _BARBARIAN_H_
#define _BARBARIAN_H_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

class GameObject;
class Barbarian : public Unit
{
public:
	Barbarian();
	~Barbarian();

	// Load animations
	bool LoadEntity();

	// Start variables
	bool Start();

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	iPoint mouse_position;

};

#endif
