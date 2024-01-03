#include"Debug.h"
#include<GLAD\glad.h>  //���ӵ�ͷ�ļ��Ϳ�·��Ӧ��C/C++���������������� glew����32λ�Ĳ���
#include<GLFW\glfw3.h> //�Ժ���û��ǲ�Ҫ���Ƹ��ӿ��Ĭ�ϰ�����ϵ
#include<iostream>
void clearError()	//ʹ�øú��� ������֮ǰ�����д��� ֻ��ע�������Ĵ���
{
	while (glGetError() != GL_NO_ERROR);
	//�ж��ִ����־ ��һ�������������ʱһЩ��־Ϊ�� glGetError���ش���ı�־��������Ϊ�޴���
}
void GLcall(const char* name, const char* file, int line)
{

}
void check(const char* file, int line)	//���ǰ��Ĵ���
{
	GLuint error;
	std::string type;
	while ((error = glGetError())!= GL_NO_ERROR)	//�޴�����0
	{
		switch(error)
		{
			case GL_INVALID_ENUM:                  type = "invalid_enum"; break;
			case GL_INVALID_VALUE:                 type = "invalid_value"; break;
			case GL_INVALID_OPERATION:             type = "invalid_operation"; break;
			case GL_OUT_OF_MEMORY:                 type = "out_of_memory"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: type = "invalid_framebuffer_operation"; break;
			default:	type = "unamed_error"; break;
	}
		std::cout << "ERROR::OPENGL:" << type << "\tFile:" << file << "\tLine:" << line << std::endl;
		__debugbreak();
	}
}