#include "GravityForceGenerator.h"

using namespace P6;

void GravityForceGenerator::UpdateForce(P6Particle* particle, float time) {
	if (particle->mass <= 0) return;

	MyVector force = Gravity * particle->mass;
	particle->AddForce(force);

}