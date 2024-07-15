#pragma once

#include <list>
#include "Particle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "Links/ParticleLink.h"


namespace P6 {
	class PhysicsWorld {
	public:
		ForceRegistry forceRegistry;

	public:
		std::list<P6Particle*> Particles;
		std::vector<ParticleContact*> Contacts;
		std::list<ParticleLink*> Links;

	protected:
		ContactResolver contactResolver = ContactResolver(20);

	public:
		void AddParticle(P6Particle* toAdd);
		void AddContact(P6Particle* p1, P6Particle* p2, float restitution, MyVector contactNormal, float depth);

		void Update(float time);
		void GenerateContacts();

	private:
		//checking what particles need to be removed
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, 0, 0));

	protected:
		void GetOverLaps();

	};
}

