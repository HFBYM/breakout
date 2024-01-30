#pragma once
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include"Ball.h"
#include"Level.h"
struct Particle		//��ͬ����ͨ��������� ���Բ��ü̳�
{
	glm::vec2 pos;
	glm::vec2 velocity;
	glm::vec4 color;		//��ɫ�Ϳ��Գ䵱����ʹ��

	Particle() :pos(0.0f), velocity(0.0f), color(1.0f) {}	//�ṹ��Ҳ�����к���
};
struct Node:public Particle		//����С����������ѭ��
{
public:
	Node* next = nullptr;
	Node():Particle(){}
};
struct Particles
{
	std::vector<Particle> particles;
	GLfloat alpha = 0.0f;
	Particles* next = nullptr;
	Particles(GLuint amount):particles(amount){}	//ÿ��ש���ü�������
};
class ParticleGenerator		//û���Լ�����Ⱦ���� ����Ҫ��VAO��ʼ��
{
private:
	const GLuint amount = 50;		//���������
	const GLuint amount_block = 20;	//ÿ��ש���������
	const GLuint hb_length = 10;	//ש��������
public:
	ParticleGenerator(Shader shader, Texture2D tex);		//ֵ�����ĺô��ǲ�����ʱ����
	~ParticleGenerator();	//�ͷ������ڴ�
	void update(GLfloat dt, Object& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void draw();
	void particleForBlock(Brick& block);
private:
	Node* head = nullptr;
	Particles* head_block = nullptr;
	Shader shader;
	Texture2D tex;
	GLuint va;
	void init();
	void respawn(Particle& particle, Object& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};