#pragma once

#include "Vector.h"
#include <iostream>

namespace P6 {

	
	class P6Particle {
	public:
		float mass; //unit is kg
		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;
		float damping = 0.9f;
	public:
		P6Particle();
		P6Particle(float m, MyVector pos, MyVector vel, MyVector accel);
	protected:
		MyVector accumulatedForce = MyVector(0.0f, 0.0f, 0.0f);

		void UpdateVelocity(float time);
		void UpdatePosition(float time);
		
	public:
		void Update(float time);

		void AddForce(MyVector force);
		void ResetForce();

	public:
	};
}