#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // the program ID
    unsigned int ID;

public:
    // constructor reads and builds the shader
    Shader(unsigned int shaderProg);
    Shader(const char* vertexPath, const char* fragmentPath);

public:
    // use/activate the shader
    void use();

public:
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;

public:
    unsigned int getID();

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};