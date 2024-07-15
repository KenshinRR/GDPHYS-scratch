#include "DragForceGenerator.h"

using namespace P6;

void DragForceGenerator::UpdateForce(P6Particle* particle, float time) {
	MyVector force = MyVector(0, 0, 0);

	MyVector currV = particle->Velocity;

	float mag = currV.Magnitude();
	if (mag <= 0) return;

	float dragF = (k1 * mag) + (k2 * mag);

	MyVector dir = currV.Direction();

	particle->AddForce(dir * -dragF);
}