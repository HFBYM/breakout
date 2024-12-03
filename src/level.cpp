#include"level.h"
#include"Resource_manager.h"
#include"check.h"
#define CHECK_STATUS() 	ASSERT_LOG(isInit, "ERROR::LEVEL: function " << __FUNCTION__ << " uses level "<< id << "not initialized");\
	ASSERT_LOG(!isClear, "ERROR::LEVEL: function " << __FUNCTION__ << " uses level "<< id << "cleared")

static void load(const mString& path, std::vector<std::vector<unsigned int>>& tileData)
{
	mString file = ResourceManager::readFile(path);

	std::vector<unsigned int> row;
	char c = NULL;
	for (unsigned int i = 0; i < file.getSize(); i++)
	{
		c = file[i];
		if (isspace(c)) {
			if (c == '\n') {
				tileData.push_back(row);
				row.clear();
			}
			continue;
		}
		row.push_back(c - 48);
	}
	ASSERT_LOG(tileData.size() > 1, "ERROR::LEVEL: file " << path.getStr() << " has wrong data");
}
Level::~Level()
{
	ASSERT_LOG(isClear, "ERROR::LEVEL:  level " << id << " not cleared");
}
void Level::init(const mString& path, unsigned int levelWidth, unsigned int levelHeight)
{
	ASSERT_LOG(!isInit, "ERROR::LEVEL: level " << id << " is initialized one more time");
	ASSERT_LOG(!isClear, "ERROR::LEVEL: function " << __FUNCTION__ << " uses level " << id << "cleared");
	isInit = true;

	std::vector<std::vector<unsigned int>> tileData;
	load(path, tileData);

	// get the number of lives in the last line
	this->lives = tileData[tileData.size() - 1][0];

	// get rid of it in the bricks
	tileData.pop_back();

	// calculate the param
	unsigned int num_in_height = static_cast<unsigned int>(tileData.size());	
	unsigned int num_in_width = static_cast<unsigned int>(tileData[0].size());
	float unit_width = (levelWidth / static_cast<float>(num_in_width));	
	float unit_height = (levelHeight / static_cast<float>(num_in_height));

	for (unsigned int y = 0; y < num_in_height; y++)
	{
		for (unsigned int x = 0; x < num_in_width; x++)
		{
			// the leftup is origin
			glm::vec2 pos(unit_width * x, unit_height * y);	
			glm::vec2 size(unit_width, unit_height);
			if (tileData[y][x] == Brick::SOLID)
			{
				this->bricks.push_back({ pos, size, "block_solid", Brick::SOLID, true, 
					glm::vec3(0.8f, 0.8f, 0.7f) });
			}
			else if (tileData[y][x] == Brick::NONE)
				continue;
			else
			{
				glm::vec3 color(1.0f);
				switch (tileData[y][x])
				{
				case Brick::BLUE:	color = glm::vec3(0.2f, 0.6f, 1.0f);	break;
				case Brick::GREEN:	color = glm::vec3(0.0f, 0.7f, 0.0f);	break;
				case Brick::YELLOW:	color = glm::vec3(0.8f, 0.8f, 0.4f);	break;
				case Brick::RED:	color = glm::vec3(1.0f, 0.5f, 0.0f);	break;
				default:
					ERROR_LOG("ERROR::LEVEL: unnamed type of brick in " << path.getStr());
				}
				this->bricks.push_back({ pos,size,"block",static_cast<Brick::BrickType>(tileData[y][x]), 
					false, color });
			}
		}
	}
}

void Level::log_renderer()
{
	CHECK_STATUS();
	for (auto& i : bricks)
		i.log_renderer();
}

void Level::detach_renderer()
{
	CHECK_STATUS();
	for (auto& i : bricks)
		i.detach_renderer();
}

void Level::clear()
{
	CHECK_STATUS();
	isClear = true;
	for (auto& i : bricks)
		i.detach_all();
}

void Level::Rotate()
{
	for (auto& i : bricks)
	{
		i.Rotate();
	}
}
