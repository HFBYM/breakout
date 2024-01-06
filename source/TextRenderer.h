#pragma once
#include<map>
#include"Texture.h"
#include"Shader.h"
struct Character {
	GLuint tex;			//���������id
	glm::ivec2 size;	//���������С
	glm::ivec2 bearing;	//��׼�ߵ�������/������ƫ��ֵ
	GLuint distance;	//ԭ�㵽�¸���ԭ��ľ���
};
using char_lib = std::map<std::string, std::map<char, Character>>;
class TextRenderer
{
public:
	char_lib m_lib;
	Shader text_shader;		//ר�Ÿ��������ɫ��

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font_file, GLuint font_size, std::string font_name);
	void renderText(std::string  text, GLfloat x, GLfloat y, std::string font_name,
		glm::vec3 color = glm::vec3(1.0f), GLfloat scale = 3.0f);	//����ѡ���������� �Ŵ����㶨������
private:
	GLuint vao, vbo;
};