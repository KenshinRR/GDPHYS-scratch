#pragma once

#include "ParticleLink.h"

#include "../ForceGenerator.h"
#include "../Vector.h"

namespace P6 {
	class Bungee : public ForceGenerator{
	public:
		float length;
		float currLength = 0;
		float restitution = 0;
		MyVector anchorPoint;

	public:
		Bungee(MyVector ap, float l) : anchorPoint(ap), length(l) {}

		void UpdateForce(P6Particle* p, float time) override;
	};
}