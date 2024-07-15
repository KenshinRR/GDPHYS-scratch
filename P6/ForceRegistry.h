#pragma once

#include <list>
#include "Particle.h"
#include "ForceGenerator.h"

namespace P6{
	class ForceRegistry{
	public:
		void Add(P6::P6Particle* particle, ForceGenerator* generator);
		void Remove(P6Particle* particle, ForceGenerator* generator);
		void Clear();
		void UpdateForces(float time);
	protected:
		struct ParticleForceRegistry {
			P6Particle* particle;
			ForceGenerator* generator;
		};

		std::list<ParticleForceRegistry> Registry;
	};
}


