#pragma once
#include<string>
#include<GLAD\glad.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>                       //数学库
#include<glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Shader
{
public:
	GLuint id;   //用其自己别名的变量

	Shader():id(0) {}
	Shader& use();   //返回自己并绑定
	void compile(const GLchar* vertexSource, const GLchar* fragmentSouce);
	void setFloat(const GLchar* name, GLfloat val, GLboolean useShader = GL_FALSE);
    void setInteger(const GLchar* name, GLint value, GLboolean useShader = GL_FALSE);
    void setVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = GL_FALSE);
    void setVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = GL_FALSE);
    void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = GL_FALSE);
    void setVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = GL_FALSE);
    void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = GL_FALSE);
    void setVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = GL_FALSE);
    void setMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = GL_FALSE);
private:
    void checkCompileErrors(GLuint object, std::string type);
};