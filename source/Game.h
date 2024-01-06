#pragma once
#include"Debug.h"
#include"Level.h"
#include"BuffManager.h"
enum GameState		//������Ϸ��ǰ״̬
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
	//��Ϸ״̬
	GameState state;
	GLboolean keys[1024];       //���ڴ�������
	GLuint init_screen_width, init_screen_height;		//��Ϸ��ʼ�ֱ���
	GLuint screen_width, screen_height;		//��Ϸ��̬�ķֱ���
	std::vector<Level> levels;	//��Ϸ�ؿ�
	GLuint level;				//��ǰ�ؿ�
	GLuint player_lives;		//�������ֵ
	//�����빹��
	Game(GLuint width, GLuint height);
	~Game();

	void doCollisions();
	void init();       //������ɫ��������
	//��Ϸѭ��
	void princessInput(GLfloat dt);		//��������
	void update(GLfloat dt);	//������Ϸ״̬
	void render();				//��Ⱦѭ��     ���˶��߼�����Ⱦ�߼��ֿ�
	void resetLevel();
	void reset_screen_size(int width, int height);
};

