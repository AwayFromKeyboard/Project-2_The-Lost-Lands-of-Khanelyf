#ifndef _BOSS_AXE_KNIGHT_
#define _BOSS_AXE_KNIGHT_

#include "Unit.h"
#include "j1Timer.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)
#define WHIRLWIND_DAMAGE 100
#define WHIRLWIND_RANGE 3

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

public:
	j1Timer ability_phase2;
	bool starter_ability_phase2_timer = false;
	void Draw_Phase2();
	void Phase2_Attack();

	std::list<iPoint> range_visited;
};

#endif
