#pragma once
#include<vector>
#include"Object.h"
class Level
{
public:
	std::vector<Object> bricks;	//砖块
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