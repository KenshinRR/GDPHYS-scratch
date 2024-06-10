#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "math.h"

namespace P6 {
	class MyVector {
	public:
		float x, y, z;

	public:
		MyVector();
		MyVector(glm::vec3 vec3);
		MyVector(const float _x, const float _y, const float _z);
		
	public:
		MyVector operator* (const float v);
		MyVector operator+ (const MyVector v);
		MyVector operator/ (const float v);

		void operator += (const MyVector v); //3 Addition
		void operator -= (const MyVector v); //4 Subtraction
		void operator *= (const MyVector v); //6 Compnnent Product
		
	public:
		
		float Magnitude();//1 Magnitude
		glm::vec3 Direction();//2 Direction
		glm::vec3 ScalarMult(float i);//5 ScalarMult
		float DotProduct(MyVector v);//7 Scalar Product
		glm::vec3 VectorProduct(MyVector v); //8 Cross Product
		
	public:
		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }
		
	};
}