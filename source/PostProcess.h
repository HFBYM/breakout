#pragma once
#include"Shader.h"
#include"Texture.h"
class PostProcessor		
{
public:
	Shader shader;		//��Ĭ��֡�������ɫ��
	Texture2D tex;		//��Ĭ��֡���������
	GLuint init_width, init_height;
	GLboolean confuse, chaos, shake, blurry;
	PostProcessor(Shader shader, GLuint width, GLuint height);
	void beginRender();		//׼��֡����
	void endRender();		//�ָ���Ĭ��֡����
	void render(GLfloat time, GLuint screen_width, GLuint screen_height);
private:
	GLuint ms_fbo, fbo;		//���ڶ��ز�����֡����ʹ������֡����
	GLuint rbo;				//���ڶ���֡�������ɫ����
	GLuint vao;				//���ڻ���Ļ
	void initData();
};