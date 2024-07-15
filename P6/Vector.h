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

		MyVector(const float _x, const float _y, const float _z);
		
	public:
		MyVector operator* (const float v);// multiplies myvector by a constant and returns it
		MyVector operator+ (const MyVector v);// adds myvector by a constant and returns it
		MyVector operator- (const MyVector v);// subtracts myvector by a constant and returns it
		MyVector operator/ (const float v);// divides myvector by a constant and returns it
		

		void operator += (const MyVector v); //3 Addition
		void operator -= (const MyVector v); //4 Subtraction
		void operator *= (const MyVector v); //6 Compnnent Product
		
	public:
		
		float Magnitude();//1 Magnitude
		float SquareMagnitude();//1 Magnitude
		MyVector Direction();//2 Direction
		MyVector ScalarMult(float i);//5 ScalarMult
		float DotProduct(MyVector v);//7 Scalar Product
		glm::vec3 VectorProduct(MyVector v); //8 Cross Product
		
	public:
		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }//turns myvector into glm::vec3
		
	};
}