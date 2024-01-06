#pragma once
#include"Debug.h"
#include"Level.h"
#include"BuffManager.h"
enum GameState		//代表游戏当前状态
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};
enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};
class Game
{
public:
	//游戏状态
	GameState state;
	GLboolean keys[1024];       //用于处理输入
	GLuint init_screen_width, init_screen_height;		//游戏初始分辨率
	GLuint screen_width, screen_height;		//游戏后动态的分辨率
	std::vector<Level> levels;	//游戏关卡
	GLuint level;				//当前关卡
	GLuint player_lives;		//玩家生命值
	//析构与构造
	Game(GLuint width, GLuint height);
	~Game();

	void doCollisions();
	void init();       //加载着色器和纹理
	//游戏循环
	void princessInput(GLfloat dt);		//处理输入
	void update(GLfloat dt);	//更新游戏状态
	void render();				//渲染循环     将运动逻辑和渲染逻辑分开
	void resetLevel();
	void reset_screen_size(int width, int height);
};

