#pragma once
#include<map>
#include<vector>
#include"Texture.h"
#include"Shader.h"
struct Character {
	GLuint tex;			//字形纹理的id
	glm::ivec2 size;	//字体纹理大小
	glm::ivec2 bearing;	//基准线到字型左/顶部的偏移值
	GLuint distance;	//原点到下个字原点的距离
};
using char_lib = std::map<std::string, std::map<char, Character>>;	//这个要放在类的后面
class TextMethod
{
public:
	GLfloat xpos;
	GLfloat ypos;
	glm::vec3 color;
	void virtual update(GLfloat dt) = 0;	//一个纯虚函数用于继承
	void virtual reset() = 0;
};
class TextMethod_1:public TextMethod		//专门给defeat用的运动轨迹
{
public:
	TextMethod_1();
	GLfloat omiga;		//旋转的角速度 单位是弧度
	GLfloat time;		//从零开始计时
	void virtual update(GLfloat dt);
	void virtual reset();
};
class TextMethod_2 :public TextMethod		//更好的实现???
{
public:
	TextMethod_2();
	GLfloat omiga;		//旋转的角速度 单位是弧度
	GLfloat time;		//从零开始计时
	void virtual update(GLfloat dt);
	void virtual reset();
};
class TextMethod_3 :public TextMethod		//更好的实现???
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
	Shader text_shader;		//专门给字体的着色器
	TextMethod_1 method_1;
	TextMethod_2 method_2;
	TextMethod_3 method_3;

	TextRenderer(GLuint width, GLuint height);
	void load(std::string font_file, GLuint font_size, std::string font_name);
	void renderText(std::string  text, GLfloat x, GLfloat y, std::string font_name,
		glm::vec3 color = glm::vec3(1.0f), GLfloat scale = 3.0f);	//自主选择字体类型 放大倍数恒定更清晰
	void renderText(std::string  text, GLfloat xpos, GLfloat ypos, 
		TextMethod& method, std::string font_name, GLfloat scale = 3.0f);
			//自主选择字体类型 放大倍数恒定更清晰
	void update(GLfloat dt);
	void reset();
private:
	GLuint vao, vbo;
};