#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "SDL/include/SDL.h"
#include "Point.h"

class Animator;

enum particle_type {
	fire,

	particle_null
};

class Particle {
public:
	Particle();
	~Particle();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool Draw(float dt);
	bool CleanUp();

	bool LoadParticle(iPoint pos);

public:
	particle_type type = particle_type::particle_null;
	iPoint position = NULLPOINT;
	SDL_Texture* entity_texture = nullptr;
	Animator* animator = nullptr;

public:
	
};



#endif // !_FIRE_H_
