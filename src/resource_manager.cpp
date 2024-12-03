#include"resource_manager.h"
#include<fstream>
#include<sstream>
#include<stb.image.h>
#include<map>
#include<glad.h>
#include"shader.h"
#include"texture.h"
#include"mString.h"
#include"check.h"
// lib for math
//#include<glm.hpp>
//#include<gtc\type_ptr.hpp>
//#include<gtc/matrix_transform.hpp>
#define CHECK_STATUS() ASSERT_LOG(isInit, "ERROR::RESOURCE: " << __FUNCTION__ << " operate resource manager not initialized"); \
	ASSERT_LOG(!isClear, "ERROR::RESOURCE: " << __FUNCTION__ << " operate resource manager cleared")

//initialize the static virants
static std::map<mString, Texture2D*> textures;
static std::map<mString, Shader*> shaders;
static bool isInit = false;
static bool isClear = false;

static void loadShader(const mString& file, const mString& name)
{
	std::ifstream ifs(file.getStr());
	ASSERT_LOG(ifs.is_open(), "ERROR::SHADER: failed to open " << name.getStr());
	std::string line;
	std::stringstream ss[2];

	// use enum type for classification
	enum Type {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	Type type = NONE;

	// record if anything is found
	bool v_found = false, f_found = false;
	while (std::getline(ifs, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = VERTEX;
				v_found = true;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = FRAGMENT;
				f_found = true;
			}
			// if the type is find just skip the turn cause it the "#shader" couldn't be compiled
			continue;
		}
		//remember the endl which shader needs but ignored by getline
		ss[type] << line << std::endl;
	}
	ASSERT_LOG(v_found&& f_found, "ERROR::SHADER: syntex error: " << (v_found ? NULL : "no vertex shader ")
		<< (f_found ? NULL : "no fragment shader ") << "named: " << name.getStr() << " in\t"
		<< file.getStr());
	shaders[name] = new Shader(name);
	shaders[name]->compile(ss[VERTEX].str().c_str(), ss[FRAGMENT].str().c_str());
}

/// @brief load texture2D from file
/// @param file filepath
/// @param has_alpha decides whether or not this texture has alpha channel
/// @param name the texture name
static void loadTexture(const mString& file, bool has_alpha, const mString& name)
{
	textures[name] = new Texture2D(name, has_alpha ? GL_RGBA : GL_RGB, has_alpha ? GL_RGBA : GL_RGB);

	// usually we need to turn up and down the texture cause OpenGL renders from the leftdown while
	// picture loaded from leftup. But it isn't here because the proj matrix has already turned
	// stbi_set_flip_vertically_on_load(1);

	int width, height;

	// the forth parm returns the channels of the image which is of no use
	unsigned char* image = stbi_load(file.getStr(), &width, &height, 0,
		textures[name]->getImage_Format() == GL_RGB ? 3 : 4);

	textures[name]->generate(width, height, image);

	// the data in CPU is useless after sended to GPU
	stbi_image_free(image);
}

Shader& ResourceManager::getShader(const mString& name)
{
	CHECK_STATUS();
	ASSERT_LOG(shaders.find(name) != shaders.end(),
		"ERROR::GET_SHADER: failed to find the shader " << name.getStr());
	return *shaders[name];
}
Texture2D& ResourceManager::getTexture(const mString& name)
{
	CHECK_STATUS();
	ASSERT_LOG(textures.find(name) != textures.end(),
		"ERROR::GET_TEXTURE: failed to find the texture " << name.getStr());
	return *textures[name];
}
void ResourceManager::init()
{
	ASSERT_LOG(!isInit, "ERROR::RESOURCE: resource manager should be initialized only once");
	isInit = true;
	CHECK_STATUS();
	//??json文件管理
	//load in shaders
	loadShader(PROJECT_DIR"/assets/shaders/postProcess.shader", "postProcess");
	loadShader(PROJECT_DIR"/assets/shaders/sprite.shader", "sprite");

	loadShader(PROJECT_DIR"/assets/shaders/particle.shader", "particle");
	//ResourceManager::getShader("particle").use().setInteger("sprite", 0);	//设定采样槽
	//ResourceManager::getShader("particle").use().setMatrix4("proj", proj);	//设置正射投影

	//load in textures
	loadTexture(PROJECT_DIR"/assets/textures/background.jpg", false, "background");
	loadTexture(PROJECT_DIR"/assets/textures/block.png", false, "block");
	loadTexture(PROJECT_DIR"/assets/textures/block_solid.png", false, "block_solid");
	loadTexture(PROJECT_DIR"/assets/textures/basketball.png", true, "basketball");
	loadTexture(PROJECT_DIR"/assets/textures/paddle.png", true, "paddle");

	loadTexture(PROJECT_DIR"/assets/textures/particle.png", true, "particle");

	loadTexture(PROJECT_DIR"/assets/textures/powerup_chaos.png", false, "tex_chaos");
	loadTexture(PROJECT_DIR"/assets/textures/powerup_confuse.png", false, "tex_confuse");
	loadTexture(PROJECT_DIR"/assets/textures/powerup_increase.png", false, "tex_size");
	loadTexture(PROJECT_DIR"/assets/textures/powerup_passthrough.png", false, "tex_pass");
	loadTexture(PROJECT_DIR"/assets/textures/powerup_speed.png", false, "tex_speed");
	loadTexture(PROJECT_DIR"/assets/textures/powerup_sticky.png", false, "tex_sticky");
}
void ResourceManager::clear()
{
	CHECK_STATUS();
	isClear = true;
	for (auto& iter : shaders)
	{
		iter.second->clear();
		delete iter.second;
	}
	for (auto& iter : textures)
	{
		iter.second->clear();
		delete iter.second;
	}
	shaders.clear();
	textures.clear();
}

mString ResourceManager::readFile(const mString& file)
{
	CHECK_STATUS();
	std::ifstream ifs(file.getStr());
	ASSERT_LOG(ifs.is_open(), "ERROR::FILE: failed to open " << file.getStr());

	std::string line;
	std::stringstream ss;

	while (std::getline(ifs, line))
		ss << line << std::endl;

	return ss.str().c_str();
}
