#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera {
private:
	glm::mat4 projection;
public:
	PerspectiveCamera(glm::vec3 pos , glm::vec3 WorldUp, glm::vec3 Front, float fov, float height, float width,float zfar, bool Dstate);

public:
	void perfromSpecifics(Shader* shaderProg);

public:
	glm::mat4 getProjection();
	
};