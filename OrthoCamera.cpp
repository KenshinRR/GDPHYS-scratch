#include "OrthoCamera.h"


OrthoCamera::OrthoCamera(glm::vec3 pos) : Camera(pos) {
    //initialize ortho cam
	this->projection = glm::ortho(
        -1.5f, //left
        1.5f, //right
        -1.5f, //bot
        1.5f, //top
        -0.1f, //z near
        100.f);  //z far
    this->WorldUp = glm::vec3(0.f, 1.0f, 0.f);
    this->Front = glm::vec3(0.0f, -3.0f, -0.1f);
}
glm::mat4 OrthoCamera::getProjection() {
	return this->projection;
}