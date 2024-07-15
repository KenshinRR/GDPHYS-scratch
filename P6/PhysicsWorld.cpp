#include "PhysicsWorld.h"

using namespace P6;

void PhysicsWorld::AddParticle(P6Particle* toAdd) {
	Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}

void P6::PhysicsWorld::AddContact(P6Particle* p1, P6Particle* p2, float restitution, MyVector contactNormal, float depth)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	this->Contacts.push_back(toAdd);
}

void PhysicsWorld::Update(float time) {
	UpdateParticleList();

	forceRegistry.UpdateForces(time);

	for (std::list<P6Particle*>::iterator p = Particles.begin(); p != Particles.end(); p++) {
		(*p)->Update(time);
	}

	this->GenerateContacts();

	if (this->Contacts.size() > 0) {
		this->contactResolver.ResolveContacts(this->Contacts, time);
	}
}

void P6::PhysicsWorld::GenerateContacts()
{
	this->Contacts.clear();

	this->GetOverLaps();

	for (std::list<ParticleLink*>::iterator i = Links.begin();
	i != Links.end();
	i++){
		ParticleContact* contact = (*i)->GetContact();

		if (contact != nullptr) {
			this->Contacts.push_back(contact);
		}
	}
}

void PhysicsWorld::UpdateParticleList() {
	Particles.remove_if([](P6Particle* p) {
		return p->IsDestroyed();
	});
}
void P6::PhysicsWorld::GetOverLaps()
{
	for (int i = 0; i < this->Particles.size() - 1; i++) {
		std::list<P6Particle*>::iterator a = std::next(this->Particles.begin(), i);
		for (int h = i + 1; h < Particles.size(); h++) {
			std::list<P6Particle*>::iterator b = std::next(this->Particles.begin(), h);
			MyVector mag2Vector = (*a)->Position - (*b)->Position;
			float mag2 = mag2Vector.SquareMagnitude();

			float rad = (*a)->radius + (*b)->radius;
			float rad2 = rad * rad;

			// == means touching, < means overlapping
			if (mag2 <= rad2) {
				MyVector dir = mag2Vector.Direction();
				float r = rad2 - mag2;
				float depth = sqrt(r);

				float restitution = fmin(
					(*a)->restitution, (*b)->restitution
				);

				this->AddContact(*a, *b, restitution, dir, depth);
			}
		}
	}
}
;