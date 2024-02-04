#pragma once
#include"Object.h"
const glm::vec2 player_size = glm::vec2(100, 20);	//挡板大小
const GLfloat player_velocity = 550.0f;				//挡板速度
class Player :public Object
{
public:
	GLboolean first_press = GL_FALSE;
	GLfloat friction = 3000.0f;			//摩擦力
	GLfloat velocity_num;
	GLfloat accelerate_num = 6000.0f;	//基础加速度的大小
	glm::vec2 accelerate = glm::vec2(0.0f);
	glm::vec2 key_vec;					//用于采集键盘输入信息
	GLboolean icy = GL_FALSE;

	Player(glm::vec2 player_pos,Texture2D tex):
		Object(player_pos,player_size,tex), velocity_num(player_velocity){}
	void princessInput(std::pair<GLboolean, GLboolean> keys[1024]);
	void update(GLfloat dt, GLfloat width, GLfloat height);
	void reset(GLboolean& ball_isStuck, GLfloat width, GLfloat height);
};