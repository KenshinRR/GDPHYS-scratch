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
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float theta_x;
	float theta_y;
	float theta_z;
	glm::mat4 identity_matrix4;

	Shader* shaderProg;
	GLuint VAO, VBO;
	std::vector<GLfloat> fullVertexData;

public:
	Model3D(glm::vec3 position,
		const char* vertexPath,
		const char* fragmentPath);

	Model3D(glm::vec3 position,
		std::vector<GLfloat> fullVertexData,
		const char* vertexPath, 
		const char* fragmentPath, float rot);

public:
	void draw();
	virtual void draw(Shader* shaderProgB, GLuint* VAOB, std::vector<GLfloat>* fullVertexDataB);

	void rotate(char axis,char direction);

	void initVAO();

	void deleteVAO();

public:
	void setFullVertexData(std::vector<GLfloat> fullVertexData);
	void setPosition(glm::vec3 pos);

public:
	Shader* getShader();
	unsigned int getShaderID();
	float getYrot();
	glm::vec3 getPosition();
};