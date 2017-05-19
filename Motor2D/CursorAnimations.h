#ifndef _CURSOR_ANIMATION_H_
#define _CURSOR_ANIMATION_H_

#include "Particle.h"
#include "Animation.h"

class CursorAnimations : public Particle {
public:
	CursorAnimations() {}
	CursorAnimations(int set);
	~CursorAnimations();

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



#endif // !_FIRE_H_