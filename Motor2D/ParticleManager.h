#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "Point.h"

class Particle;
enum particle_type;



class ParticleManager : public j1Module {
public:
	ParticleManager();
	~ParticleManager();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Particle * CreateParticle(particle_type type, int set, iPoint pos);

public:
	std::list<Particle*> particle_list;



};

#endif // !_PARTICLE_MANAGER_H_
