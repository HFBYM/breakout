#include<iostream>
#include"BuffManager.h"
#include"Resource_manager.h"		//静态的整个sln共享
inline GLboolean shouldSpawn(GLuint chance)	//1/chance的几率
{
	return (rand() % chance) == 0;	//就用rand函数
}
void BuffManager::spawnPowerUp(Object& block, glm::vec2 unit_size)		//一直塞用完后摧毁才是最优解
{
	if (shouldSpawn(BUFF)) // 1/75的几率  
		this->powerUps.push_back(		
			PowerUp(PowerUp::SPEED, glm::vec3(0.5f, 0.5f, 1.0f), block.pos,		//即场景方块高的一半
				ResourceManager::getTexture("tex_speed"), glm::vec2(unit_size.x, unit_size.y / 2)));
	else if (shouldSpawn(BUFF))
		this->powerUps.push_back(
			PowerUp(PowerUp::STICKY, glm::vec3(1.0f, 0.5f, 1.0f), block.pos,
				ResourceManager::getTexture("tex_sticky"), glm::vec2(unit_size.x, unit_size.y / 2)));
	else if (shouldSpawn(BUFF))
		this->powerUps.push_back(
			PowerUp(PowerUp::PASS_THROUGH, glm::vec3(0.5f, 1.0f, 0.5f), block.pos,
				ResourceManager::getTexture("tex_pass"), glm::vec2(unit_size.x, unit_size.y / 2)));
	else if (shouldSpawn(BUFF))
		this->powerUps.push_back(
			PowerUp(PowerUp::PAD_SIZE_INCREASE, glm::vec3(1.0f, 0.6f, 0.4f), block.pos,
				ResourceManager::getTexture("tex_size"), glm::vec2(unit_size.x, unit_size.y / 2)));
	else if (shouldSpawn(DEBUFF))		// 负面道具
		this->powerUps.push_back(
			PowerUp(PowerUp::CONFUSE, glm::vec3(1.0f, 0.3f, 0.3f), block.pos,
				ResourceManager::getTexture("tex_confuse"), glm::vec2(unit_size.x, unit_size.y / 2)));
	else if (shouldSpawn(DEBUFF))
		this->powerUps.push_back(
			PowerUp(PowerUp::CHAOS, glm::vec3(0.9f, 0.25f, 0.25f), block.pos,
				ResourceManager::getTexture("tex_chaos"), glm::vec2(unit_size.x, unit_size.y / 2)));
	else
		return;
}
void BuffManager::activatePowerUp(PowerUp& powerUp,Ball& ball, Player& player, PostProcessor& post_processor)
	//根据道具类型实现相应效果
{
	switch (powerUp.m_type)
	{
	case PowerUp::SPEED:		player.velocity *= 1.2f;	break;	//????生成疾风效果
	case PowerUp::STICKY:	ball.isStuck = GL_TRUE;	break;
	case PowerUp::PAD_SIZE_INCREASE:	player.size.x += 50;	break;
	case PowerUp::PASS_THROUGH:	this->t_pass_through += T_PASS_THROUGH;	//增加时间
		ball.color = glm::vec3(1.0f, 0.5f, 0.5f);	break;		//穿透为红色
	case PowerUp::CONFUSE:
		if (!post_processor.chaos)
		{
			post_processor.confuse = GL_TRUE;
			this->t_confuse += T_CONFUSE;	//触发一次就增加持续时间
		}
		break;
	case PowerUp::CHAOS:
		if (!post_processor.confuse)
		{
			post_processor.chaos = GL_TRUE;
			this->t_chaos += T_CHAOS;
		}
		break;
	default:	std::cout << "ERROR::BUFF: invalid type of buff" << std::endl;
		__debugbreak();
		break;
	}
}
void BuffManager::updatePowerUp(GLfloat dt, PostProcessor& post_processor,GLfloat height, glm::vec3& ball_color)
{
	if (this->t_chaos > 0)
	{
		this->t_chaos -= dt;
		if (this->t_chaos <= 0)		//保证是在变化的时候关闭buff
		{
			this->t_chaos = 0;
			post_processor.chaos = GL_FALSE;
		}
	}
	if (this->t_confuse > 0)
	{
		this->t_confuse -= dt;
		if (this->t_confuse <= 0)		//保证是在变化的时候关闭buff
		{
			this->t_confuse = 0;
			post_processor.confuse = GL_FALSE;
		}
	}
	if (this->t_pass_through > 0)
	{
		this->t_pass_through -= dt;
		if (this->t_pass_through <= 0)		//保证是在变化的时候关闭buff
		{
			this->t_pass_through = 0;
			ball_color = glm::vec3(1.0f);
		}
	}
	for (PowerUp& powerUp : this->powerUps)
	{
		if (!powerUp.destroyed)
		{
			powerUp.pos += powerUp.velocity * dt;
			if (powerUp.pos.y > height)		//超过屏幕就摧毁
				powerUp.destroyed = GL_TRUE;
		}
	}
	this->powerUps.erase(std::remove_if(this->powerUps.begin(), this->powerUps.end(),
		[](const PowerUp& powerUp) {return powerUp.destroyed ; }), this->powerUps.end());
	//只需要销毁超出屏幕的道具
}
void BuffManager::draw(SpriteRenderer& renderer)
{
	for (auto& powerUp : this->powerUps)	//画道具
		if (!powerUp.destroyed)
			powerUp.draw(renderer);

}
void BuffManager::reset(PostProcessor& post_processor, glm::vec3& ball_color)
{
	if (this->t_chaos > 0)
	{
		this->t_chaos = 0;
		post_processor.chaos = GL_FALSE;
	}
	if (this->t_confuse > 0)
	{
		this->t_confuse = 0;
		post_processor.confuse = GL_FALSE;
	}
	if (this->t_pass_through > 0)
	{
		this->t_pass_through = 0;
		ball_color = glm::vec3(1.0f);
	}
	//for (PowerUp& powerUp : this->powerUps)
	//{
	//	if (!powerUp.destroyed)
	//		powerUp.destroyed = GL_TRUE;
	//}
}
void BuffManager::clear()
{
	this->powerUps.clear(); 
}
