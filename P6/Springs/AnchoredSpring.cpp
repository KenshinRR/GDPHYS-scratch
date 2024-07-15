#include "AnchoredSpring.h"

void P6::AnchoredSpring::UpdateForce(P6Particle* p, float time)
{
	MyVector pos = p->Position;

	MyVector force = (this->anchorPoint * -1) + pos;

	float mag = force.Magnitude();

	float springForce = -this->springConstant * abs(mag - restLength);

	force = force.Direction();

	force = force * springForce;

	p->AddForce(force);
}
