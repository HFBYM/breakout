#pragma once
#include<vector>
#include"Ball.h"
#include"PostProcess.h"
#include"Player.h"
#include"Object.h"
static const glm::vec2 VELOCITY(0.0f, 150.0f);
struct PowerUp :public Object	//???�Ӹ�������debuff
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
class BuffManager	//���ڹ����������
{
private:
	enum Duration {		//ö�ٳ���ֻ��������
		T_PASS_THROUGH = 20, T_CHAOS = 5, T_CONFUSE = 5
	};
	enum Chance {	//����͸���buff�ĸ���
		BUFF = 75,
		DEBUFF = 50
	};
public:
	std::vector<PowerUp> powerUps;
	GLfloat t_pass_through, t_chaos, t_confuse;		//����buff�ĳ���ʱ��

	BuffManager():t_pass_through(0), t_chaos(0), t_confuse(0)
	{}
	void spawnPowerUp(Object& block,glm::vec2 unit_size);	//��ָ��ש�����ɵ���
	void activatePowerUp(PowerUp& powerUp, Ball& ball, Player& player,PostProcessor& post_processor);
	void updatePowerUp(GLfloat dt, PostProcessor& post_processor, GLfloat height, glm::vec3& ball_color);
	void draw(SpriteRenderer& renderer);
	void reset(PostProcessor& post_processor, glm::vec3& ball_color);
	void clear();
};