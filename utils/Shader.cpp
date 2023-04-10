#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// complie the shader
// ------------------------------------------------------------------------
void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
    GLuint sVertex, sFragment, gShader;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // If geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::use()
{
    glUseProgram(ID);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setInt(const GLchar* name, GLint value) const
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const GLchar* name, GLfloat value) const
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const GLchar* name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void Shader::setVec2(const GLchar* name, GLfloat x, GLfloat y) const
{
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const GLchar* name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void Shader::setVec3(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) const
{
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const GLchar* name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void Shader::setVec4(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const GLchar* name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}
// ------------------------------------------------------------------------
void Shader::setMat3(const GLchar* name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}
// ------------------------------------------------------------------------
void Shader::setMat4(const GLchar* name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

// Utility functions (string ver)
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, GLint value) const
{
    setInt(name.c_str(), value);
}
void Shader::setFloat(const std::string& name, GLfloat value) const
{
    setFloat(name.c_str(), value);
}
void Shader::setVec2(const std::string& name, GLfloat x, GLfloat y) const
{
    Shader::setVec2(name.c_str(), x, y);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    setVec2(name.c_str(), value);
}
void Shader::setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
{
    setVec3(name.c_str(), x, y, z);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    setVec3(name.c_str(), value);
}
void Shader::setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    setVec4(name.c_str(), x, y, z, w);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    setVec4(name.c_str(), value);
}
void Shader::setMat2(const std::string& name, const glm::mat2& matrix) const
{
    setMat2(name.c_str(), matrix);
}
void Shader::setMat3(const std::string& name, const glm::mat3& matrix) const
{
    setMat3(name.c_str(), matrix);
}
void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const
{
    setMat4(name.c_str(), matrix);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}