#ifndef _HERO_H_
#define _HERO_H_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

class Hero : public Unit
{
public:
	Hero(entity_type _type);
	~Hero();

	// Load animations
	bool LoadEntity(iPoint pos, entity_name name);

public:
	iPoint mouse_position;
	int levelup_points = 0;
};

#endif