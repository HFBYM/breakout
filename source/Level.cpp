#include<fstream>
#include<iostream>
#include<sstream>
#include"Level.h"
#include"Resource_manager.h"
void Level::load(const char* file, GLuint width, GLuint height)
{
	this->bricks.clear();	//���֮ǰ����

	GLuint tileCode;	//0���� 1�Ǽ�Ӳ�� ��������ͬ��ɫ
	std::string line;	//���ļ��ж�ȡ�ַ���
	std::ifstream ifs(file);
	std::vector<std::vector<GLuint>> tileData;
	if (ifs)
	{
		while (std::getline(ifs, line))	//���ϴ��ļ��ж�ȡһ��
		{
			std::istringstream iss(line);	//���ַ���תΪ������ �൱�ڴ���һ��������
			std::vector<GLuint> row;
			while (iss >> tileCode)	//�Կո�ָ�
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size())
			this->init(tileData, width, height);	//ʹ��ָ�룿����
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
		if (!brick.destroyed)		//���ƻ���������
			brick.draw(renderer);
}
GLboolean Level::isCompleted()
{
	for (auto& brick : this->bricks)
		if (!brick.isSolid && !brick.destroyed)	//�����һ���ɱ��ݻٵ�û�б��ݻ� ��δ���
			return GL_FALSE;
	return GL_TRUE;
}
void Level::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)//�ؿ�����
{
	//������Ӧ����ֵ
	GLuint height = static_cast<GLuint>(tileData.size());	//�������
	GLuint width = static_cast<GLuint>( tileData[0].size());
	GLuint unit_width = static_cast<GLuint>(levelWidth / static_cast<GLfloat>(width));	//��λ��������س���
	GLuint unit_height = static_cast<GLuint>(levelHeight / static_cast<GLfloat>(height));
	this->unit_size = glm::vec2(unit_width, unit_height);

	for (GLuint y = 0; y < height; y++)
	{
		for (GLuint x = 0; x < width; x++)
		{
			glm::vec2 pos(unit_width * x, unit_height * y);	//�����Ͻ�Ϊ����
			glm::vec2 size(unit_width, unit_height);
			if (tileData[y][x] == 1)
			{
				Object brick(pos, size, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				brick.isSolid = GL_TRUE;
				this->bricks.push_back(brick);
			}
			else if (tileData[y][x] == 0)	//0�ǿհ� ʲô������
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