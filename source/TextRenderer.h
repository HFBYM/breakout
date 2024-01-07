#pragma once
#include<map>
#include<vector>
#include"Texture.h"
#include"Shader.h"
struct Character {
	GLuint tex;			//���������id
	glm::ivec2 size;	//���������С
	glm::ivec2 bearing;	//��׼�ߵ�������/������ƫ��ֵ
	GLuint distance;	//ԭ�㵽�¸���ԭ��ľ���
};
using char_lib = std::map<std::string, std::map<char, Character>>;	//���Ҫ������ĺ���
class TextMethod
{
public:
	GLfloat xpos;
	GLfloat ypos;
	glm::vec3 color;
	void virtual update(GLfloat dt) = 0;	//һ�����麯�����ڼ̳�
};
class TextMethod_1:public TextMethod		//ר�Ÿ�defeat�õ��˶��켣
{
public:
	TextMethod_1();
	GLfloat omiga;		//��ת�Ľ��ٶ� ��λ�ǽǶ�
	GLfloat time;		//���㿪ʼ��ʱ
	void virtual update(GLfloat dt);
};
class TextRenderer
{
public:
	char_lib m_lib;
	Shader text_shader;		//ר�Ÿ��������ɫ��
	TextMethod_1 method_1;

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font_file, GLuint font_size, std::string font_name);
	void renderText(std::string  text, GLfloat x, GLfloat y, std::string font_name,
		glm::vec3 color = glm::vec3(1.0f), GLfloat scale = 3.0f);	//����ѡ���������� �Ŵ����㶨������
	void renderText(std::string  text, TextMethod& method, std::string font_name,
		glm::vec3 color = glm::vec3(1.0f), GLfloat scale = 3.0f);	//����ѡ���������� �Ŵ����㶨������
	void update(GLfloat dt);
private:
	GLuint vao, vbo;
};