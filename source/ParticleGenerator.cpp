#include"ParticleGenerator.h"
#include"Debug.h"
#include<iostream>
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D tex):
	shader(shader),tex(tex)
{
	this->init();
}
ParticleGenerator::~ParticleGenerator()
{
	if(this->head)
	{
		auto current = this->head;
		auto next = current->next;
		while (next != this->head)
		{
			delete current;
			current = next;
			next = next->next;
		}
		delete current;
		this->head = nullptr;
	}
	if (this->head_block)
	{
		auto current = this->head_block;
		auto next = current->next;
		while (next != this->head_block)
		{
			delete current;
			current = next;
			next = next->next;
		}
		delete current;
		this->head_block = nullptr;
	}
}
void ParticleGenerator::update(GLfloat dt, Object& object, GLuint newParticles, glm::vec2 offset)
{
	for (GLuint i = 0; i < newParticles; i++)	//激活n个粒子
	{
		this->respawn(*this->head, object, offset);
		this->head = this->head->next;
	}
	Node* p = this->head;
	for (GLuint i = 0; i < this->amount; i++)
	{
		if(p->color.w > 0.0f)
		{
			p->color.w -= dt * 2.5f;	//颜色透明度衰减
			if (p->color.w > 0.0f)
				p->pos += p->velocity * dt;
			else
				p->color.w = 0.0f;
		}
		p = p->next;
	}

	Particles* pb = this->head_block;
	for (GLuint i = 0; i < this->hb_length; i++)
	{
		if (pb->alpha > 0.0f)
		{
			pb->alpha -= dt * 2.5f;	//颜色透明度衰减
			if (pb->alpha > 0.0f)
			{
				for (auto& particle : pb->particles)	//移动粒子
				{
					particle.pos += particle.velocity * dt;
					particle.color.a = pb->alpha;
				}
			}
			else
				pb->alpha = 0.0f;
		}
		pb = pb->next;
	}
}
void ParticleGenerator::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	//开启混合 增强粒子混合后的颜色
	this->shader.use();
	this->tex.bind();
	glBindVertexArray(this->va);
	Node* p = this->head;
	for (GLuint i = 0; i < this->amount; i++)
	{
		if (p->color.w > 0.0f)	//仍存活的
		{
			this->shader.setVector2f("offset", p->pos);
			this->shader.setVector4f("particleColor", p->color);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		}
		p = p->next;
	}
	Particles* pb = this->head_block;
	for (GLuint i = 0; i < this->hb_length; i++)
	{
		if (pb->alpha > 0.0f)
		{
			for (auto& particle : pb->particles)
			{
				this->shader.setVector2f("offset", particle.pos);
				this->shader.setVector4f("particleColor", particle.color);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			}
		}
		pb = pb->next;
	}
	glBindVertexArray(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//这才是常用的
}
static void resetParticles(Particles& particles, Brick& block)	//砖块的宽高
{
	GLfloat width = block.size.x;
	GLfloat height = block.size.y;
	std::vector<Particle>& particles_temp = particles.particles;
	particles.alpha = 1.0f;
	for (auto& particle : particles_temp)
	{
		GLfloat x = (rand() % static_cast<GLuint>(height * 100)) / 100.0f - height / 2;
		GLfloat y = (rand() % static_cast<GLuint>(height * 100)) / 100.0f - height / 2;
		particle.pos.x = x + block.pos.x + width / 2 - 5;
		particle.pos.y = y + block.pos.y + height / 2 - 5;
		particle.velocity.x = x;
		particle.velocity.y = y;

		GLfloat rcolor = rand() % 100 / 100.0f + 0.25f;	//0.25到1.25
		glm::vec4 color;
		switch (block.type)		//根据砖块的不同颜色显示不同粒子
		{
		case Brick::BLUE:	color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);	break;
		case Brick::GREEN:	color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);	break;
		case Brick::YELLOW:	color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);	break;
		case Brick::RED:	color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);	break;
		default:
			std::cout << "ERROR::LEVEL: unnamed type of brick" << std::endl;
			__debugbreak();
		}
		particle.color = glm::vec4(rcolor, rcolor, rcolor, 1.0f) * color;	//使颜色随机强度
	}
}
void ParticleGenerator::particleForBlock(Brick& block)
{
	resetParticles(*this->head_block, block);
	this->head_block = this->head_block->next;
}
void ParticleGenerator::init()
{
	Check();
	GLuint vb;
	GLuint eb;
	GLfloat particle_quad[] = {		//左下角为原点
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	GLuint indices[] = {
		0,1,2,
		0,3,1
	};
	glGenVertexArrays(1, &this->va);
	glGenBuffers(1, &vb);
	glGenBuffers(1, &eb);

	glBindVertexArray(this->va);

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Node* p = this->head = new Node;
	for (GLuint i = 1; i < this->amount; i++)
	{
		p->next = new Node;
		p = p->next;
	}
	p->next = this->head;		//实现循环链表

	Particles* pb = this->head_block = new Particles(amount_block);
	for (GLuint i = 1; i < hb_length; i++)
	{
		pb->next = new Particles(amount_block);
		pb = pb->next;
	}
	pb->next = this->head_block;		//实现循环链表

	Check();
}
void ParticleGenerator::respawn(Particle& particle, Object& object, glm::vec2 offset)
{
	GLfloat random = ((rand()) % 100 - 50) / 10.0f;	//-5到5的小数
	GLfloat rcolor = rand() % 100 / 100.0f + 0.25f;	//0.25到1.25
	particle.pos = object.pos + offset + random;	//生成在物体周围
	particle.color = glm::vec4(rcolor, rcolor, rcolor, 1.0f) * glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
	particle.velocity = object.velocity * 0.1f;		//重置速度
}
