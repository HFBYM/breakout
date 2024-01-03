#pragma once
#include<vector>
#include"PowerUp.h"
#include"Ball.h"
#include"PostProcess.h"
#include"Player.h"
enum Duration {		//ö�ٳ���ֻ��������
	T_PASS_THROUGH = 20, T_CHAOS = 5, T_CONFUSE = 5
};
class BuffManager	//���ڹ����������
{
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
};