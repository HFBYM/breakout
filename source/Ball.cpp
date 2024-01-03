#include"Ball.h"
Ball::Ball():Object(),radius(12.5f),isStuck(true)	//用于子类的初始化
{

}
Ball::Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite):	//双构造
Object(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), isStuck(true), radius(radius)
{

}
glm::vec2 Ball::move(GLfloat dt, GLuint window_width,glm::vec2 pos)
{
	if (!this->isStuck)	//不固定的时候才可以移动
	{
		this->pos += this->velocity * dt;	//与键盘输入不同 设计惯性？？？
		if (this->pos.x <= 0.0f)
		{
			this->pos.x = 0.0f;
			this->velocity.x = -this->velocity.x;
		}
		else if (this->pos.x >= window_width - this->size.x)
		{
			this->pos.x = window_width - this->size.x;
			this->velocity.x = -this->velocity.x;
		}
		if (this->pos.y <= 0)
		{
			this->pos.y = 0;
			this->velocity.y = -this->velocity.y;
		}
	}
	else		//固定的时候
		this->pos = pos;
	return this->pos;
}
void Ball::reset(glm::vec2 pos, glm::vec2 velocity)
{
	this->pos = pos;
	this->velocity = velocity;
	this->isStuck = true;
}
