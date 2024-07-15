#include "OrthoCamera.h"


OrthoCamera::OrthoCamera(glm::vec3 pos, glm::vec3 WorldUp, glm::vec3 Front,
float height, float width, float zfar, bool Dstate) : Camera(pos, WorldUp, Front, Dstate) {
	this->projection = glm::ortho(
        -width / 2, //left
        width / 2, //right
        -height / 2, //bot
        height / 2, //top
        -zfar, //z near
        zfar); //z far
}

void OrthoCamera::perfromSpecifics(Shader* shaderProg)
{
    this->performFundamentals(shaderProg);
    shaderProg->setMat4("projection", projection);
}

glm::mat4 OrthoCamera::getProjection() {
	return this->projection;
}