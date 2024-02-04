#include"Player.h"
#include<iostream>
void Player::princessInput(std::pair<GLboolean, GLboolean> keys[1024])
{
	if (!icy)
	{
		if (keys[GLFW_KEY_A].first)	//两种情况对速度的改变是相同的 类似于冲量
		{
			this->key_vec += glm::vec2(-1.0f, 0.0f);
			this->first_press = GL_TRUE;
		}
		if (keys[GLFW_KEY_D].first)
		{
			this->key_vec += glm::vec2(1.0f, 0.0f);
			this->first_press = GL_TRUE;
		}
		if (keys[GLFW_KEY_W].first)
		{
			this->key_vec += glm::vec2(0.0f, -1.0f);
			this->first_press = GL_TRUE;
		}
		if (keys[GLFW_KEY_S].first)
		{
			this->key_vec += glm::vec2(0.0f, 1.0f);
			this->first_press = GL_TRUE;
		}
		if (this->first_press)
		{
			if (keys[GLFW_KEY_D].second)	//两种情况对速度的改变是相同的 类似于冲量
				this->key_vec += glm::vec2(-1.0f, 0.0f);
			if (keys[GLFW_KEY_A].second)
				this->key_vec += glm::vec2(1.0f, 0.0f);
			if (keys[GLFW_KEY_S].second)
				this->key_vec += glm::vec2(0.0f, -1.0f);
			if (keys[GLFW_KEY_W].second)
				this->key_vec += glm::vec2(0.0f, 1.0f);
		}
	}
	else
	{
		if (keys[GLFW_KEY_A].first)	//两种情况对加速度的改变是相同的
			key_vec += glm::vec2(-1.0f, 0.0f);
		if (keys[GLFW_KEY_D].first)
			key_vec += glm::vec2(1.0f, 0.0f);
		if (keys[GLFW_KEY_W].first)
			key_vec += glm::vec2(0.0f, -1.0f);
		if (keys[GLFW_KEY_S].first)
			key_vec += glm::vec2(0.0f, 1.0f);
		if (keys[GLFW_KEY_D].second)
			key_vec += glm::vec2(-1.0f, 0.0f);
		if (keys[GLFW_KEY_A].second)
			key_vec += glm::vec2(1.0f, 0.0f);
		if (keys[GLFW_KEY_S].second)
			key_vec += glm::vec2(0.0f, -1.0f);
		if (keys[GLFW_KEY_W].second)
			key_vec += glm::vec2(0.0f, 1.0f);
	}
}
void Player::update(GLfloat dt, GLfloat width, GLfloat height)
{
	if (!icy)
	{
		if (this->key_vec != glm::vec2(0.0f))	//保证基础速度不为0
			this->velocity = glm::normalize(this->key_vec) * this->velocity_num;
		else										//基础速度为0
		{
			this->velocity = glm::vec2(0.0f);
			return;
		}
	}
	else
	{
		if (this->key_vec != glm::vec2(0.0f))	//加速度不为0
			this->accelerate = glm::normalize(this->key_vec) * this->accelerate_num;
		else
			this->accelerate = glm::vec2(0.0f);

		if (this->velocity != glm::vec2(0.0f))	//速度不为0 加速度是不是0无所谓
		{
			this->velocity += (this->accelerate - glm::normalize(this->velocity) * this->friction) * dt;
			if (glm::length(this->velocity) < 0.01f * this->friction)		//设置精度下限
				this->velocity = glm::vec2(0.0f);
		}
		else if (this->accelerate != glm::vec2(0.0f))	//速度为0但加速度不为0
			this->velocity = (this->accelerate - glm::normalize(this->accelerate) * this->friction) * dt;
		else                 //速度和加速度都为0
			return;
		if (glm::length(this->velocity) > (this->accelerate_num + this->friction) / 2)	//速度有上限
			this->velocity = glm::normalize(this->velocity) * ((this->accelerate_num + this->friction) / 2);
	}
	this->pos += this->velocity * dt;

	if (this->pos.x < 0)
		this->pos.x = 0;
	else if (this->pos.x + this->size.x > width)
		this->pos.x = width - this->size.x;
	else;
	if (this->pos.y < height / 2)
		this->pos.y = height / 2;
	else if (this->pos.y > height - this->size.y)
		this->pos.y = height - this->size.y;
	else
		return;
}
void Player::reset(GLboolean& ball_isStuck, GLfloat width, GLfloat height)
{
	ball_isStuck = GL_TRUE;
	this->pos = glm::vec2(width / 2 - this->size.x / 2, height - this->size.y);
	this->velocity_num = player_velocity;
	this->size = player_size;
	this->first_press = GL_FALSE;
	this->key_vec = glm::vec2(0.0f);
	this->accelerate = glm::vec2(0.0f);
	this->velocity = glm::vec2(0.0f);
}
