#ifndef _SWORDSMAN_H_
#define _SWORDSMAN_H_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

enum entity_state;
class GameObject;
class Swordsman : public Unit
{
public:
	Swordsman(entity_state state);
	~Swordsman();

	// Load animations
	bool LoadEntity();

	// Start variables
	bool Start();

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	iPoint mouse_position;

public:
	entity_state swordsman_state;
};

#endif
