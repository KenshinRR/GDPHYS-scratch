#include "GravityForceGenerator.h"

void P6::GravityForceGenerator::UpdateForce(P6Particle* p, float time)
{
	if (p->mass <= 0) return;

	MyVector force = this->Gravity * p->mass;
	p->AddForce(force);
}
