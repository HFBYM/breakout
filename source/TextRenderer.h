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
	void virtual reset() = 0;
};
class TextMethod_1:public TextMethod		//ר�Ÿ�defeat�õ��˶��켣
{
public:
	TextMethod_1();
	GLfloat omiga;		//��ת�Ľ��ٶ� ��λ�ǻ���
	GLfloat time;		//���㿪ʼ��ʱ
	void virtual update(GLfloat dt);
	void virtual reset();
};
class TextMethod_2 :public TextMethod		//���õ�ʵ��???
{
public:
	TextMethod_2();
	GLfloat omiga;		//��ת�Ľ��ٶ� ��λ�ǻ���
	GLfloat time;		//���㿪ʼ��ʱ
	void virtual update(GLfloat dt);
	void virtual reset();
};
class TextMethod_3 :public TextMethod		//���õ�ʵ��???
{
public:
	TextMethod_3();
	GLuint i;
	void virtual update(GLfloat dt);
	void virtual reset();
};

class TextRenderer
{
public:
	char_lib m_lib;
	Shader text_shader;		//ר�Ÿ��������ɫ��
	TextMethod_1 method_1;
	TextMethod_2 method_2;
	TextMethod_3 method_3;

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font_file, GLuint font_size, std::string font_name);
	void renderText(std::string  text, GLfloat x, GLfloat y, std::string font_name,
		glm::vec3 color = glm::vec3(1.0f), GLfloat scale = 3.0f);	//����ѡ���������� �Ŵ����㶨������
	void renderText(std::string  text, GLfloat xpos, GLfloat ypos, 
		TextMethod& method, std::string font_name, GLfloat scale = 3.0f);
			//����ѡ���������� �Ŵ����㶨������
	void update(GLfloat dt);
	void reset();
private:
	GLuint vao, vbo;
};