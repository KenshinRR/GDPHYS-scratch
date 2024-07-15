#pragma once

#include "Particle.h"

namespace P6 {
	class ForceGenerator {
	public:
		virtual void UpdateForce(P6Particle* p, float time) {
			p->AddForce(MyVector(0, 0, 0));
		}
	};
}