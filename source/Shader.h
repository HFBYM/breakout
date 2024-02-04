#pragma once
#include<string>
#include<GLAD\glad.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>                       //��ѧ��
#include<glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Shader
{
public:
	GLuint id;   //�����Լ������ı���

	Shader():id(0) {}
	Shader& use();   //�����Լ�����
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