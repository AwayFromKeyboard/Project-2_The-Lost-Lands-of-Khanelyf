#include "ParticleManager.h"
#include "Particle.h"
#include "Fire.h"
#include "Log.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

bool ParticleManager::Start()
{
	bool ret = true;

	for (std::list<Particle*>::iterator it = particle_list.begin(); it != particle_list.end(); it++) {
		ret = (*it)->Start();
	}

	return ret;
}

bool ParticleManager::PreUpdate()
{
	bool ret = true;

	for (std::list<Particle*>::iterator it = particle_list.begin(); it != particle_list.end(); it++) {
		ret = (*it)->PreUpdate();
	}

	return ret;
}

bool ParticleManager::Update(float dt)
{
	bool ret = true;

	for (std::list<Particle*>::iterator it = particle_list.begin(); it != particle_list.end(); it++) {
		ret = (*it)->Update(dt);
		(*it)->Draw(dt);
	}

	return ret;
}

bool ParticleManager::PostUpdate()
{
	bool ret = true;

	for (std::list<Particle*>::iterator it = particle_list.begin(); it != particle_list.end();) {
		if ((*it)->to_delete == true) {
			list<Particle*>::iterator it_next = std::next(it);
			DeleteParticle(*it);
			it = it_next;
		}
		else {
			ret = (*it)->PostUpdate();
			++it;
		}
	}

	return ret;
}

bool ParticleManager::CleanUp()
{
	bool ret = true;

	for (std::list<Particle*>::iterator it = particle_list.begin(); it != particle_list.end();)
	{
		list<Particle*>::iterator it_next = ++it;
		--it;
		DeleteParticle(*it);
		it = it_next;
	}

	return ret;
}

Particle* ParticleManager::CreateParticle(particle_type type, int set, iPoint pos)
{
	Particle* ret = nullptr;

	switch (type)
	{
	case particle_type::fire:
		ret = new Fire(set);
		break;
	}

	if (ret != nullptr)
	{
		ret->LoadParticle(pos);
		ret->Start();
		particle_list.push_back(ret);
	}
	else
		LOG("Particle creation returned nullptr");

	return ret;
}

void ParticleManager::DeleteParticle(Particle* particle)
{
	particle->CleanUp();
	particle_list.remove(particle);
	RELEASE(particle);
}