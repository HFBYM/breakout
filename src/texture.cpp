#include"texture.h"
#include"check.h"
#include<glad.h>
#define CHECK_STATUS() ASSERT_LOG(isGenerate, "ERROR::TEXTURE: function " << __FUNCTION__ << " uses ungenerated texture " << name.getStr());\
ASSERT_LOG(!isClear, "ERROR::TEXTURE: function " << __FUNCTION__ << " uses ungenerated texture "<< name.getStr())

Texture2D::Texture2D(const mString & name, unsigned int inFormat, unsigned int imaFormat)
	:internal_format(inFormat),image_format(imaFormat), wrap_t(GL_REPEAT), wrap_s(GL_REPEAT), 
	filter_max(GL_LINEAR), filter_min(GL_LINEAR), name(name)
{
	glGenTextures(1, &this->id);
}

Texture2D::~Texture2D()
{
	ASSERT_LOG(isClear, "ERROR::TEXTURE: texture " << name.getStr() << " isn't clear before distruction ");
}

void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data)
{
	ASSERT_LOG(!isGenerate, "ERROR::TEXTURE: texture " << name.getStr() << " is generated one more time");
	isGenerate = true;
	CHECK_STATUS();
	this->width = width;
	this->height = height;
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0,
		this->image_format, GL_UNSIGNED_BYTE, data);
	
	// set the parm for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::bind() const
{
	CHECK_STATUS();
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::clear()
{
	CHECK_STATUS();
	isClear = true;
	glDeleteTextures(1, &this->id);
}
