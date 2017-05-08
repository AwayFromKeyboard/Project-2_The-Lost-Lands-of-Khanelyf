#ifndef _SWORDSMAN_H_
#define _SWORDSMAN_H_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

enum entity_type;

class Swordsman : public Unit
{
public:
	Swordsman(entity_type _type);
	~Swordsman();

	// Load animations
	bool LoadEntity(iPoint pos, entity_name name);

public:
	iPoint mouse_position;
};

#endif