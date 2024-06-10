#include "Vector.h"

using namespace P6;


MyVector::MyVector() : x(0), y(0), z(0) {}

P6::MyVector::MyVector(glm::vec3 vec3)
{
	this->x = vec3.x;
	this->y = vec3.y;
	this->z = vec3.z;
}

MyVector::MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}




MyVector P6::MyVector::operator+(const MyVector v)
{
	return MyVector(this->x + v.x,
		this->y + v.y,
		this->z + v.z);
}

MyVector P6::MyVector::operator*(float v)
{
	return MyVector(this->x * v,
		this->y * v,
		this->z * v);
}
MyVector P6::MyVector::operator/ (const float v)
{
	return MyVector(this->x / v,
		this->y / v,
		this->z / v);
}


void MyVector::operator += (const MyVector v) {
	this->x += v.x,
		this->y += v.y,
		this->z += v.z;
}

void MyVector::operator -= (const MyVector v) {
	this->x -= v.x,
		this->y -= v.y,
		this->z -= v.z;
}

void MyVector::operator *= (const MyVector v) {
	this->x *= v.x,
		this->y *= v.y,
		this->z *= v.z;
}


float MyVector::Magnitude() {
	
	return abs(sqrt(this->x  * this->x + this->y * this->y + this->z * this->z));
}

glm::vec3 MyVector::Direction() {
	return glm::vec3(this->x / this->Magnitude(),
		this->y / this->Magnitude(),
		this->z / this->Magnitude());
}

glm::vec3 MyVector::ScalarMult(float i) {
	return {
		this->x * i,
		this->y * i,
		this->z * i
	};
}

float  MyVector::DotProduct(MyVector v) {
	return this->x* v.x +
		this->y * v.y +		
		this->z * v.z;
}

glm::vec3 MyVector::VectorProduct(MyVector v) {
	return glm::vec3(this->y * v.z - this->z * v.y,
		this->z* v.x - this->x * v.z,
		this->x* v.y - this->y * v.x);
}

