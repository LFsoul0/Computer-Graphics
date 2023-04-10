#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    // State
    GLuint ID;
    // Constructor
    Shader(): ID(0) { }
    // Compiles the shader from given source code
    void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional 
    // Sets the current shader as active
    void use();
    // Utility functions
    void setInt(const GLchar* name, GLint value) const;
    void setFloat(const GLchar* name, GLfloat value) const;
    void setVec2(const GLchar* name, GLfloat x, GLfloat y) const;
    void setVec2(const GLchar* name, const glm::vec2& value) const;
    void setVec3(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) const;
    void setVec3(const GLchar* name, const glm::vec3& value) const;
    void setVec4(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
    void setVec4(const GLchar* name, const glm::vec4& value) const;
    void setMat2(const GLchar* name, const glm::mat2& matrix) const;
    void setMat3(const GLchar* name, const glm::mat3& matrix) const;
    void setMat4(const GLchar* name, const glm::mat4& matrix) const;
    // Utility functions (string ver)
    void setInt(const std::string& name, GLint value) const;
    void setFloat(const std::string& name, GLfloat value) const;
    void setVec2(const std::string& name, GLfloat x, GLfloat y) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat2(const std::string& name, const glm::mat2& matrix) const;
    void setMat3(const std::string& name, const glm::mat3& matrix) const;
    void setMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    // Checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(GLuint shader, std::string type);
};

