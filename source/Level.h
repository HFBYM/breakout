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
	std::vector<Brick> bricks;	//砖块
	glm::vec2 unit_size;		//砖块的长宽
	GLuint lives;				//每个关卡的生命值

	Level():lives(0){}
	void load(const char* file, GLuint width, GLuint height);	//从文件中加载关卡
	void draw(SpriteRenderer& renderer);
	GLboolean isCompleted();	//检测关卡是否完成 即所有可被破坏的都破坏完
private:
	//初始化砖块数据
	void init(std::vector<std::vector<GLuint>> tileData, GLuint width, GLuint height);
};