#pragma once

#include "../Particle.h"
#include "../ParticleContact.h"

namespace P6 {
	class ParticleLink {
	public:
		P6Particle* particles[2];
		virtual ParticleContact* GetContact() { return nullptr; };

	protected:
		float CurrentLength();
	};
}