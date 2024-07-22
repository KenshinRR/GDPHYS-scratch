/*

The logic is simple, after the currLength is greater than or equal to the actual length
of the rope, add force equal to the opposite force exerting on the object which will make
it stop. There is also a constant force opposite to the gravity to make it hang it place

*/

#include "Chain.h"

void P6::Chain::UpdateForce(P6Particle* particle, float time)
{
	this->currLength = (this->anchorPoint - particle->Position).Magnitude();
	if (this->currLength < this->length) return;

	MyVector pos = particle->Position;

	MyVector force = (MyVector(0, 9.8f, 0) + particle->Velocity * -1) * particle->mass;

	particle->AddForce(force);
}
