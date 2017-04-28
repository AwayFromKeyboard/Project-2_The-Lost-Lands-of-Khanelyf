#ifndef _Test_
#define _Test_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

class Hero : public Unit
{
public:
	Hero(entity_type _type);
	~Hero();

	// Load animations
	bool LoadEntity(iPoint pos);

public:
	iPoint mouse_position;
	int levelup_points = 0;
};

#endif