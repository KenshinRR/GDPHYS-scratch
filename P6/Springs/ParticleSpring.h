#pragma once

#include "../ForceGenerator.h"
#include "../Particle.h"

namespace P6 {
	class ParticleSpring : public ForceGenerator {
	private:
		P6Particle* otherParticle;
		float springConstant;
		float restLength;

	public:
		ParticleSpring(P6Particle* p, float _springConstant, float _restLength) : 
			otherParticle(p), springConstant(_springConstant), restLength(_restLength) {}

		void UpdateForce(P6Particle* particle, float time) override;
	};
}