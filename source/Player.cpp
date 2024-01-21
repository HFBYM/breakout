#include"Player.h"
void Player::princessInput(GLfloat dt, GLboolean keys[1024], GLfloat width, GLfloat height)
{
	GLfloat velocity = this->velocity * dt;	//ʹ�ٶȸ�ƽ�� ������Ⱦʱ��Ӱ��
	if (keys[GLFW_KEY_A])
	{
		if (this->pos.x >= velocity)	//��ֹԽ��
			this->pos.x -= velocity;
		else
			this->pos.x = 0;
	}
	if (keys[GLFW_KEY_D])
	{
		if (this->pos.x < width - this->size.x)
			this->pos.x += velocity;
	}
	if (keys[GLFW_KEY_W])
	{
		if (this->pos.y > (height) / 2.0)
			this->pos.y -= velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		if (this->pos.y + velocity <= height - this->size.y)
			this->pos.y += velocity;
		else
			this->pos.y = height - this->size.y;
	}
}
void Player::reset(GLboolean& ball_isStuck, GLfloat width, GLfloat height)
{
	ball_isStuck = GL_TRUE;
	this->pos = glm::vec2(width / 2 - this->size.x / 2, height - this->size.y);
	this->velocity = player_velocity;
	this->size = player_size;
}
