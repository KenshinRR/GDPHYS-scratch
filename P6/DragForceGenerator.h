#pragma once

#include "ForceGenerator.h"
#include "Particle.h"

namespace P6 {
	class DragForceGenerator : public ForceGenerator
	{
	private:
		float k1 = 0.74;

		float k2 = 0.57;
	public:
		DragForceGenerator(){}
		DragForceGenerator(float k1, float k2) : k1(k1), k2(k2){}
		void UpdateForce(P6Particle* particle, float time) override;
	};
}


