#pragma once
#include<vector>
#include"Object.h"
class Level
{
public:
	std::vector<Object> bricks;	//ש��
	glm::vec2 unit_size;		//ש��ĳ���
	GLuint lives;				//ÿ���ؿ�������ֵ

	Level():lives(0){}
	void load(const char* file, GLuint width, GLuint height);	//���ļ��м��عؿ�
	void draw(SpriteRenderer& renderer);
	GLboolean isCompleted();	//���ؿ��Ƿ���� �����пɱ��ƻ��Ķ��ƻ���
private:
	//��ʼ��ש������
	void init(std::vector<std::vector<GLuint>> tileData, GLuint width, GLuint height);
};