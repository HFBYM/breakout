#pragma once
#include<map>
#include<string>
#include"Shader.h"
#include"Texture.h"
class ResourceManager //����һ����Դ��������Ϊ�������ɫ���ȵ����ļ� ��һ����̬��һʵ������ ȫ���̿�ʹ��
{
private:
	ResourceManager(){}		//����������಻��ʵ���� ֻ��ֱ��ʹ�þ�̬����
public:
	//�洢
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name);
	static Shader loadShader(const GLchar* file, std::string name);
	static Shader getShader(std::string name);
	static Texture2D loadTexture(const GLchar* file, GLboolean alpha, std::string name);	//alphaΪ0����ֻ��RGB
	static Texture2D& getTexture(std::string name);
	static void clear();
private:
	static Shader loadShaderFromFile(const GLchar* cShaderFile, const GLchar* fShaderFile);
	static Shader loadShaderFromFile(const GLchar* file);
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};