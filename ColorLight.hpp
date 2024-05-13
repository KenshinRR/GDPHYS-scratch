#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class ColorLight{
private:
	float red;
	float blue;
	float green;

public:
	ColorLight();

	void perform(Shader* shaderProg);

public:
	void setColor(float red, float green, float blue);
	void setColor(glm::vec3 rgb);
};