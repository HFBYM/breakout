#include"Shader.h"
#include<glad.h>	
#include<glfw3.h>	
#include<glm.hpp>
#include<gtc\type_ptr.hpp>
#include<gtc/matrix_transform.hpp>
#include"check.h"
#define CHECK_STATUS() ASSERT_LOG(is_compiled, "ERROR::SHADER: function " << __FUNCTION__ << " uses uncompiled shader " << m_name.getStr());\
    ASSERT_LOG(!isClear, "ERROR::SHADER: function " << __FUNCTION__ << " uses cleared shader "<< m_name.getStr())

Shader::~Shader()
{
    ASSERT_LOG(isClear, "ERROR::SHADER: shader " << m_name.getStr() << " isn't clear before distruction ");
}

Shader& Shader::use()
{
    CHECK_STATUS();
    glUseProgram(this->id);
        return *this;
}
void Shader::checkCompileErrors(unsigned int object, const mString& type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            ERROR_LOG("ERROR::SHADER: compile_time error :type" << type.getStr() << "name: " 
                << m_name.getStr() << std::endl << infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            ERROR_LOG("ERROR::PROGRAM: link_time error :type" << type.getStr() << "name: " 
                << m_name.getStr() << std::endl << infoLog)
        }
    }
}
void Shader::compile(const mString& vertexSource, const mString& fragmentSouce)
{
    ASSERT_LOG(!is_compiled, "ERROR::SHADER: shader " << m_name.getStr() << " is compiled one more time");
    this->is_compiled = true;
    CHECK_STATUS();

    // compile the vertex and fragment shader
    GLuint sVertex, sFragment;
    const char* temp_v = vertexSource.getStr();
    const char* temp_f = fragmentSouce.getStr();

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &temp_v, NULL); 
    glCompileShader(sVertex);
    this->checkCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &temp_f, NULL);
    glCompileShader(sFragment);
    this->checkCompileErrors(sFragment, "FRAGMENT");

    //link the shader program
    this->id = glCreateProgram();
    glAttachShader(this->id, sVertex);
    glAttachShader(this->id, sFragment);
    glLinkProgram(this->id);
    this->checkCompileErrors(this->id, "PROGRAM");

    //program is created so these is out of use
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}
void Shader::clear()
{
    CHECK_STATUS();
    isClear = true;
    glDeleteProgram(this->id);
    this->id = 0;
}

template<typename T>
void Shader::setUniform(const mString& name, const T& value)
{
    CHECK_STATUS();
    int pos = glGetUniformLocation(this->id, name.getStr());
    ASSERT_LOG(pos != -1, "ERROR::UNIFORM: " << name.getStr() << " can't be found in " << m_name.getStr());
    if constexpr (std::is_same_v<T, float>)
    {
        this->use();
        glUniform1f(pos, value);
    }
    else if constexpr (std::is_same_v<T, int>)
    {
        this->use();
        glUniform1i(pos, value);
    }
    else if constexpr (std::is_same_v<T, glm::vec2>)
    {
        this->use();
        glUniform2f(pos, value.x, value.y);
    }
    else if constexpr (std::is_same_v<T, glm::vec3>)
    {
        this->use();
        glUniform3f(pos, value.x, value.y, value.z);
    }
    else if constexpr (std::is_same_v<T, glm::vec4>)
    {
        this->use();
        glUniform4f(pos, value.x, value.y, value.z, value.w);
    }
    else if constexpr (std::is_same_v<T, glm::mat4>)
    {
        this->use();
        glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
    }
    else
        ERROR_LOG("ERROR::UNIFORM: wrong type of value for " << name.getStr() << " in " << m_name.getStr());
}

/// @brief need to instance the template
template void Shader::setUniform<int>(const mString& name, const int& value);
template void Shader::setUniform<float>(const mString& name, const float& value);
template void Shader::setUniform<glm::vec2>(const mString& name, const glm::vec2& value);
template void Shader::setUniform<glm::vec3>(const mString& name, const glm::vec3& value);
template void Shader::setUniform<glm::vec4>(const mString& name, const glm::vec4& value);
template void Shader::setUniform<glm::mat4>(const mString& name, const glm::mat4& value);