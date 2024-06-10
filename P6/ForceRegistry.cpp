#include "ForceRegistry.h"

void P6::ForceRegistry::Add(P6Particle* particle, ForceGenerator* generator)
{
	ParticleForceRegistry toAdd;
	
	toAdd.particle = particle;
	toAdd.generator = generator;

	Registry.push_back(toAdd);
}

void P6::ForceRegistry::Remove(P6Particle* particle, ForceGenerator* generator)
{
	Registry.remove_if(
		[particle, generator](ParticleForceRegistry reg) {
			return reg.particle == particle && reg.generator == generator;
		}
	);
}

void P6::ForceRegistry::Clear()
{
	this->Registry.clear();
}

void P6::ForceRegistry::UpdateForces(float time)
{
	for (std::list<ParticleForceRegistry>::iterator i = Registry.begin();
		i != Registry.end();
		i++) {
		i->generator->UpdateForce(i->particle, time);
	}
}
