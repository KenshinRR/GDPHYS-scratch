#pragma once

#include "Camera.h"

class OrthoCamera : public Camera {
private:
	glm::mat4 projection;
public:
	OrthoCamera(glm::vec3 pos);
public:
	
	glm::mat4 getProjection();
};