#pragma once
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include"Object.h"
struct Particle		//不同于普通的物体对象
{
	glm::vec2 pos;
	glm::vec2 velocity;
	glm::vec4 color;
	GLfloat life;
	Particle() :pos(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}	//结构体也可以有函数 初始生命为0
};
class ParticleGenerator		//没有自己的渲染精灵 所以要从VAO开始画
{
public:
	ParticleGenerator(Shader shader, Texture2D tex, GLuint amount);		//值拷贝的好处是不怕临时变量
	void update(GLfloat dt, Object& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void draw();
private:
	std::vector<Particle> particles;
	GLuint amount;
	Shader shader;
	Texture2D tex;
	GLuint va;
	void init();
	GLuint firstUnusedParticle();	//找到第一个熄灭的粒子索引
	void respawn(Particle& particle, Object& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};