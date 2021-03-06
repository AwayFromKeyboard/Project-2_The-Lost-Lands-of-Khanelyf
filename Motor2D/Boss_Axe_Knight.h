#ifndef _BOSS_AXE_KNIGHT_
#define _BOSS_AXE_KNIGHT_

#include "Unit.h"
#include "j1Timer.h"
#define	COLLISION_ADJUSTMENT iPoint(0, 0)

#define PHASE2_DAMAGE 100
#define PHASE2_RANGE 3

#define PHASE3_DAMAGE 5
#define PHASE3_RANGE 4

enum entity_type;
class Particle;

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

	j1Timer ability_phase3;
	bool starter_ability_phase3_timer = false;
	bool fireballs_created = false;
	void Draw_Phase3();
	void Phase3_Attack();
	void Phase3_Damage();
	j1Timer damage_ticks;
	bool tick_started = false;
	std::list<iPoint> fireball_points;
	std::list<Particle*> fireballs;

	j1Timer ability_last_phase;
	bool starter_ability_last_phase_timer = false;
	void Draw_LastPhase();
	int modifier = 0;

	std::list<iPoint> range_visited;
	std::list<iPoint> range_visited2;
};

#endif
