#ifndef _PROVISIONS_H_
#define _PROVISIONS_H_

#include "Object.h"

class Provisions : public Object
{
public:
	Provisions(entity_type _type);
	~Provisions();

	bool LoadEntity(iPoint pos);

	bool Start();

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
};

#endif
