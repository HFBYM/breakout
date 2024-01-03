#pragma once
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include"Object.h"
struct Particle		//��ͬ����ͨ���������
{
	glm::vec2 pos;
	glm::vec2 velocity;
	glm::vec4 color;
	GLfloat life;
	Particle() :pos(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}	//�ṹ��Ҳ�����к��� ��ʼ����Ϊ0
};
class ParticleGenerator		//û���Լ�����Ⱦ���� ����Ҫ��VAO��ʼ��
{
public:
	ParticleGenerator(Shader shader, Texture2D tex, GLuint amount);		//ֵ�����ĺô��ǲ�����ʱ����
	void update(GLfloat dt, Object& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void draw();
private:
	std::vector<Particle> particles;
	GLuint amount;
	Shader shader;
	Texture2D tex;
	GLuint va;
	void init();
	GLuint firstUnusedParticle();	//�ҵ���һ��Ϩ�����������
	void respawn(Particle& particle, Object& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};