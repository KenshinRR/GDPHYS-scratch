#include "Model3D.h"

Model3D::Model3D(glm::vec3 position,
    const char* vertexPath,
    const char* fragmentPath)
{
	this->position = position;
	this->rotation = { 0,0,0 };
	this->scale = { 0.1f,0.1f,0.1f };
    theta_x = 0.f;
    theta_y = 0.f; 
    theta_z = 0.f;

    /* Initialize the identity matrix */
    this->identity_matrix4 = glm::mat4(1.0f);

    this->shaderProg = new Shader(vertexPath, fragmentPath);
}

Model3D::Model3D(glm::vec3 position,
    std::vector<GLfloat> fullVertexData,
    const char* vertexPath, const char* fragmentPath,float rot)
{
    this->position = position;
    this->rotation = { 0,0,0 };
    this->scale = { 0.1f,0.1f,0.1f };
    theta_x = 0.f;
    theta_y = rot;
    theta_z = 0.f;

    /* Initialize the identity matrix */
    this->identity_matrix4 = glm::mat4(1.0f);

    this->fullVertexData = fullVertexData;

    this->shaderProg = new Shader(vertexPath, fragmentPath);
}

void Model3D::draw()
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
        glm::radians(theta_z),
        glm::normalize(glm::vec3(0, 0, 1))
    );

    //setting the transformation
    this->shaderProg->setMat4("transform", transformation_matrix);

    //tell open GL to use this shader for the VAOs below
    this->shaderProg->use();

    //bind the VAO to prep for drawing
    glBindVertexArray(this->VAO);

    //drawing
    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 14);
}

void Model3D::draw(Shader* shaderProgB, GLuint* VAOB, std::vector<GLfloat>* fullVertexDataB)
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
        glm::radians(-90.f),
        glm::normalize(glm::vec3(0, 0, 1))
    );

    //setting the transformation
    shaderProgB->setMat4("transform", transformation_matrix);

    //tell open GL to use this shader for the VAOs below
    shaderProgB->use();

    //bind the VAO to prep for drawing
    glBindVertexArray(*VAOB);

    //drawing
    glDrawArrays(GL_TRIANGLES, 0, fullVertexDataB->size() / 14);
}

void Model3D::rotate(char axis, char direction)
{
    switch (direction)
    {
    
    case '+':
        this->theta_y += 7.5;
        break;
    case '-':
        this->theta_y -= 7.5;
        break;
    
    default:
        break;
    }
}

void Model3D::initVAO()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        //
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,

        14 * sizeof(float),
        (void*)0

    );
    glEnableVertexAttribArray(0);

    GLintptr litPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,

        14 * sizeof(float),
        (void*)litPtr

    );
    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,

        14 * sizeof(float),
        (void*)uvPtr
    );
    glEnableVertexAttribArray(2);

    //tangent point
    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(
        3, //3 == tangent
        3, //T (XYZ)
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)tangentPtr
    );
    glEnableVertexAttribArray(3);

    //bitangent 
    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(
        4, //4 == bitangent
        3, //B(XYZ)
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)bitangentPtr
    );
    glEnableVertexAttribArray(4);
}

void Model3D::deleteVAO()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model3D::setFullVertexData(std::vector<GLfloat> fullVertexData)
{
    this->fullVertexData = fullVertexData;
}

void Model3D::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

Shader* Model3D::getShader()
{
    return this->shaderProg;
}

unsigned int Model3D::getShaderID()
{
    return this->shaderProg->getID();
}

float Model3D::getYrot()
{
    return this->theta_y;
}

glm::vec3 Model3D::getPosition()
{
    return this->position;
}


