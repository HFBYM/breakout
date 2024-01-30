#pragma once
#include<vector>
#include"Object.h"
class Brick :public Object
{
public:
	enum BrickType {
		NONE=0, SOLID, BLUE, GREEN, YELLOW, RED
	};
	BrickType type = NONE;

	Brick(glm::vec2 pos, glm::vec2 size, Texture2D tex, BrickType type, glm::vec3 color = glm::vec3(1.0f),
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f)):Object(pos,size,tex,color,velocity), type(type){}
};
class Level
{
public:
	std::vector<Brick> bricks;	//ש��
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