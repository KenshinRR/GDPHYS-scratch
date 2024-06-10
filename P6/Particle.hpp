#pragma once

#include "MyVector.hpp"

namespace P6 {
	class Particle {
	private:
		MyVector position;
		MyVector velocity;
		MyVector accel;
		float mass;
	};
}