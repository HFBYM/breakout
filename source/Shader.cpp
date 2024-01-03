#include"Shader.h"
#include<iostream>
Shader& Shader::use()
{
	glUseProgram(this->id);
	return *this;
}
void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER: compile_time error :type" << type << std::endl
                << infoLog << std::endl;
            __debugbreak();        //ֱ���жϺ�������
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM: link_time error :type" << type << std::endl
                << infoLog << std::endl;

        }
    }
}
void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSouce)
{
	GLuint sVertex, sFragment;
	sVertex = glCreateShader(GL_VERTEX_SHADER); //�õ�һ����ɫ����id
	glShaderSource(sVertex, 1, &vertexSource, NULL); //�Ѵ������
	glCompileShader(sVertex);                        //����
    this->checkCompileErrors(sVertex, "VERTEX");  //���������

	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSouce, NULL);
	glCompileShader(sFragment);
    this->checkCompileErrors(sFragment, "FRAGMENT");
	 
	//������ɫ������
	this->id = glCreateProgram();      //�õ�����id
	glAttachShader(this->id, sVertex); //������ɫ��
	glAttachShader(this->id, sFragment);
	glLinkProgram(this->id);           //���ӳ���
    this->checkCompileErrors(this->id, "PROGRAM");

	glDeleteShader(sVertex);    //���õ���ɫ����ɾ�� 
	glDeleteShader(sFragment);
}
void Shader::setFloat(const GLchar* name, GLfloat val, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform1f(glGetUniformLocation(this->id, name), val);
}
void Shader::setInteger(const GLchar* name, GLint value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
}
void Shader::setVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}
void Shader::setVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}
void Shader::setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}
void Shader::setVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}
void Shader::setVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}