#include<fstream>
#include<sstream>
#include<iostream>
#include"Resource_manager.h"
#include<stb_image\stb.image.h>		//ͼ���뺯��
#include"Debug.h"

//��ʼ����̬����
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
	std::string vCode, fCode;
	try 
	{
		std::ifstream vertexShader(vShaderFile);		//���ļ�
		std::ifstream fragmentShader(fShaderFile);
		std::stringstream vsstream, fsstream;

		vsstream << vertexShader.rdbuf();		//��һ�����е���Ϣ�ض�����һ������
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
	if (!ifs.is_open())		//����ļ���ʧ��
	{
		std::cout << "ERROR::SHADER: failed to open " << file << std::endl;
		__debugbreak();
	}
	std::string line;
	std::stringstream ss[2];
	enum Type {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};	//��ö�ٳ�������¼״̬
	Type type = NONE;
	bool v_found = GL_FALSE, f_found = GL_FALSE;		//��¼�Ƿ��ҵ���
	while (std::getline(ifs, line))		//���϶�ȡ��
	{
		if (line.find("#shader") != std::string::npos)	//����ֵ���ǿ�
		{
			if (line.find("vertex") != std::string::npos)		//�ҵ�������ɫ����һ��
			{
				type = VERTEX;
				v_found = GL_TRUE;
			}
			if (line.find("fragment") != std::string::npos)		//Ƭ����ɫ����һ��
			{
				type = FRAGMENT;
				f_found = GL_TRUE;
			}
			continue;		//����
		}
		if (type != NONE)
			ss[type] << line << std::endl;	//ע������س� getline��ȡ�س������� ����ɫ����Ҫ�س�
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
	shader.compile(ss[VERTEX].str().c_str(), ss[FRAGMENT].str().c_str());	//�ȱ��string�ٱ��C����ַ���
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
	//stbi_set_flip_vertically_on_load(1);       //��ת���� ��ΪOpenGL�����½ǿ�ʼ ��ͼƬ���ش����Ͻǿ�ʼ
	int width, height;
	unsigned char* image = stbi_load(file, &width, &height, 0,
		texture.image_format == GL_RGB ? 3 : 4);
	//����λ����ԴͼƬ�е�ͨ���� �ò���
	Check();
	texture.generate(width, height, image);
	stbi_image_free(image);

	return texture;    //����GPU������ֻ��id����
}
Texture2D ResourceManager::loadTexture(const GLchar* file, GLboolean alpha, std::string name)//alpha �ǲ�͸����
{
	Check();
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
	Check();
}
Texture2D &ResourceManager::getTexture(std::string name)
{
	if(textures.find(name)!=textures.end())	//�ɱ��ҵ�
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
