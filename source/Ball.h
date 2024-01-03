#pragma once
#include"Object.h"
class Ball : public Object	//�̳б�Ҫ���� ֻ�¼Ӽ���
{
public:
	GLfloat radius;	//�뾶
	GLboolean isStuck;	//�Ƿ񱻹̶��ڵ�����

	Ball();
	Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(GLfloat dt, GLuint window_width, glm::vec2);	//���ڼ���ʱ��ת�ٶ� ��������벻ͬ
	void reset(glm::vec2 pos, glm::vec2 velocity);		//����
};