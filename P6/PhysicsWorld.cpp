#include "PhysicsWorld.h"

void P6::PhysicsWorld::AddParticle(P6Particle* toAdd)
{
	this->Particles.push_back(toAdd);
	
	forceRegistry.Add(toAdd, &Gravity);
}

void P6::PhysicsWorld::Update(float time)
{
	this->UpdateParticleList();

	forceRegistry.UpdateForces(time);

	for (P6Particle* particle : this->Particles) {
		particle->Update(time);
	}
}

void P6::PhysicsWorld::UpdateParticleList()
{
}
