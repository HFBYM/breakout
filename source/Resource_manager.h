#pragma once
#include<map>
#include<string>
#include"Shader.h"
#include"Texture.h"
class ResourceManager //创建一个资源管理器，为纹理和着色器等导入文件 是一个静态单一实例的类 全过程可使用
{
private:
	ResourceManager(){}		//这样让这个类不能实例化 只能直接使用静态函数
public:
	//存储
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name);
	static Shader loadShader(const GLchar* file, std::string name);
	static Shader getShader(std::string name);
	static Texture2D loadTexture(const GLchar* file, GLboolean alpha, std::string name);	//alpha为0代表只有RGB
	static Texture2D& getTexture(std::string name);
	static void clear();
private:
	static Shader loadShaderFromFile(const GLchar* cShaderFile, const GLchar* fShaderFile);
	static Shader loadShaderFromFile(const GLchar* file);
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};