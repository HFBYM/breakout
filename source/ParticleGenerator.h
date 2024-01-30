#pragma once
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include"Ball.h"
#include"Level.h"
struct Particle		//不同于普通的物体对象 所以不用继承
{
	glm::vec2 pos;
	glm::vec2 velocity;
	glm::vec4 color;		//颜色就可以充当生命使用

	Particle() :pos(0.0f), velocity(0.0f), color(1.0f) {}	//结构体也可以有函数
};
struct Node:public Particle		//用于小球粒子链表循环
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
	Particles(GLuint amount):particles(amount){}	//每个砖块用几个粒子
};
class ParticleGenerator		//没有自己的渲染精灵 所以要从VAO开始画
{
private:
	const GLuint amount = 50;		//球的粒子数
	const GLuint amount_block = 20;	//每个砖块的粒子数
	const GLuint hb_length = 10;	//砖块链表长度
public:
	ParticleGenerator(Shader shader, Texture2D tex);		//值拷贝的好处是不怕临时变量
	~ParticleGenerator();	//释放链表内存
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