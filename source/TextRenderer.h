#pragma once
#include<map>
#include"Texture.h"
#include"Shader.h"
struct Character {
	GLuint tex;			//���������id
	glm::ivec2 size;	//�����С
	glm::ivec2 bearing;	//��׼�ߵ�������/������ƫ��ֵ
	GLuint distance;	//ԭ�㵽�¸���ԭ��ľ���
};
class TextRenderer
{
public:
	std::map<char, Character> characters;
	Shader text_shader;		//ר�Ÿ��������ɫ��

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font, GLuint font_size);
	void renderText(std::string  text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint vao, vbo;
};