#include "Particle.h"


using namespace P6;

P6Particle::P6Particle() : Position(0.f,0.f,0.f), Velocity(0.f, 0.f, 0.f), Acceleration(0.f, 0.f, 0.f), mass(0.f) {

}

P6::P6Particle::P6Particle(float m, MyVector pos, MyVector vel, MyVector accel)
{
	this->mass = m;
	this->Position = pos;
	this->Velocity = vel;
	this->Acceleration = accel;
}

void P6Particle::UpdateVelocity(float time) {
	//add Force
	this->Acceleration += accumulatedForce * (1/this->mass);

	this->Velocity += this->Acceleration * time;
	
	//damping
	this->Velocity = this->Velocity * powf(damping, time);
}

void P6Particle::UpdatePosition(float time) {
	this->Position += (this->Velocity * time) + (this->Acceleration * time * time) * (1.0f / 2.0f);
}

void P6Particle::AddForce(MyVector force) {
	this->accumulatedForce += force;
}
void P6Particle::ResetForce() {
	this->accumulatedForce = MyVector(0, 0, 0);
	this->Acceleration = MyVector(0, 0, 0);
}

void P6Particle::Update(float time) {
	this->UpdatePosition(time);
	this->UpdateVelocity(time);

	//reset forces
	this->ResetForce();

	//lifespan
	this->lifeSpan -= time;
	if (this->lifeSpan <= 0) {
		this->Destroy();
	}
}

void P6Particle::Destroy() {
	this->isDestroyed = true;
}