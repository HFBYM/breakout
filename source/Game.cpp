#include"Game.h"
#include"SpriteRenderer.h"
#include"Resource_manager.h"	//��̬�ĺô� �������ɸ���
#include"Ball.h"
#include"ParticleGenerator.h"
#include"PostProcess.h"
#include"Player.h"
#include<irrKlang/include/irrKlang.h>	//���ֿ�Ŀ¼
#include"TextRenderer.h"
const GLuint level_num = 5;

using Collision = std::tuple<GLboolean, Direction, glm::vec2>;	//��ṹ�� ��std::get<i>(val)����

using namespace irrklang;		//ʹ�������ռ�
ISoundEngine* sound_engine = createIrrKlangDevice();	//��ʼ����������

SpriteRenderer* renderer;

Player* player;

const glm::vec2 ball_velocity(200.0f, -450.0f);
const GLfloat ball_radius(25.0f);
Ball* ball;

ParticleGenerator* particles;		//?????��������ʱ������Ч��????

BuffManager* buff_manager;

PostProcessor* post_processor;
GLfloat shake_time = 0.0f;

TextRenderer* text_renderer;
Direction getDirect(glm::vec2 target)	//�õ�һ�������Ĵ��·���	???����׼ȷ
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,1.0f),	//y+	��
		glm::vec2(1.0f,0.0f),	//x+	��
		glm::vec2(0.0f,-1.0f),	//y-	��
		glm::vec2(-1.0f,0.0f)	//x-	��
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
	//AABB-AABB		x�����y�������ײ
	bool collisionX = one.pos.x + one.size.x >= two.pos.x && two.pos.x + two.size.x >= one.pos.x;
	bool collisionY = one.pos.y + one.size.y >= two.pos.y && two.pos.y + two.size.y >= one.pos.y;
	return collisionX && collisionY;
}
Collision checkCollisions(Ball& one, Object& two)
{
	// ��ȡԲ������ 
	glm::vec2 center(one.pos + one.radius);
	// ����AABB����Ϣ�����ġ���߳���
	glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
	glm::vec2 aabb_center(
		two.pos.x + aabb_half_extents.x,
		two.pos.y + aabb_half_extents.y
	);
	// ��ȡ�������ĵĲ�ʸ��
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);	//????
	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
	glm::vec2 closest = aabb_center + clamped;
	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
	difference = closest - center;
	if (glm::length(difference) < one.radius)	//������ײ	��ģ�ͱ��Ͳ��Ǻܾ�ȷ
		return { GL_TRUE,getDirect(difference),difference };	//�õ���ʸ�����ض�λ��
	else
		return{ GL_FALSE,UP,glm::vec2(0.0f) };
}

Game::Game(GLuint width, GLuint height):state(GAME_MENU),screen_width(width),screen_height(height),keys(),
level(3),init_screen_width(width), init_screen_height(height),player_lives(0)
{
	//���Կհ׳�ʼ������
}
Game::~Game()
{
	if(renderer)
		delete renderer;
}
void Game::init()	//����������Դ�ĵ���
{
	Check();
	//װ����ɫ��
	ResourceManager::loadShader("resource/shaders/postProcess.shader", "postProcess");
	ResourceManager::loadShader("resource/shaders/sprite.shader", "sprite");

	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(this->init_screen_width), 
		static_cast<GLfloat>(this->init_screen_height), 0.0f, -1.0f, 1.0f);		
	//����ͶӰ���� ���Ͻ���(0,0)����Ļ�����Ӧ	��ת���������Ͻ�???���·�ת??
	ResourceManager::getShader("sprite").use().setInteger("image", 0);	//�趨������
	ResourceManager::getShader("sprite").use().setMatrix4("proj", proj);	//��������ͶӰ

	ResourceManager::loadShader("resource/shaders/particle.shader", "particle");
	ResourceManager::getShader("particle").use().setInteger("sprite", 0);	//�趨������
	ResourceManager::getShader("particle").use().setMatrix4("proj", proj);	//��������ͶӰ
	
	//װ������
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

	//����ؿ�
	for (unsigned int i = 0; i < level_num; i++)
	{
		Level level;
		std::string str = "resource/levels/level_" + std::to_string(i + 1) + ".lvl";
		level.load(str.c_str(), this->init_screen_width,
			static_cast<GLuint>(this->init_screen_height * 0.5));
		this->levels.push_back(level);
	}

	renderer = new SpriteRenderer (ResourceManager::getShader("sprite").use());	//ֱ���õ��������Ⱦ

	glm::vec2 player_pos = glm::vec2(this->init_screen_width / 2 - player_size.x / 2, 
		this->init_screen_height - player_size.y);
	player = new Player(player_pos, ResourceManager::getTexture("paddle"));
	glm::vec2 ball_pos = player_pos + glm::vec2(player_size.x / 2 - ball_radius, -ball_radius * 2);
	ball = new Ball(ball_pos, ball_radius, ball_velocity, ResourceManager::getTexture("basketball"));

	particles = new ParticleGenerator(ResourceManager::getShader("particle"),
		ResourceManager::getTexture("particle"), 500);	//�ܹ�500������

	post_processor = new PostProcessor(ResourceManager::getShader("postProcess"),
		this->init_screen_width, this->init_screen_height);

	buff_manager = new BuffManager;

	sound_engine->play2D("resource/music/breakout.mp3", GL_TRUE);	//�ظ�����

	text_renderer = new TextRenderer(this->init_screen_width, this->init_screen_height);
	text_renderer->load("resource/font/ARIALNI.TTF", 20, "medium");	//�������·��arialbi.ttf
	text_renderer->load("resource/font/arialbi.ttf", 40, "bold");	//���ڱ���
	Check();
}
void Game::doCollisions()
{
	for (auto& brick : this->levels[this->level].bricks)
	{
		if (!brick.destroyed)	//δ���ݻٵ�
		{
			Collision ret = checkCollisions(*ball, brick);
			if (std::get<0>(ret))	//������ײ
			{
				if(!brick.isSolid)	//�ɴݻٵ�
				{
					sound_engine->play2D("resource/music/solid.wav", GL_FALSE);	//���ظ�����
					brick.destroyed = true;
					buff_manager->spawnPowerUp(brick,this->levels[this->level].unit_size);	
					//��ÿһ�����ݻٵ���һ�������ж�
					if (buff_manager->t_pass_through!=0.0f)		//����д�ԽЧ����ֱ�Ӵ���
						continue;
				}
				else	//��̵ľͶ���
				{
					sound_engine->play2D("resource/music/bleep.mp3", GL_FALSE);	//���ظ�����
					shake_time = 0.05f;
					post_processor->shake = GL_TRUE;
				}
				Direction dir = std::get<1>(ret);
				glm::vec2 difference = std::get<2>(ret);
				if (dir == RIGHT || dir == LEFT)	//ˮƽ��ײ
				{
					ball->velocity.x *= -1;
					//�ض�λ	???	������һ��Сƽ�� ����ȷҲ����
					GLfloat penetration = ball->radius - glm::length(difference.x);
					if (dir == LEFT)
						ball->pos.x -= penetration;	
					else
						ball->pos.x += penetration;
				}
				else
				{
					ball->velocity.y *= -1;  // ��ת��ֱ�ٶ�
					GLfloat penetration = ball->radius - glm::length(difference.y);
					if (dir == UP)
						ball->pos.y -= penetration;	//����
					else
						ball->pos.y += penetration;
				}
			}
		}
	}
	Collision ret = checkCollisions(*ball, *player);
	if (!ball->isStuck && std::get<0>(ret))		//δ���̶����뵲�巢����ײ
	{
		GLfloat distance = ball->pos.x + ball->radius - (player->pos.x + player->size.x / 2);
		GLfloat percentage = distance / (player->size.x / 2);	//��ײ��������ĵİٷֱ�	����Ϊ����
		sound_engine->play2D("resource/music/bleep.wav", GL_FALSE);	//���ظ�����

		//�ٶȸ���
		GLfloat strenghth = 2.0f;
		ball->velocity.y = -glm::abs(ball->velocity.y);	//��Զ����һ�����ϵ��ٶ� ��ֹճ�� ???����ʹ�·���ײ������
		ball->velocity.x = ball_velocity.x * percentage * strenghth;	//x�����ٶȸ���	ʹx������һЩ�����ٶ�
		ball->velocity = glm::normalize(ball->velocity) * glm::length(ball_velocity);	//�����ٶȴ�С����
	}
	for (auto& powerUp : buff_manager->powerUps)
	{
		if (checkCollisions(*player, powerUp))	//�����������ײ
			{   
			//�������
			sound_engine->play2D("resource/music/powerup.wav", GL_FALSE);	//���ظ�����
			buff_manager->activatePowerUp(powerUp,*ball,*player,*post_processor);
				powerUp.destroyed = GL_TRUE;
			}
	}
}
void Game::princessInput(GLfloat dt)	//����ٶ������������ٶȲ�ͬ
{
	if (this->state == GAME_ACTIVE)	//����ʱ
	{
		player->princessInput(dt, this->keys, static_cast<GLfloat>(this->init_screen_width),
			static_cast<GLfloat>(this->init_screen_height));
		if (this->keys[GLFW_KEY_SPACE])
			ball->isStuck = false;
		if (this->keys[GLFW_KEY_P])		//???����ʹ��
			__debugbreak();
	}
	else if (this->state == GAME_DEFEAT)
	{
		if (this->keys[GLFW_KEY_ENTER])		//����һ��
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
void Game::update(GLfloat dt)	//���ڸ����ڲ����˶�	ÿ��ѭ����Ҫ���еĴ���
{
	if (this->state == GAME_ACTIVE)
	{
		//����
		ball->move(dt, this->init_screen_width, player->pos + glm::vec2(player->size.x / 2 - ball_radius,
			-2 * ball_radius));
		buff_manager->updatePowerUp(dt, *post_processor, static_cast<GLfloat>(this->init_screen_height), 
			ball->color);
		particles->update(dt, *ball, 2, glm::vec2(ball->radius * 4 / 5));	
		//һ�μ������� offset�����������������

		this->doCollisions();
		//����Ƿ������ױ�
		if (ball->pos.y >= this->init_screen_height)
		{
			this->player_lives--;
			if (this->player_lives > 0)
				ball->isStuck = GL_TRUE;
			else		//ʧ��
			{
				this->state = GAME_DEFEAT;
				post_processor->blurry = GL_TRUE;	//����ʧ��ʱģ��Ч��
				buff_manager->reset(*post_processor, ball->color);	//��ֹ���еĵ���Ч��
			}
		}
		if (this->levels[this->level].isCompleted())	//�ɹ�ͨ���ؿ�
		{
			this->state = GAME_WIN;
			buff_manager->reset(*post_processor, ball->color);	//��ֹ���еĵ���Ч��
		}

		if (shake_time > 0.0f)	//��ѭ����ʱ����ʵ�ֵ�ʱ��ֿ�		???�ŵ�����������
		{
			shake_time -= dt;	//1sʧȥ1
			if (shake_time <= 0.0f)		//���������ֹ����趨
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
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//����
		this->levels[this->level].draw(*renderer);	//�ؿ�
		player->draw(*renderer);		//������
		buff_manager->draw(*renderer);	//������
		particles->draw();		//������������֮�� ����֮ǰ ��Ϊû����ȼ��
		ball->draw(*renderer);	//����

		text_renderer->renderText("Lives " + std::to_string(this->player_lives), 0, 0, "medium");
	}
	else if (this->state == GAME_DEFEAT)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//����
		this->levels[this->level].draw(*renderer);	//�ؿ�
		player->draw(*renderer);		//������
		buff_manager->draw(*renderer);	//������
		particles->draw();		//������������֮�� ����֮ǰ ��Ϊû����ȼ��

		text_renderer->renderText("Press Enter to Restart",
			150, 400, text_renderer->method_2, "medium");
		text_renderer->renderText("Press Backspace to Back to Menu",
			25, 450, text_renderer->method_2, "medium");

		text_renderer->renderText("DEFEATED!",0, 0, text_renderer->method_1, "bold");
	}
	else if (this->state == GAME_MENU)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//����
		text_renderer->renderText("Breakout", 140, 150, text_renderer->method_3, "bold");
		text_renderer->renderText("press from 1 to "+std::to_string(level_num) + " to choose a level", 
			20, 400, text_renderer->method_2, "medium");
	}
	else if (this->state == GAME_WIN)
	{
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f)
			, glm::vec2(this->init_screen_width, this->init_screen_height), 0.0f);	//����
		this->levels[this->level].draw(*renderer);	//�ؿ�
		player->draw(*renderer);		//������
		buff_manager->draw(*renderer);	//������
		particles->draw();		//������������֮�� ����֮ǰ ��Ϊû����ȼ��
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
	//�õ�glfw���е�ʱ��(s)Ϊ��λ  ��ʱ����ʵ�ֺ���Ч��
	Check();
}
void Game::reset()
{
	this->resetLevel();
	this->state = GAME_ACTIVE;
	this->player_lives = this->levels[this->level].lives;
	buff_manager->clear();		//���֮ǰ�ĵ���
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
			brick.destroyed = GL_FALSE;		//ȫ���ָ���δ�ݻ�
	}
}
void Game::reset_screen_size(int width, int height)
{
	this->screen_width = width;
	this->screen_height = height;
}
