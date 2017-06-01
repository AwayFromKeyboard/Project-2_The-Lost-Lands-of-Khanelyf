#ifndef _BATTLECRY_BUFF_
#define _BATTLECRY_BUFF_

#include "Particle.h"
#include "Animation.h"

class BattlecryBuff : public Particle {
public:
	BattlecryBuff() {}
	BattlecryBuff(int set);
	~BattlecryBuff();

	bool Draw(float dt);
	bool Update(float dt);

	bool LoadParticle(iPoint pos);

private:


public:
	int set = 0;
	iPoint offset = NULLPOINT;

	Animation* current_animation;
	// Idle
	Animation movement;

};



#endif
