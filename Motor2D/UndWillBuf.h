#ifndef _UNDWILL_BUFF_
#define _UNDWILL_BUFF_

#include "Particle.h"
#include "Animation.h"

class UndWillBuff : public Particle {
public:
	UndWillBuff() {}
	UndWillBuff(int set);
	~UndWillBuff();

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
