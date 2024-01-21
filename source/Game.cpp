#include"Game.h"
#include"SpriteRenderer.h"
#include"Resource_manager.h"	//静态的好处 包含即可更改
#include"Ball.h"
#include"ParticleGenerator.h"
#include"PostProcess.h"
#include"Player.h"
#include<irrKlang/include/irrKlang.h>	//音乐库目录
#include"TextRenderer.h"
const GLuint level_num = 5;

using Collision = std::tuple<GLboolean, Direction, glm::vec2>;	//类结构体 用std::get<i>(val)访问

using namespace irrklang;		//使用命名空间
ISoundEngine* sound_engine = createIrrKlangDevice();	//初始化声音引擎

SpriteRenderer* renderer;

Player* player;

const glm::vec2 ball_velocity(200.0f, -450.0f);
const GLfloat ball_radius(25.0f);
Ball* ball;

ParticleGenerator* particles;		//?????物体破碎时的粒子效果????

BuffManager* buff_manager;

PostProcessor* post_processor;
GLfloat shake_time = 0.0f;

TextRenderer* text_renderer;
Direction getDirect(glm::vec2 target)	//得到一个向量的大致方向	???不够准确
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,1.0f),	//y+	上
		glm::vec2(1.0f,0.0f),	//x+	右
		glm::vec2(0.0f,-1.0f),	//y-	下
		glm::vec2(-1.0f,0.0f)	//x-	左
	};
	GLfloat max = 0.0f;
	GLuint best = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat ret = glm::dot(glm::normalize(target), compass[i]);
		if (ret > max)
		{
			max = ret;
			best = i;
		}
	}
	return (Direction)best;
}
GLboolean checkCollisions(Object& one, Object& two)		
{
	//AABB-AABB		x方向和y方向的碰撞
	bool collisionX = one.pos.x + one.size.x >= two.pos.x && two.pos.x + two.size.x >= one.pos.x;
	bool collisionY = one.pos.y + one.size.y >= two.pos.y && two.pos.y + two.size.y >= one.pos.y;
	return collisionX && collisionY;
}
Collision checkCollisions(Ball& one, Object& two)
{
	// 获取圆的中心 
	glm::vec2 center(one.pos + one.radius);
	// 计算AABB的信息（中心、半边长）
	glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
	glm::vec2 aabb_center(
		two.pos.x + aabb_half_extents.x,
		two.pos.y + aabb_half_extents.y
	);
	// 获取两个中心的差矢量
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);	//????
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	glm::vec2 closest = aabb_center + clamped;
	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
	difference = closest - center;
	if (glm::length(difference) < one.radius)	//发生碰撞	该模型本就不是很精确
		return { GL_TRUE,getDirect(difference),difference };	//得到差矢用于重定位球
	else
		return{ GL_FALSE,UP,glm::vec2(0.0f) };
}

Game::Game(GLuint width, GLuint height):state(GAME_MENU),screen_width(width),screen_height(height),keys(),
level(3),init_screen_width(width), init_screen_height(height),player_lives(0)
{
	//可以空白初始化变量
}
Game::~Game()
{
	if(renderer)
		delete renderer;
}
void Game::init()	//进行所有资源的导入
{
	Check();
	//装入着色器
	ResourceManager::loadShader("resource/shaders/postProcess.shader", "postProcess");
	ResourceManager::loadShader("resource/shaders/sprite.shader", "sprite");

	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(this->init_screen_width), 
		static_cast<GLfloat>(this->init_screen_height), 0.0f, -1.0f, 1.0f);		
	//正射投影即可 左上角是(0,0)与屏幕坐标对应	旋转中心在左上角???上下翻转??
	ResourceManager::getShader("sprite").use().setInteger("image", 0);	//设定采样槽
	ResourceManager::getShader("sprite").use().setMatrix4("proj", proj);	//设置正射投影

	ResourceManager::loadShader("resource/shaders/particle.shader", "particle");
	ResourceManager::getShader("particle").use().setInteger("sprite", 0);	//设定采样槽
	ResourceManager::getShader("particle").use().setMatrix4("proj", proj);	//设置正射投影
	
	//装入纹理
	ResourceManager::loadTexture("resource/textures/CTRL.jpg", GL_TRUE, "cxk");
	ResourceManager::loadTexture("resource/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("resource/textures/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("resource/textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::loadTexture("resource/textures/basketball.png", GL_TRUE, "basketball");
	ResourceManager::loadTexture("resource/textures/paddle.png", GL_TRUE, "paddle");

	ResourceManager::loadTexture("resource/textures/particle.png", GL_TRUE, "particle");

	ResourceManager::loadTexture("resource/textures/powerup_chaos.png", GL_FALSE, "tex_chaos");
	ResourceManager::loadTexture("resource/textures/powerup_confuse.png", GL_FALSE, "tex_confuse");
	ResourceManager::loadTexture("resource/textures/powerup_increase.png", GL_FALSE, "tex_size");
	ResourceManager::loadTexture("resource/textures/powerup_passthrough.png", GL_FALSE, "tex_pass");
	ResourceManager::loadTexture("resource/textures/powerup_speed.png", GL_FALSE, "tex_speed");
	ResourceManager::loadTexture("resource/textures/powerup_sticky.png", GL_FALSE, "tex_sticky");

	//载入关卡
	for (unsigned int i = 0; i < level_num; i++)
	{
		Level level;
		std::string str = "resource/levels/level_" + std::to_string(i + 1) + ".lvl";
		level.load(str.c_str(), this->init_screen_width,
			static_cast<GLuint>(this->init_screen_height * 0.5));
		this->levels.push_back(level);
	}

	renderer = new SpriteRenderer (ResourceManager::getShader("sprite").use());	//直接拿到精灵的渲染

	glm::vec2 player_pos = glm::vec2(this->init_screen_width / 2 - player_size.x / 2, 
		this->init_screen_height - player_size.y);
	player = new Player(player_pos, ResourceManager::getTexture("paddle"));
	glm::vec2 ball_pos = player_pos + glm::vec2(player_size.x / 2 - ball_radius, -ball_radius * 2);
	ball = new Ball(ball_pos, ball_radius, ball_velocity, ResourceManager::getTexture("basketball"));

	particles = new ParticleGenerator(ResourceManager::getShader("particle"),
		ResourceManager::getTexture("particle"), 500);	//总共500个粒子

	post_processor = new PostProcessor(ResourceManager::getShader("postProcess"),
		this->init_screen_width, this->init_screen_height);

	buff_manager = new BuffManager;

	sound_engine->play2D("resource/music/breakout.mp3", GL_TRUE);	//重复播放

	text_renderer = new TextRenderer(this->init_screen_width, this->init_screen_height);
	text_renderer->load("resource/font/ARIALNI.TTF", 20, "medium");	//导入相对路径arialbi.ttf
	text_renderer->load("resource/font/arialbi.ttf", 40, "bold");	//用于标题
	Check();
}
void Game::doCollisions()
{
	for (auto& brick : this->levels[this->level].bricks)
	{
		if (!brick.destroyed)	//未被摧毁的
		{
			Collision ret = checkCollisions(*ball, brick);
			if (std::get<0>(ret))	//发生碰撞
			{
				if(!brick.isSolid)	//可摧毁的
				{
					sound_engine->play2D("resource/music/solid.wav", GL_FALSE);	//不重复播放
					brick.destroyed = true;
					buff_manager->spawnPowerUp(brick,this->levels[this->level].unit_size);	
					//对每一个被摧毁的做一次生成判定
					if (buff_manager->t_pass_through!=0.0f)		//如果有穿越效果就直接穿过
						continue;
				}
				else	//坚固的就抖动
				{
					sound_engine->play2D("resource/music/bleep.mp3", GL_FALSE);	//不重复播放
					shake_time = 0.05f;
					post_processor->shake = GL_TRUE;
				}
				Direction dir = std::get<1>(ret);
				glm::vec2 difference = std::get<2>(ret);
				if (dir == RIGHT || dir == LEFT)	//水平碰撞
				{
					ball->velocity.x *= -1;
					//重定位	???	仅仅是一个小平移 不精确也够用
					GLfloat penetration = ball->radius - glm::length(difference.x);
					if (dir == LEFT)
						ball->pos.x -= penetration;	
					else
						ball->pos.x += penetration;
				}
				else
				{
					ball->velocity.y *= -1;  // 反转垂直速度
					GLfloat penetration = ball->radius - glm::length(difference.y);
					if (dir == UP)
						ball->pos.y -= penetration;	//上移
					else
						ball->pos.y += penetration;
				}
			}
		}
	}
	Collision ret = checkCollisions(*ball, *player);
	if (!ball->isStuck && std::get<0>(ret))		//未被固定且与挡板发生碰撞
	{
		GLfloat distance = ball->pos.x + ball->radius - (player->pos.x + player->size.x / 2);
		GLfloat percentage = distance / (player->size.x / 2);	//碰撞点距离中心的百分比	可以为负的
		sound_engine->play2D("resource/music/bleep.wav", GL_FALSE);	//不重复播放

		//速度更改
		GLfloat strenghth = 2.0f;
		ball->velocity.y = -glm::abs(ball->velocity.y);	//永远返回一个向上的速度 防止粘板 ???但会使下方碰撞出问题
		ball->velocity.x = ball_velocity.x * percentage * strenghth;	//x方向速度更改	使x方向有一些基本速度
		ball->velocity = glm::normalize(ball->velocity) * glm::length(ball_velocity);	//保持速度大小不变
	}
	for (auto& powerUp : buff_manager->powerUps)
	{
		if (checkCollisions(*player, powerUp))	//挡板与道具相撞
			{   
			//激活道具
			sound_engine->play2D("resource/music/powerup.wav", GL_FALSE);	//不重复播放
			buff_manager->activatePowerUp(powerUp,*ball,*player,*post_processor);
				powerUp.destroyed = GL_TRUE;
			}
	}
}
void Game::princessInput(GLfloat dt)	//板的速度与其他物体速度不同
{
	if (this->state == GAME_ACTIVE)	//运行时
	{
		player->princessInput(dt, this->keys, static_cast<GLfloat>(this->init_screen_width),
			static_cast<GLfloat>(this->init_screen_height));
		if (this->keys[GLFW_KEY_SPACE])
			ball->isStuck = false;
		if (this->keys[GLFW_KEY_P])		//???调试使用
			__debugbreak();
	}
	else if (this->state == GAME_DEFEAT)
	{
		if (this->keys[GLFW_KEY_ENTER])		//再来一次
		{
			this->resetLevel();
			this->reset();
			post_processor->blurry = GL_FALSE;
		}
		if (this->keys[GLFW_KEY_BACKSPACE])
			this->state = GAME_MENU;
	}
	else if (this->state == GAME_MENU)
	{
		if (this->keys[GLFW_KEY_1])
		{
			this->level = 0;
			this->reset();
			post_processor->blurry = GL_FALSE;
		}
		else if (this->keys[GLFW_KEY_2])
		{
			this->level = 1;
			this->reset();
			post_processor->blurry = GL_FALSE;
		}
		else if (this->keys[GLFW_KEY_3])
		{
			this->level = 2;
			this->reset();
			post_processor->blurry = GL_FALSE;
		}
		else if (this->keys[GLFW_KEY_4])
		{
			this->level = 3;
			this->reset();
			post_processor->blurry = GL_FALSE;
		}
		else if (this->keys[GLFW_KEY_5])
		{
			this->level = 4;
			this->reset();
			post_processor->blurry = GL_FALSE;
		}
		else;
	}
	else if (this->state == GAME_WIN)
	{
		if (this->keys[GLFW_KEY_BACKSPACE])
			this->state = GAME_MENU;
		if (this->level + 1 < level_num)
		{
			if (this->keys[GLFW_KEY_ENTER])
			{
				this->level++;
				this->reset();
			}
		}
	}
	else
		return;
}
void Game::update(GLfloat dt)	//用于更新内部的运动	每次循环需要运行的代码
{
	if (this->state == GAME_ACTIVE)
	{
		//更新
		ball->move(dt, this->init_screen_width, player->pos + glm::vec2(player->size.x / 2 - ball_radius,
			-2 * ball_radius));
		buff_manager->updatePowerUp(dt, *post_processor, static_cast<GLfloat>(this->init_screen_height), 
			ball->color);
		particles->update(dt, *ball, 2, glm::vec2(ball->radius * 4 / 5));	
		//一次激活两个 offset让粒子在球里面出现

		this->doCollisions();
		//检查是否碰到底边
		if (ball->pos.y >= this->init_screen_height)
		{
			this->player_lives--;
			if (this->player_lives > 0)
				ball->isStuck = GL_TRUE;
			else		//失败
			{
				this->state = GAME_DEFEAT;
				post_processor->blurry = GL_TRUE;	//启用失败时模糊效果
				buff_manager->reset(*post_processor, ball->color);	//终止所有的道具效果
			}
		}
		if (this->levels[this->level].isCompleted())	//成功通过关卡
		{
			this->state = GAME_WIN;
			buff_manager->reset(*post_processor, ball->color);	//终止所有的道具效果
		}

		if (shake_time > 0.0f)	//将循环的时间与实现的时间分开		???放到发生器里面
		{
			shake_time -= dt;	//1s失去1
			if (shake_time <= 0.0f)		//放在里面防止多次设定
				post_processor->shake = GL_FALSE;
		}
	}
	else if (this->state == GAME_DEFEAT || this->state == GAME_WIN || this->state == GAME_MENU)
	{
		text_renderer->update(dt);
	}
	else
		return;
}
void Game::render()
{
	Check();
	post_processor->beginRender();
	if (this->state == GAME_ACTIVE)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//背景
		this->levels[this->level].draw(*renderer);	//关卡
		player->draw(*renderer);		//画挡板
		buff_manager->draw(*renderer);	//画道具
		particles->draw();		//画粒子在其他之后 在球之前 因为没有深度检测
		ball->draw(*renderer);	//画球

		text_renderer->renderText("Lives " + std::to_string(this->player_lives), 0, 0, "medium");
	}
	else if (this->state == GAME_DEFEAT)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//背景
		this->levels[this->level].draw(*renderer);	//关卡
		player->draw(*renderer);		//画挡板
		buff_manager->draw(*renderer);	//画道具
		particles->draw();		//画粒子在其他之后 在球之前 因为没有深度检测

		text_renderer->renderText("Press Enter to Restart",
			150, 400, text_renderer->method_2, "medium");
		text_renderer->renderText("Press Backspace to Back to Menu",
			25, 450, text_renderer->method_2, "medium");

		text_renderer->renderText("DEFEATED!",0, 0, text_renderer->method_1, "bold");
	}
	else if (this->state == GAME_MENU)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//背景
		text_renderer->renderText("Breakout", 140, 150, text_renderer->method_3, "bold");
		text_renderer->renderText("press from 1 to "+std::to_string(level_num) + " to choose a level", 
			20, 400, text_renderer->method_2, "medium");
	}
	else if (this->state == GAME_WIN)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//背景
		this->levels[this->level].draw(*renderer);	//关卡
		player->draw(*renderer);		//画挡板
		buff_manager->draw(*renderer);	//画道具
		particles->draw();		//画粒子在其他之后 在球之前 因为没有深度检测
		ball->draw(*renderer);

		if (this->level + 1 < level_num)
		{
			text_renderer->renderText("Press Enter to Next Level",
				125, 400, text_renderer->method_2, "medium");
		}
		text_renderer->renderText("Press Backspace to Back to Menu",
			25, 450, text_renderer->method_2, "medium");

		text_renderer->renderText("VICTORY!", 60, 40, text_renderer->method_1, "bold");
	}
	else;
	post_processor->endRender();
	post_processor->render(static_cast<GLfloat>(glfwGetTime()), this->screen_width, this->screen_height);
	//得到glfw运行的时间(s)为单位  用时间来实现后期效果
	Check();
}
void Game::reset()
{
	this->resetLevel();
	this->state = GAME_ACTIVE;
	this->player_lives = this->levels[this->level].lives;
	buff_manager->clear();		//清除之前的道具
	player->reset(ball->isStuck, static_cast<GLfloat>(this->init_screen_width),
		static_cast<GLfloat>(this->init_screen_height));
	ball->isStuck = GL_TRUE;
	text_renderer->reset();
}
void Game::resetLevel()
{
	for (auto& brick : this->levels[this->level].bricks)
	{
		if (!brick.isSolid)
			brick.destroyed = GL_FALSE;		//全都恢复到未摧毁
	}
}
void Game::reset_screen_size(int width, int height)
{
	this->screen_width = width;
	this->screen_height = height;
}
