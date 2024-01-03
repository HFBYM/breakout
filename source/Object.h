#pragma once
#include<glm\glm.hpp>
#include"Debug.h"
#include"Texture.h"
#include"SpriteRenderer.h"
class Object	//��Ϸ������ж�����������������  ʹ���������ע���������Զ�������Ⱦ�߼�
{
public:	
	//ȷ���������ݺ�����
	glm::vec2 pos, size, velocity;	//�ٶ�
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid;		//�Ƿ�ɴݻ� 
	GLboolean destroyed;	//�Ƿ��ѱ��ݻ�

	//��Ⱦ״̬
	Texture2D tex_sprite;
	Object();
	Object(glm::vec2 pos, glm::vec2 size, Texture2D tex, glm::vec3 color = glm::vec3(1.0f),
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f));	//���ֳ�ʼ����ʽ
	virtual void draw(SpriteRenderer &renderer);	//�麯�� ��������
};