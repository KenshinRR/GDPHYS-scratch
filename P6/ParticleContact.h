#pragma once

#include "Particle.h"
#include "Vector.h"

namespace P6{
	class ParticleContact
	{
	public:
		//particles in contact
		P6Particle* particles[2];
		//holds the coefficient of restitution
		float restitution;
		//contact normal of this collision
		MyVector contactNormal;
		//interpenetration depth
		float depth;

	public:
		//resolve this contact
		void Resolve(float time);
		//get the seperating speed of this collision
		float GetSeperatingSpeed();

	protected:
		//calculates the new velocities of the objects
		void ResolveVelocity(float time);
		//
		void ResolveInterpenetration(float time);
	};
}
