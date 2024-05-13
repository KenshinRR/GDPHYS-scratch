#include "Light.h"

Light::Light(glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, float specStr, float specPhong)
{
    //initial light variables
    this->lightPos = lightPos;

    this->lightColor = lightColor;

    this->ambientStr = ambientStr;

    this->ambientColor = lightColor;

    this->specStr = specStr;

    this->specPhong = specPhong;
}

void Light::attachFundamentals(Shader* shaderProg)
{
    //light
    shaderProg->setVec3("lightPos", lightPos);
    shaderProg->setVec3("lightColor", lightColor);
    //ambient
    shaderProg->setFloat("ambientStr", ambientStr);
    shaderProg->setVec3("ambientColor", ambientColor);
    //specular
    shaderProg->setFloat("specStr", specStr);
    shaderProg->setFloat("specPhong", specPhong);
}
