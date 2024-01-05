#pragma once
#include<map>
#include"Texture.h"
#include"Shader.h"
struct Character {
	GLuint tex;			//字形纹理的id
	glm::ivec2 size;	//字体大小
	glm::ivec2 bearing;	//基准线到字型左/顶部的偏移值
	GLuint distance;	//原点到下个字原点的距离
};
class TextRenderer
{
public:
	std::map<char, Character> characters;
	Shader text_shader;		//专门给字体的着色器

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font, GLuint font_size);
	void renderText(std::string  text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint vao, vbo;
};