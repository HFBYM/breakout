#include"Texture.h"
#include"Debug.h"
Texture2D::Texture2D() :width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_t(GL_REPEAT),
wrap_s(GL_REPEAT), filter_max(GL_LINEAR), filter_min(GL_LINEAR)
{
	glGenTextures(1, &this->id);
}
void Texture2D::generate(GLuint width, GLuint height, unsigned char* data) //并未设置到绑定到哪个槽中
{
	this->width = width;
	this->height = height;
	glBindTexture(GL_TEXTURE_2D, this->id);
	Check();
	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0,
		this->image_format, GL_UNSIGNED_BYTE, data);
	Check();
	//设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->id);
}
