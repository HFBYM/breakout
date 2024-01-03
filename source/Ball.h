#pragma once
#include"Object.h"
class Ball : public Object	//继承必要属性 只新加几个
{
public:
	GLfloat radius;	//半径
	GLboolean isStuck;	//是否被固定在挡板上

	Ball();
	Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(GLfloat dt, GLuint window_width, glm::vec2);	//用于检测何时反转速度 与键盘输入不同
	void reset(glm::vec2 pos, glm::vec2 velocity);		//重置
};