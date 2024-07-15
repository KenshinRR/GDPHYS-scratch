#pragma once

#include "Camera.h"
#include "../Renderer/Shader.h"

class OrthoCamera : public Camera {
private:
	glm::mat4 projection;
public:
	OrthoCamera(glm::vec3 pos, glm::vec3 WorldUp, glm::vec3 Front,
		float height, float width, float zfar, bool Dstate);
public:

	void perfromSpecifics(Shader* shaderProg);
	
	glm::mat4 getProjection();
};