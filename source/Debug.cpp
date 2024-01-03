#include"Debug.h"
#include<GLAD\glad.h>  //附加的头文件和库路径应在C/C++和链接器里面配置 glew库是32位的不符
#include<GLFW\glfw3.h> //以后最好还是不要打破附加库的默认包含关系
#include<iostream>
void clearError()	//使用该函数 将忽略之前的所有错误 只关注接下来的错误
{
	while (glGetError() != GL_NO_ERROR);
	//有多种错误标志 当一个或多个错误出现时一些标志为真 glGetError返回错误的标志并将其置为无错误
}
void GLcall(const char* name, const char* file, int line)
{

}
void check(const char* file, int line)	//检查前面的错误
{
	GLuint error;
	std::string type;
	while ((error = glGetError())!= GL_NO_ERROR)	//无错误是0
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