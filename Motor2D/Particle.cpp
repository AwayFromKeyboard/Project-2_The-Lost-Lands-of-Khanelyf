#include "Particle.h"
#include "Point.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

bool Particle::Start()
{
	return true;
}

bool Particle::PreUpdate()
{
	return true;
}

bool Particle::Update(float dt)
{
	return true;
}

bool Particle::PostUpdate()
{
	return true;
}

bool Particle::Draw(float dt)
{
	return true;
}

bool Particle::CleanUp()
{
	return true;
}

void Particle::LoadParticle(iPoint pos)
{
}
