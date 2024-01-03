#include<fstream>
#include<iostream>
#include<sstream>
#include"Level.h"
#include"Resource_manager.h"
void Level::load(const char* file, GLuint width, GLuint height)
{
	this->bricks.clear();	//清空之前数据

	GLuint tileCode;	//0是无 1是坚硬的 其他代表不同颜色
	std::string line;	//从文件中读取字符流
	std::ifstream ifs(file);
	std::vector<std::vector<GLuint>> tileData;
	if (ifs)
	{
		while (std::getline(ifs, line))	//不断从文件中读取一行
		{
			std::istringstream iss(line);	//将字符串转为流输入 相当于创建一个缓冲区
			std::vector<GLuint> row;
			while (iss >> tileCode)	//以空格分隔
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size())
			this->init(tileData, width, height);	//使用指针？？？
	}
	else
	{
		std::cout << "ERROR::LEVEL: fail to open level file" << std::endl;
		__debugbreak();
	}
}
void Level::draw(SpriteRenderer& renderer)
{
	for (auto& brick : this->bricks)
		if (!brick.destroyed)		//被破坏后不再生成
			brick.draw(renderer);
}
GLboolean Level::isCompleted()
{
	for (auto& brick : this->bricks)
		if (!brick.isSolid && !brick.destroyed)	//如果有一个可被摧毁的没有被摧毁 就未完成
			return GL_FALSE;
	return GL_TRUE;
}
void Level::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)//关卡像素
{
	//计算相应的数值
	GLuint height = static_cast<GLuint>(tileData.size());	//横向个数
	GLuint width = static_cast<GLuint>( tileData[0].size());
	GLuint unit_width = static_cast<GLuint>(levelWidth / static_cast<GLfloat>(width));	//单位方块的像素长度
	GLuint unit_height = static_cast<GLuint>(levelHeight / static_cast<GLfloat>(height));
	this->unit_size = glm::vec2(unit_width, unit_height);

	for (GLuint y = 0; y < height; y++)
	{
		for (GLuint x = 0; x < width; x++)
		{
			glm::vec2 pos(unit_width * x, unit_height * y);	//以左上角为坐标
			glm::vec2 size(unit_width, unit_height);
			if (tileData[y][x] == 1)
			{
				Object brick(pos, size, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				brick.isSolid = GL_TRUE;
				this->bricks.push_back(brick);
			}
			else if (tileData[y][x] == 0)	//0是空白 什么都不做
				continue;
			else
			{
				glm::vec3 color(1.0f);
				switch (tileData[y][x])
				{
				case 2:	color = glm::vec3(0.2f, 0.6f, 1.0f);	break;
				case 3:	color = glm::vec3(0.0f, 0.7f, 0.0f);	break;
				case 4:	color = glm::vec3(0.8f, 0.8f, 0.4f);	break;
				case 5:	color = glm::vec3(1.0f, 0.5f, 0.0f);	break;
				default:
					std::cout << "ERROR::LEVEL: unnamed type of brick" << std::endl;
					__debugbreak();
				}
					this->bricks.push_back({ pos,size,ResourceManager::getTexture("block"),color });
			}
		}
	}
}