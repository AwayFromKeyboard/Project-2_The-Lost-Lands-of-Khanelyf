#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"

class ParticleManager : public j1Module {
public:
	ParticleManager();
	~ParticleManager();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();





};

#endif // !_PARTICLE_MANAGER_H_
