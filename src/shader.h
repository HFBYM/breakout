#pragma once
#include "mString.h"
// ±‡“Î ±ºÏ≤Èuniform??
class Shader
{
public:
    Shader(const mString& name):m_name(name){}
    Shader& operator=(Shader&& other) = delete;

    /// @brief each shader should be clear manually
    ~Shader();

    /// @brief return *this and bind the shader
    Shader& use();

    /// @brief a shader must be compiled and only once
    void compile(const mString& vertexSource, const mString& fragmentSouce);

    /// @brief delete the shader program which must be a compiled one and it can't clear one more time
    void clear();

    /// @brief if Uniform can't be found, program will break down.  
    //after setting the uniform, the shader would be of use
    template<typename T>
    void setUniform(const mString& name, const T& value);
private:    
    // the id of the shader
    unsigned int id = 0;

    bool is_compiled = false;
    bool isClear = false;
    mString m_name;

    void checkCompileErrors(unsigned int object, const mString& type);
};
