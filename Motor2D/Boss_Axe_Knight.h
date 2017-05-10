#ifndef _BOSS_AXE_KNIGHT_
#define _BOSS_AXE_KNIGHT_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

enum entity_type;

class BossAxeKnight : public Unit
{
public:
	BossAxeKnight(entity_type _type);
	~BossAxeKnight();

	// Load animations
	bool LoadEntity(iPoint pos, entity_name name);
public:
	iPoint mouse_position;
};

#endif
