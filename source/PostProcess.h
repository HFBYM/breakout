#pragma once
#include"Shader.h"
#include"Texture.h"
class PostProcessor		
{
public:
	Shader shader;		//给默认帧缓冲的着色器
	Texture2D tex;		//给默认帧缓冲的纹理
	GLuint init_width, init_height;
	GLboolean confuse, chaos, shake, blurry;
	PostProcessor(Shader shader, GLuint width, GLuint height);
	void beginRender();		//准备帧缓冲
	void endRender();		//恢复到默认帧缓冲
	void render(GLfloat time, GLuint screen_width, GLuint screen_height);
private:
	GLuint ms_fbo, fbo;		//用于多重采样的帧缓冲和存纹理的帧缓冲
	GLuint rbo;				//用于多重帧缓冲的颜色附件
	GLuint vao;				//用于画屏幕
	void initData();
};