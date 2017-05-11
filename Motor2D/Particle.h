#ifndef _PARTICLE_H_
#define _PARTICLE_H_

enum particle_type {
	fire,

	null
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

	void LoadParticle(iPoint pos);

public:
	particle_type type = particle_type::null;
	iPoint position = NULLPOINT;
	SDL_Texture* entity_texture;
	Animator* animator;

};



#endif // !_FIRE_H_
