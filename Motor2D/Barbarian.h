#ifndef _BARBARIAN_H_
#define _BARBARIAN_H_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

enum entity_type;
class GameObject;

class Barbarian : public Unit
{
public:
	Barbarian(entity_type _type);
	~Barbarian();

	// Load animations
	bool LoadEntity(iPoint pos);

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	iPoint mouse_position;
};

#endif
