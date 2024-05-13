#include "Camera.h"

Camera::Camera(glm::vec3 pos){

    //camera
    
    /*      INITIAL CAMERA VARIABLES        */
    this->cameraPos = pos;
    /*this->WorldUp = glm::vec3(0.f, 1.0f, 0.f);
    this->Front = glm::vec3(0.0f, -3.0f, -0.1f);*/

    this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->Front, this->WorldUp);
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
    return this->viewMatrix;
}

glm::vec3 Camera::getCameraPos() {
    return this->cameraPos;
}