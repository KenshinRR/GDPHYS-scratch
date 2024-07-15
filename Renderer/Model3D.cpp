#include "Model3D.h"

Model3D::Model3D(glm::vec3 position)
{
	this->position = position;
	this->rotation = { 0,0,0 };
	this->scale = { 1.0f,1.0f,1.0f };
    theta_x = 1;
    theta_y = 0;
    theta_z = 0;

    /* Initialize the identity matrix */
    this->identity_matrix4 = glm::mat4(1.0f);

    this->color = { 1.0,0.0,0.0 };
    
    
}

void Model3D::setPos(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Model3D::setColor(float x, float y, float z) {
    this->color = glm::vec3(x, y, z);
}

void Model3D::setScale(float x, float y, float z)
{
    this->scale = glm::vec3(x, y, z);
}





void Model3D::draw(Shader* shaderProg, GLuint* VAO, std::vector<GLfloat>* fullVertexData)
{
    //start with the translation matrix
    glm::mat4 transformation_matrix = glm::translate(
        identity_matrix4,
        glm::vec3(position.x, position.y, position.z)
    );

    //multiply the resultin matrix with the scale
    transformation_matrix = glm::scale(
        transformation_matrix,
        glm::vec3(scale.x, scale.y, scale.z)
    );

    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta_x),
        glm::normalize(glm::vec3(1, 0, 0))
    );

    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta_y),
        glm::normalize(glm::vec3(0, 1, 0))
    );

    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta_x),
        glm::normalize(glm::vec3(0, 0, 1))
    );

    //setting the transformation
    shaderProg->setMat4("transform", transformation_matrix);

    //setting COLOR
    shaderProg->setVec4("color", glm::vec4(this->color, 0.0f));
    


    //tell open GL to use this shader for the VAOs below
    shaderProg->use();

    //bind the VAO to prep for drawing
    glBindVertexArray(*VAO);

    //drawing
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData->size() / 8);
}

