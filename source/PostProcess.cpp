#include<iostream>
#include"PostProcess.h"
#include"Debug.h"
PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height):
	shader(shader),init_width(width),init_height(height),confuse(GL_FALSE),blurry(GL_FALSE),
	chaos(GL_FALSE),shake(GL_FALSE),tex(Texture2D()),shake_time(0.0f)
{
	glGenFramebuffers(1, &this->ms_fbo);	//��֡���岻��һ��Ļ���
	glGenFramebuffers(1, &this->fbo);
	glGenRenderbuffers(1, &this->rbo);

	//���ÿ����
	glBindFramebuffer(GL_FRAMEBUFFER, this->ms_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);		//���ز������岻��Ҫ��ȡ �����rbo
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);	//�û���洢��ʽΪRGB ����A
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->rbo);	//�󶨵�֡����
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)	//���������
	{
		std::cout << "ERROR::FRAMEBUFFER: MSFBO is incomplete" << std::endl;
		__debugbreak();
	}
	Check();

	//���ú���֡����
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	this->tex.generate(width, height, NULL);		//����һ���д�С�����ݵ�����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->tex.id, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)	//���������
	{
		std::cout << "ERROR::FRAMEBUFFER: MSFBO is incomplete" << std::endl;
		__debugbreak();
	}

	//��ʼ������
	this->initData();
	this->shader.setInteger("scene", 0, GL_TRUE);	//����uniform�ĸ���ֵ
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {	//�������õ�ƫ�ú�
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	glUniform2fv(glGetUniformLocation(this->shader.id, "offset"), 9, (GLfloat*)offsets);	//�����������������
	GLint edge_kernel[9] = {	//�񻯺�
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->shader.id, "edge_kernel"), 9, edge_kernel);
	GLfloat blur_kernel[9] = {	//ģ����
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->shader.id, "blur_kernel"), 9, blur_kernel);
}
void PostProcessor::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->ms_fbo);	//msaa����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT);		//����һ���趨����ɫ�������ɫ�����ֹ���

	glViewport(0, 0, init_width, init_height);	//��ʱ�����ӿڴ�С�뻺��һ�� ʹ�䲻��ʧ��
}
void PostProcessor::endRender()
{
	//��msaa�������ݴ�������֡������
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->ms_fbo);	//ȫ��Ҫ��֡����
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
	glBlitFramebuffer(0, 0, this->init_width, this->init_height, 0, 0, 
		this->init_width, this->init_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);	//����ɫ���� ����һ����������ԭ��ԭ���ֱ���
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//��ԭ��Ĭ��֡����׼������Ч��
}
void PostProcessor::render(GLfloat time, GLuint screen_width, GLuint screen_height)
{
	glViewport(0, 0, screen_width, screen_height);	//�ٽ��ӿ����ûص�ǰ���ڴ�С ����֡���嵽�ӿ�������
	this->shader.use();
	this->shader.setFloat("time", time);
	this->shader.setInteger("chaos", this->chaos);
	this->shader.setInteger("shake", this->shake);
	this->shader.setInteger("confuse", this->confuse);
	this->shader.setInteger("blurry", this->blurry);
	glActiveTexture(GL_TEXTURE0);
	this->tex.bind();
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
void PostProcessor::initData()	//����vao
{
	GLuint vbo;
	GLfloat vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), nullptr);
	//һ���������ĸ�float		���ý��б�׼��

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}