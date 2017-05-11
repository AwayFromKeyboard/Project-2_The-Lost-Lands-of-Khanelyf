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
	return false;
}

bool ParticleManager::PreUpdate()
{
	return false;
}

bool ParticleManager::Update(float dt)
{
	return false;
}

bool ParticleManager::PostUpdate()
{
	return false;
}

bool ParticleManager::CleanUp()
{
	return false;
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
