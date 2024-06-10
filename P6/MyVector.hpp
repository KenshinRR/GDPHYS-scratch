#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {
	class MyVector {
	public:
		float x, y, z;

		MyVector() : x(0), y(0), z(0) {}
		MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }

		//Operations
	//addition
		MyVector operator+ (const MyVector v) {
			return MyVector(
				this->x + v.x,
				this->y + v.y,
				this->z + v.z
			);
		}

		void operator+= (const MyVector v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
		}

		//subtraction
		MyVector operator- (const MyVector v) {
			return MyVector(
				this->x - v.x,
				this->y - v.y,
				this->z - v.z
			);
		}

		void operator-= (const MyVector v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
		}

		//scalar muliplication
		MyVector operator* (const float s) {
			return MyVector(
				this->x * s,
				this->y * s,
				this->z * s
			);
		}

		void operator*= (const float s) {
			this->x *= s;
			this->y *= s;
			this->z *= s;
		}

		//magnitude
		float getMagnitude() {
			return sqrtf({
				this->x * this->x +
				this->y * this->y +
				this->z * this->z
				});
		}

		//direction
		MyVector getDirection() {
			return MyVector(
				this->x / this->getMagnitude(),
				this->y / this->getMagnitude(),
				this->z / this->getMagnitude()
			);
		}

		//component product
		MyVector operator* (const MyVector v) {
			return MyVector(
				this->x * v.x,
				this->y * v.y,
				this->z * v.z
			);
		}

		//dot product
		float dotProduct(const MyVector v) {
			return (this->x + v.x) + (this->y + v.y) + (this->z + v.z);
		}

		//vector product
		MyVector crossProduct(const MyVector v) {
			return MyVector(
				(this->y * v.z) - (this->z * v.y),
				(this->z * v.x) - (this->x * v.z),
				(this->x * v.y) - (this->y * v.x)
			);
		}
	};
}