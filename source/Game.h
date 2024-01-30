#pragma once
#include"Debug.h"
#include"Level.h"
#include"BuffManager.h"
#include<irrKlang/include/irrKlang.h>	//���ֿ�Ŀ¼
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
public:
	enum GameState		//������Ϸ��ǰ״̬
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN,
		GAME_DEFEAT
	};
	//��Ϸ״̬
	GameState state;
	GLboolean keys[1024];       //���ڴ�������
	GLuint init_screen_width, init_screen_height;		//��Ϸ��ʼ�ֱ���
	GLuint screen_width, screen_height;		//��Ϸ��̬�ķֱ���
	std::vector<Level> levels;	//��Ϸ�ؿ�
	GLuint level;				//��ǰ�ؿ�
	int player_lives;		//�������ֵ

	//������		������Ҫ��ȫ�ֱ���
	irrklang::ISoundEngine* sound_engine = irrklang::createIrrKlangDevice();	//��ʼ��
	SpriteRenderer* renderer = nullptr;
	Player* player = nullptr;
	Ball* ball = nullptr;
	ParticleGenerator* particles = nullptr;		//?????��������ʱ������Ч��????
	BuffManager* buff_manager = nullptr;
	PostProcessor* post_processor = nullptr;
	TextRenderer* text_renderer = nullptr;

	//�����빹��
	Game(GLuint width, GLuint height);
	~Game();

	void doCollisions();
	void init();       //������ɫ��������
	//��Ϸѭ��
	void princessInput(GLfloat dt);		//��������
	void update(GLfloat dt);	//������Ϸ״̬
	void render();				//��Ⱦѭ��     ���˶��߼�����Ⱦ�߼��ֿ�
	void reset();
	void resetLevel();
	void reset_screen_size(int width, int height);
};

