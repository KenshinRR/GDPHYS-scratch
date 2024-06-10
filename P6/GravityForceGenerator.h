#pragma once
#include "ForceGenerator.h"
#include "Vector.h"
#include "Particle.h"

namespace P6 {
	class GravityForceGenerator : public ForceGenerator{
	private:
		MyVector Gravity = MyVector(0.0f, -9.8f, 0.0f);
	public:
		GravityForceGenerator(const MyVector gravity) : Gravity(gravity) {};

		void UpdateForce(P6Particle* p, float time) override;
	};
}