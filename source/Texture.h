#pragma once
#include<GLAD\glad.h>
class Texture2D
{
public:
	GLuint id;
	GLuint width, height;
	GLuint internal_format;			//����ĸ�ʽ
	GLuint image_format;			//ͼƬ�ĸ�ʽ
	GLuint wrap_s, wrap_t;			//���ݷ�����Ʒ�ʽ
	GLuint filter_min, filter_max;	//�Ŵ����Сʱ��ͼƬ������ʽ

	Texture2D();
	void generate(GLuint width, GLuint height, unsigned char* data);
	void bind() const;
};