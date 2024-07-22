/*
	During a bungee jump, the jumper only gets pull up when the has stretched
	or rather after the jumper's distance from where the line is attached to
	is equal to or greater than the length of the rope

	the same process is applied here wherein only after the current distance between
	the jumper and the anchor point	is equal to or greater than the rope's length
	is when the force is applied to the jumper i.e the particle

	the spring constant is set to 10 to make it so that the jumper doesn't go
	above the anchor point
*/

#include "Bungee.h"

void P6::Bungee::UpdateForce(P6Particle* particle, float time)
{
	this->currLength = (this->anchorPoint - particle->Position).Magnitude();
	if (this->currLength < this->length) return;

	MyVector pos = particle->Position;

	MyVector force = pos - this->anchorPoint;

	float mag = force.Magnitude();

	float springConstant = 10.0f;

	float springForce = -springConstant * abs(mag - this->length);

	force = force.Direction();

	force = force * springForce;

	particle->AddForce(force);
}
