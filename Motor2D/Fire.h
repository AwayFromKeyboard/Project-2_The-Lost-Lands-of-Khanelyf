#ifndef _FIRE_H_
#define _FIRE_H_

#include "Particle.h"
#include "Animation.h"

class Fire : public Particle {
public:
	Fire() {}
	Fire(int set);
	~Fire();

	bool Draw(float dt);

	bool LoadParticle(iPoint pos);

private:


public:
	int set = 0;

	Animation* current_animation;
	// Idle
	Animation set_0;
	Animation set_1;
	Animation set_2;
	Animation set_3;
	Animation set_4;

};



#endif // !_FIRE_H_
