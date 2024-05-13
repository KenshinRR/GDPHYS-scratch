#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Light {
protected:
	//Light variables
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float ambientStr;
	glm::vec3 ambientColor;
	float specStr;
	float specPhong;

public:
	Light(
		glm::vec3 lightPos,
		glm::vec3 lightColor,
		float ambientStr,
		float specStr,
		float specPhong
	);

public:
	void attachFundamentals(Shader* shaderProg);
	virtual void attachSpecifics(Shader* shaderProg) = 0;
};