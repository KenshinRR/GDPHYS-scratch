#pragma once

#include "../P6/Vector.h"

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

using namespace P6;

class RenderLine {
private:
	MyVector P1;
	MyVector P2;
	glm::mat4 projectionMatrix;

	unsigned int ID;

public:
	RenderLine(MyVector p1, MyVector p2, glm::mat4 projectionMat) :
		P1(p1), P2(p2), projectionMatrix(projectionMat), ID(glCreateProgram()) {}

	void Update(MyVector p1, MyVector p2, glm::mat4 projectionMatrix);
	void Draw();
};