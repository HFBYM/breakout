#pragma once
#include"Debug.h"
#include"Level.h"
#include"BuffManager.h"
#include<irrKlang/include/irrKlang.h>	//音乐库目录
#include"ParticleGenerator.h"
#include"PostProcess.h"
#include"Ball.h"
#include"Player.h"
#include"TextRenderer.h"
enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};
class Game
{
private:
	enum GameState		//代表游戏当前状态
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN,
		GAME_DEFEAT
	};
	//游戏状态
	GameState state;
	GLuint init_screen_width, init_screen_height;		//游戏初始分辨率
	GLuint screen_width, screen_height;		//游戏后动态的分辨率
	std::vector<Level> levels;	//游戏关卡
	GLuint level;				//当前关卡
	int player_lives;		//玩家生命值

	//工具类		尽量不要用全局变量
	irrklang::ISoundEngine* sound_engine = irrklang::createIrrKlangDevice();	//初始化
	SpriteRenderer* renderer = nullptr;
	Player* player = nullptr;
	Ball* ball = nullptr;
	ParticleGenerator* particles = nullptr;
	BuffManager* buff_manager = nullptr;
	PostProcessor* post_processor = nullptr;
	TextRenderer* text_renderer = nullptr;

	void doCollisions();
	void reset();
	void resetLevel();

public:			//public是给外部的接口 使用类时只许看public部分

	GLboolean keys[1024];       //用于处理输入
	std::pair<GLboolean, GLboolean> keys_status[1024];		//第一个是press 第二个是release 模拟冲量

	//析构与构造
	Game(GLuint width, GLuint height);
	~Game();
	void init();       //加载着色器和纹理

	//游戏循环
	void princessInput(GLfloat dt);		//处理输入
	void update(GLfloat dt);	//更新游戏状态
	void render();				//渲染循环     将运动逻辑和渲染逻辑分开
	void reset_screen_size(int width, int height);
};

