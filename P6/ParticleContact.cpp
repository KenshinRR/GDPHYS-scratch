#include "ParticleContact.h"

using namespace P6;

void P6::ParticleContact::Resolve(float time)
{
	this->ResolveVelocity(time);
	this->ResolveInterpenetration(time);
}

float P6::ParticleContact::GetSeperatingSpeed()
{
	//Av
	MyVector velocity = this->particles[0]->Velocity;
	//						Av			-					Bv
	if (this->particles[1]) velocity -= this->particles[1]->Velocity;
	//		(Av - Bv) dot N
	return velocity.DotProduct(this->contactNormal);
}

void P6::ParticleContact::ResolveVelocity(float time)
{
	//sS
	float seperatingSpeed = this->GetSeperatingSpeed();

	//if object is already moving away from eachother then skip
	if (seperatingSpeed > 0) return;

	//SAfter =		-c				*	sS
	float newSS = -this->restitution * seperatingSpeed;
	//magnitude of our delta velocity
	float deltaSpeed = newSS - seperatingSpeed;

	//get the total inverse of the colliding particles
	//we'll apply impulse based on this later
	float totalMass = ((float)1 / this->particles[0]->mass);
	if (this->particles[1]) totalMass += ((float)1 / this->particles[1]->mass);

	//invalid collision if total mass is 0 or less
	if (totalMass <= 0) return;

	float impulse_mag = deltaSpeed / totalMass;
	MyVector Impulse = contactNormal * impulse_mag;

	MyVector V_a = Impulse * ((float)1 / this->particles[0]->mass);
	this->particles[0]->Velocity += V_a;

	if (this->particles[1]) {
		MyVector V_b = Impulse * ((float)-1 / this->particles[1]->mass);
		this->particles[1]->Velocity += V_b;
	}
}

void P6::ParticleContact::ResolveInterpenetration(float time)
{
	if (depth <= 0) return;

	float totalMass = (float)1 / this->particles[0]->mass;
	if (this->particles[1]) totalMass += (float)1 / this->particles[1]->mass;

	if (totalMass <= 0) return;

	float totalMoveByMass = depth / totalMass;

	MyVector moveByMass = contactNormal * totalMoveByMass;

	MyVector P_a = moveByMass * ((float)1 / this->particles[0]->mass);
	this->particles[0]->Position += P_a;

	if (this->particles[1]) {
		MyVector P_b = moveByMass * (-(float)1 / this->particles[1]->mass);
		this->particles[1]->Position += P_b;
	}

	this->depth = 0;
}
