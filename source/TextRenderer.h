#pragma once
#include<map>
#include"Texture.h"
#include"Shader.h"
struct Character {
	GLuint tex;			//字形纹理的id
	glm::ivec2 size;	//字体纹理大小
	glm::ivec2 bearing;	//基准线到字型左/顶部的偏移值
	GLuint distance;	//原点到下个字原点的距离
};
using char_lib = std::map<std::string, std::map<char, Character>>;
class TextRenderer
{
public:
	char_lib m_lib;
	Shader text_shader;		//专门给字体的着色器

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font_file, GLuint font_size, std::string font_name);
	void renderText(std::string  text, GLfloat x, GLfloat y, std::string font_name,
		glm::vec3 color = glm::vec3(1.0f), GLfloat scale = 3.0f);	//自主选择字体类型 放大倍数恒定更清晰
private:
	GLuint vao, vbo;
};