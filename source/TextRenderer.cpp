#include<iostream>
#include<freetype/ft2build.h>
#include FT_FREETYPE_H 
#include"TextRenderer.h"
#include"Resource_manager.h"
TextRenderer::TextRenderer(GLuint width, GLuint height)		///???更好的理解和实现
{
	this->text_shader = ResourceManager::loadShader("resource/shaders/text.shader", "text_shader");
	this->text_shader.setMatrix4("proj", glm::ortho(0.0f,		//设定透视矩阵
		static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->text_shader.setInteger("text", 0);
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);	
	//用于动态绘制 每帧送入新数据 分配空间 但先不送入数据
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void TextRenderer::load(std::string font_file, GLuint font_size, std::string font_name)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))	//初始化
	{
		std::cout << "ERROR::FREETYPE: failed to init freetype" << std::endl;
		__debugbreak();
	}
	FT_Face face;			//创建一个字体面
	if (FT_New_Face(ft, font_file.c_str(), 0, &face))	//把字体导入字体面中
	{
		std::cout << "ERROR::FONT: failed to load font\t" << font_file << std::endl;
		__debugbreak();
	}
	FT_Set_Pixel_Sizes(face, 0, font_size);	//将宽度值设为0表示我们要从字体面通过给定的高度中动态计算出字形的宽度。
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//禁用地址对齐 因为这个纹理一个颜色只有一个字节
	std::map<char, Character> characters;
	for (GLubyte c = 0; c < 128; c++)	//导入128个字符	
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))	//从字符面里导入字符
		{
			std::cout << "ERROR::FONT: failed to load char " << c << std::endl;
			__debugbreak();
		}
		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0
			, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		//源图生成的是8位灰度图，一种颜色只有一个字节 此处只需要红色通道设为alpha分量
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = { tex,glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), 
			static_cast<GLuint>(face->glyph->advance.x )};
		characters.insert(std::make_pair(c, character));
	}
	this->m_lib.insert(std::make_pair(font_name, characters));
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);	//freeType里面的数据都导出来了 可以删除
}
void TextRenderer::renderText(std::string text, GLfloat x, GLfloat y,  
	std::string font_name, glm::vec3 color, GLfloat scale)
{
	if (!this->m_lib.count(font_name))
	{
		std::cout << "ERROR::FONT: the name " << font_name << " hasn't been found" << std::endl;
		__debugbreak();
	}
	this->text_shader.setVector3f("color", color, true);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->vao);

	for (auto c : text)
	{
		Character ch = this->m_lib[font_name][c];		//调出对应的字符
		GLfloat xpos = x + ch.bearing.x * scale;	
		//为了补偿字体纹理宽度和原图的区别 原图的原点排列在一起是最好的
		GLfloat ypos = y + (this->m_lib[font_name]['H'].bearing.y - ch.bearing.y) * scale;
		//因为H的高度为顶高，又从上往下渲染 所以来衡量每个字符的高度

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[] = {				//对每个字符动态的生成位置
			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f,
			xpos,	ypos,	0.0f,0.0f,

			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos + h,	1.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f
		};
		glBindTexture(GL_TEXTURE_2D, ch.tex);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//动态的刷新缓冲
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.distance >> 6) * scale;	
		//advance是下一个字型应在的位置 其存储方式决定需要除以64 
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void TextRenderer::renderText(std::string text, TextMethod& method, std::string font_name, 
	glm::vec3 color, GLfloat scale)
{
	GLfloat x = method.xpos;
	GLfloat y = method.ypos;
	if (!this->m_lib.count(font_name))
	{
		std::cout << "ERROR::FONT: the name " << font_name << " hasn't been found" << std::endl;
		__debugbreak();
	}
	this->text_shader.setVector3f("color", color, true);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->vao);

	for (auto c : text)
	{
		Character ch = this->m_lib[font_name][c];		//调出对应的字符
		GLfloat xpos = x + ch.bearing.x * scale;
		//为了补偿字体纹理宽度和原图的区别 原图的原点排列在一起是最好的
		GLfloat ypos = y + (this->m_lib[font_name]['H'].bearing.y - ch.bearing.y) * scale;
		//因为H的高度为顶高，又从上往下渲染 所以来衡量每个字符的高度

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[] = {				//对每个字符动态的生成位置
			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f,
			xpos,	ypos,	0.0f,0.0f,

			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos + h,	1.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f
		};
		glBindTexture(GL_TEXTURE_2D, ch.tex);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//动态的刷新缓冲
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.distance >> 6) * scale;
		//advance是下一个字型应在的位置 其存储方式决定需要除以64 
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}
void TextRenderer::update(GLfloat dt)
{
	this->method_1.update(dt);
}
TextMethod_1::TextMethod_1()
{
	this->xpos = 0;
	this->ypos = 100;
	this->omiga = 4.5;
	this->omiga = 30 / 180 * 3.14;		//????
	this->time = 0.0f;
}
void TextMethod_1::update(GLfloat dt)
{
	if (this->omiga * this->time > 90)		//达到直角就停下
	{
		__debugbreak();
		return;
	}
	this->time += dt;
	this->xpos = 60 * glm::sin(this->omiga * time);
	this->ypos = 160 - 60 * glm::cos(this->omiga * time);
}
