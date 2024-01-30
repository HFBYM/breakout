#pragma once
#include<vector>
#include"Ball.h"
#include"PostProcess.h"
#include"Player.h"
#include"Object.h"
static const glm::vec2 VELOCITY(0.0f, 150.0f);
struct PowerUp :public Object	//???加个滑冰的debuff
{
public:
	enum BuffType {
		SPEED, STICKY, PASS_THROUGH, PAD_SIZE_INCREASE, CONFUSE, CHAOS
	};
	BuffType m_type;

	PowerUp(BuffType type, glm::vec3 color, glm::vec2 pos, Texture2D tex, glm::vec2 size)
		:Object(pos, size, tex, color, VELOCITY), m_type(type)
	{}
};
class BuffManager	//用于管理各个道具
{
private:
	enum Duration {		//枚举常量只能是整数
		T_PASS_THROUGH = 20, T_CHAOS = 5, T_CONFUSE = 5
	};
	enum Chance {	//增益和负面buff的概率
		BUFF = 75,
		DEBUFF = 50
	};
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
	void clear();
};