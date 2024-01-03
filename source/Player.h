#pragma once
#include"Object.h"
const glm::vec2 player_size(100, 20);	//挡板大小
const GLfloat player_velocity(500.0f);	//挡板速度
class Player :public Object
{
public:
	GLfloat velocity;
	Player(glm::vec2 player_pos,Texture2D tex):
		Object(player_pos,player_size,tex),velocity(player_velocity)	{}
	void princessInput(GLfloat dt, GLboolean keys[1024], GLfloat width, GLfloat height);
	void reset(GLboolean& ball_isStuck, GLfloat width, GLfloat height);
};