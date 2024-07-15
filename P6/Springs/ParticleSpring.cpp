#include "ParticleSpring.h"

void P6::ParticleSpring::UpdateForce(P6Particle* particle, float time)
{
	MyVector pos = particle->Position;

	MyVector force = pos - this->otherParticle->Position;

	float mag = force.Magnitude();

	float springForce = -springConstant * abs(mag - this->restLength);

	force = force.Direction();

	force = force * springForce;

	particle->AddForce(force);
}
