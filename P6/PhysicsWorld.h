#pragma once

#include <list>
#include "Particle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace P6 {
	class PhysicsWorld {
	public:
		ForceRegistry forceRegistry;
		std::list<P6Particle*> Particles;

	public:
		void AddParticle(P6Particle* toAdd);
		void Update(float time);

	private:
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8f, 0));
	};
}