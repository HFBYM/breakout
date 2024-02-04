#include<iostream>
#include"PostProcess.h"
#include"Debug.h"
PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height):
	shader(shader),init_width(width),init_height(height),confuse(GL_FALSE),blurry(GL_FALSE),
	chaos(GL_FALSE),shake(GL_FALSE),tex(Texture2D()),shake_time(0.0f)
{
	glGenFramebuffers(1, &this->ms_fbo);	//是帧缓冲不是一般的缓冲
	glGenFramebuffers(1, &this->fbo);
	glGenRenderbuffers(1, &this->rbo);

	//配置抗锯齿
	glBindFramebuffer(GL_FRAMEBUFFER, this->ms_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);		//多重采样缓冲不需要读取 因此用rbo
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);	//该缓冲存储格式为RGB 不用A
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->rbo);	//绑定到帧缓冲
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)	//检测完整性
	{
		std::cout << "ERROR::FRAMEBUFFER: MSFBO is incomplete" << std::endl;
		__debugbreak();
	}
	Check();

	//配置后期帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	this->tex.generate(width, height, NULL);		//生成一个有大小无数据的纹理
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->tex.id, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)	//检测完整性
	{
		std::cout << "ERROR::FRAMEBUFFER: MSFBO is incomplete" << std::endl;
		__debugbreak();
	}

	//初始化数据
	this->initData();
	this->shader.setInteger("scene", 0, GL_TRUE);	//设置uniform的各个值
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {	//给纹理用的偏置核
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
	glUniform2fv(glGetUniformLocation(this->shader.id, "offset"), 9, (GLfloat*)offsets);	//传递两个浮点的向量
	GLint edge_kernel[9] = {	//锐化核
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->shader.id, "edge_kernel"), 9, edge_kernel);
	GLfloat blur_kernel[9] = {	//模糊核
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->shader.id, "blur_kernel"), 9, blur_kernel);
}
void PostProcessor::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->ms_fbo);	//msaa缓冲
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT);		//用上一行设定的颜色来清除颜色缓冲防止溢出

	glViewport(0, 0, init_width, init_height);	//此时设置视口大小与缓冲一致 使其不会失真
}
void PostProcessor::endRender()
{
	//把msaa缓冲数据传到后期帧缓冲中
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->ms_fbo);	//全都要用帧缓冲
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
	glBlitFramebuffer(0, 0, this->init_width, this->init_height, 0, 0, 
		this->init_width, this->init_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);	//传颜色缓冲 并用一个过滤器还原到原本分辨率
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//还原到默认帧缓冲准备后期效果
}
void PostProcessor::render(GLfloat time, GLuint screen_width, GLuint screen_height)
{
	glViewport(0, 0, screen_width, screen_height);	//再将视口设置回当前窗口大小 反正帧缓冲到视口是满射
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
void PostProcessor::initData()	//配置vao
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
	//一个顶点有四个float		不用进行标准化

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}