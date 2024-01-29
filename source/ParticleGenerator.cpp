#include"ParticleGenerator.h"
#include"Debug.h"
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D tex, GLuint amount):
	shader(shader),tex(tex),amount(amount)
{
	this->init();
}
void ParticleGenerator::update(GLfloat dt, Object& object, GLuint newParticles, glm::vec2 offset)
{
	for (GLuint i = 0; i < newParticles; i++)	//����n������
		this->respawn(this->particles[this->firstUnusedParticle()], object, offset);
	for (auto& particle : this->particles)
	{
		particle.life -= dt;	//ÿ������1����	ע��dt�Ĳ���
		if (particle.life > 0.0f)
		{
			particle.pos += particle.velocity * dt;
			particle.color.w -= dt * 2.5f;	//��ɫ͸����˥�����ٶȿ�������
		}
	}
}
void ParticleGenerator::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	//�������
	this->shader.use();
	this->tex.bind();
	glBindVertexArray(this->va);
	for(auto& particle:this->particles)
	{
		if(particle.life>0.0f)	//�Դ���
		{
			this->shader.setVector2f("offset", particle.pos);
			this->shader.setVector4f("particleColor", particle.color);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindVertexArray(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//����ǳ��õ�
}
void ParticleGenerator::init()
{
	Check();
	GLuint vb;
	GLfloat particle_quad[] = {		//���½�Ϊԭ��
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->va);
	glGenBuffers(1, &vb);
	glBindVertexArray(this->va);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->amount; i++)
		this->particles.push_back(Particle());	//��������������ʼ��
	Check();
}
static GLuint lastParticle = 0;	//�����ҵ����һ����Ϩ�������
GLuint ParticleGenerator::firstUnusedParticle()
{
	for (GLuint i = lastParticle; i < this->amount; i++)
	{
		if (this->particles[i].life < 0.0f)	//�ҵ���һ��Ϩ�������
		{
			lastParticle = i;
			return i;
		}
	}
	for (GLuint i = 0; i < lastParticle; i++)
	{
		if (this->particles[i].life < 0.0f)	//�ҵ���һ��Ϩ�������
		{
			lastParticle = i;
			return i;
		}
	}
	return 0;	//û��Ϩ�������
}
void ParticleGenerator::respawn(Particle& particle, Object& object, glm::vec2 offset)
{
	GLfloat random = ((rand()) % 100 - 50) / 10.0f;	//-5��5��С��
	GLfloat rcolor = 3.5f + rand() % 100 / 100.0f;	//3.5��4.5
	particle.pos = object.pos + offset + random;	//������������Χ
	particle.color = glm::vec4(rcolor, rcolor, rcolor, 1.0f);	//ʹ��ɫ���ǿ��
	particle.life = 1.0f;	//��������
	particle.velocity = object.velocity * 0.1f;		//�����ٶ�
}
