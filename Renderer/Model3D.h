#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include "Shader.h"

class Model3D {
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float theta_x;
	float theta_y;
	float theta_z;
	glm::mat4 identity_matrix4;

	glm::vec3 color;


public:
	Model3D(glm::vec3 position);

public:
	
	void setPos(float x, float y, float z);
	void setColor(float x, float y, float z);
	void setScale(float x, float y, float z);
	
	void draw(Shader* shaderProg, GLuint* VAO, std::vector<GLfloat>* fullVertexData);
	
};