#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 WorldUp, glm::vec3 Front,bool Dstate){

    //camera
    
    /*      INITIAL CAMERA VARIABLES        */
    this->cameraPos = pos;
    this->WorldUp = WorldUp;
    this->Front = Front;
    this->DState = Dstate;

    this->viewMatrix = glm::lookAt(pos, Front, WorldUp);
}

void Camera::performFundamentals(Shader* shaderProg)
{
    shaderProg->setMat4("view", viewMatrix);
    //shaderProg->setVec3("camerapos", cameraPos); this one is for lights
}

void Camera::setCameraPos(glm::vec3 camPos) {
    this->cameraPos = camPos;
}

void Camera::setFront(glm::vec3 Front) {
    this->Front = Front;
}

glm::vec3 Camera::getFront() {
    return this->Front;
}

glm::vec3 Camera::getWorldUp() {
    return this->WorldUp;
}
	
glm::mat4 Camera::getViewMat() {
    // camPos + front for 1st person, 3rd person is just front btw this is target
    if (DState) {
        this->viewMatrix = glm::lookAt(this->cameraPos, this->Front, this->WorldUp); //apparently perspective
    }
    else {
        this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->Front, this->WorldUp); //ortho?
    }
    
    return this->viewMatrix;
}


void Camera::setViewMatrix() {
    this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->Front, this->WorldUp);
}

glm::vec3 Camera::getCameraPos() {
    return this->cameraPos;
}