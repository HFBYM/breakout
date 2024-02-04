#include<fstream>
#include<sstream>
#include<iostream>
#include"Resource_manager.h"
#include<stb_image\stb.image.h>		//图像导入函数
#include"Debug.h"

//初始化静态变量
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
	std::string vCode, fCode;
	try 
	{
		std::ifstream vertexShader(vShaderFile);		//打开文件
		std::ifstream fragmentShader(fShaderFile);
		std::stringstream vsstream, fsstream;

		vsstream << vertexShader.rdbuf();		//将一个流中的信息重定向到另一个流中
		fsstream << fragmentShader.rdbuf();

		vertexShader.close();
		fragmentShader.close();

		vCode = vsstream.str();
		fCode = fsstream.str();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER failed to open files" << std::endl;
		__debugbreak();
	}
	Shader shader;
	shader.compile(vCode.c_str(), fCode.c_str());
	return shader;
}
Shader ResourceManager::loadShaderFromFile(const GLchar* file)
{
	std::ifstream ifs(file);
	if (!ifs.is_open())		//如果文件打开失败
	{
		std::cout << "ERROR::SHADER: failed to open " << file << std::endl;
		__debugbreak();
	}
	std::string line;
	std::stringstream ss[2];
	enum Type {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};	//用枚举常量来记录状态
	Type type = NONE;
	bool v_found = GL_FALSE, f_found = GL_FALSE;		//记录是否被找到过
	while (std::getline(ifs, line))		//不断读取行
	{
		if (line.find("#shader") != std::string::npos)	//返回值不是空
		{
			if (line.find("vertex") != std::string::npos)		//找到顶点着色器第一行
			{
				type = VERTEX;
				v_found = GL_TRUE;
			}
			if (line.find("fragment") != std::string::npos)		//片段着色器第一行
			{
				type = FRAGMENT;
				f_found = GL_TRUE;
			}
			continue;		//跳过
		}
		if (type != NONE)
			ss[type] << line << std::endl;	//注意塞入回车 getline读取回车并丢弃 而着色器需要回车
		else
		{
			std::cout << "ERROR::SHADER: syntex error: no type of the shader\t" << file << std::endl;
			__debugbreak();
		}
	}
	if (!v_found || !f_found)
	{
		std::cout << "ERROR::SHADER: syntex error: no type of the shader\t" << file << std::endl;
		__debugbreak();
	}
	Shader shader;
	shader.compile(ss[VERTEX].str().c_str(), ss[FRAGMENT].str().c_str());	//先变成string再变成C风格字符串
	return shader;
}
Shader ResourceManager::getShader(std::string name)
{
	if(shaders.find(name)!=shaders.end())
		return shaders[name];
	else
	{
		std::cout << "ERROR::GET_SHADER: failed to find the shader named " << name << std::endl;
		__debugbreak();
	}
}
Shader ResourceManager::loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name)
{
	Check();
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return shaders[name];
}
Shader ResourceManager::loadShader(const GLchar* file, std::string name)
{
	shaders[name] = loadShaderFromFile(file);
	return shaders[name];
}
Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha)
{
	Texture2D texture;
	if (alpha)
	{
		texture.internal_format = GL_RGBA;
		texture.image_format = GL_RGBA;
	}
	//stbi_set_flip_vertically_on_load(1);       //反转纹理 因为OpenGL从左下角开始 而图片加载从左上角开始
	int width, height;
	unsigned char* image = stbi_load(file, &width, &height, 0,
		texture.image_format == GL_RGB ? 3 : 4);
	//第四位返回源图片中的通道数 用不上
	Check();
	texture.generate(width, height, image);
	stbi_image_free(image);

	return texture;    //导入GPU后这里只有id有用
}
Texture2D ResourceManager::loadTexture(const GLchar* file, GLboolean alpha, std::string name)//alpha 是不透明度
{
	Check();
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
	Check();
}
Texture2D &ResourceManager::getTexture(std::string name)
{
	if(textures.find(name)!=textures.end())	//可被找到
		return textures[name];
	else
	{
		std::cout << "ERROR::GET_TEXTURE: failed to find the texture named " << name << std::endl;
		__debugbreak();
	}
}
void ResourceManager::clear()
{
	for (auto& iter : shaders)
		glDeleteProgram(iter.second.id);
	for (auto& iter : textures)
		glDeleteTextures(1, &iter.second.id);
}
