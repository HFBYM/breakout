#pragma once
#include<vector>
#include"PowerUp.h"
#include"Ball.h"
#include"PostProcess.h"
#include"Player.h"
enum Duration {		//枚举常量只能是整数
	T_PASS_THROUGH = 20, T_CHAOS = 5, T_CONFUSE = 5
};
class BuffManager	//用于管理各个道具
{
public:
	std::vector<PowerUp> powerUps;
	GLfloat t_pass_through, t_chaos, t_confuse;		//各个buff的持续时间

	BuffManager():t_pass_through(0), t_chaos(0), t_confuse(0)
	{}
	void spawnPowerUp(Object& block,glm::vec2 unit_size);	//在指定砖块生成道具
	void activatePowerUp(PowerUp& powerUp, Ball& ball, Player& player,PostProcessor& post_processor);
	void updatePowerUp(GLfloat dt, PostProcessor& post_processor, GLfloat height, glm::vec3& ball_color);
	void draw(SpriteRenderer& renderer);
	void reset(PostProcessor& post_processor, glm::vec3& ball_color);
};