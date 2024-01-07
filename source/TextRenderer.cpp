#include<iostream>
#include<freetype/ft2build.h>
#include FT_FREETYPE_H 
#include"TextRenderer.h"
#include"Resource_manager.h"
TextRenderer::TextRenderer(GLuint width, GLuint height)		///???���õ�����ʵ��
{
	this->text_shader = ResourceManager::loadShader("resource/shaders/text.shader", "text_shader");
	this->text_shader.setMatrix4("proj", glm::ortho(0.0f,		//�趨͸�Ӿ���
		static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->text_shader.setInteger("text", 0);
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);	
	//���ڶ�̬���� ÿ֡���������� ����ռ� ���Ȳ���������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void TextRenderer::load(std::string font_file, GLuint font_size, std::string font_name)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))	//��ʼ��
	{
		std::cout << "ERROR::FREETYPE: failed to init freetype" << std::endl;
		__debugbreak();
	}
	FT_Face face;			//����һ��������
	if (FT_New_Face(ft, font_file.c_str(), 0, &face))	//�����嵼����������
	{
		std::cout << "ERROR::FONT: failed to load font\t" << font_file << std::endl;
		__debugbreak();
	}
	FT_Set_Pixel_Sizes(face, 0, font_size);	//�����ֵ��Ϊ0��ʾ����Ҫ��������ͨ�������ĸ߶��ж�̬��������εĿ�ȡ�
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//���õ�ַ���� ��Ϊ�������һ����ɫֻ��һ���ֽ�
	std::map<char, Character> characters;
	for (GLubyte c = 0; c < 128; c++)	//����128���ַ�	
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))	//���ַ����ﵼ���ַ�
		{
			std::cout << "ERROR::FONT: failed to load char " << c << std::endl;
			__debugbreak();
		}
		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0
			, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		//Դͼ���ɵ���8λ�Ҷ�ͼ��һ����ɫֻ��һ���ֽ� �˴�ֻ��Ҫ��ɫͨ����Ϊalpha����
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
	FT_Done_FreeType(ft);	//freeType��������ݶ��������� ����ɾ��
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
		Character ch = this->m_lib[font_name][c];		//������Ӧ���ַ�
		GLfloat xpos = x + ch.bearing.x * scale;	
		//Ϊ�˲������������Ⱥ�ԭͼ������ ԭͼ��ԭ��������һ������õ�
		GLfloat ypos = y + (this->m_lib[font_name]['H'].bearing.y - ch.bearing.y) * scale;
		//��ΪH�ĸ߶�Ϊ���ߣ��ִ���������Ⱦ ����������ÿ���ַ��ĸ߶�

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[] = {				//��ÿ���ַ���̬������λ��
			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f,
			xpos,	ypos,	0.0f,0.0f,

			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos + h,	1.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f
		};
		glBindTexture(GL_TEXTURE_2D, ch.tex);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//��̬��ˢ�»���
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.distance >> 6) * scale;	
		//advance����һ������Ӧ�ڵ�λ�� ��洢��ʽ������Ҫ����64 
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
		Character ch = this->m_lib[font_name][c];		//������Ӧ���ַ�
		GLfloat xpos = x + ch.bearing.x * scale;
		//Ϊ�˲������������Ⱥ�ԭͼ������ ԭͼ��ԭ��������һ������õ�
		GLfloat ypos = y + (this->m_lib[font_name]['H'].bearing.y - ch.bearing.y) * scale;
		//��ΪH�ĸ߶�Ϊ���ߣ��ִ���������Ⱦ ����������ÿ���ַ��ĸ߶�

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[] = {				//��ÿ���ַ���̬������λ��
			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f,
			xpos,	ypos,	0.0f,0.0f,

			xpos,	ypos + h,	0.0f,1.0f,
			xpos + w,	ypos + h,	1.0f,1.0f,
			xpos + w,	ypos,	1.0f,0.0f
		};
		glBindTexture(GL_TEXTURE_2D, ch.tex);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//��̬��ˢ�»���
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.distance >> 6) * scale;
		//advance����һ������Ӧ�ڵ�λ�� ��洢��ʽ������Ҫ����64 
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
	if (this->omiga * this->time > 90)		//�ﵽֱ�Ǿ�ͣ��
	{
		__debugbreak();
		return;
	}
	this->time += dt;
	this->xpos = 60 * glm::sin(this->omiga * time);
	this->ypos = 160 - 60 * glm::cos(this->omiga * time);
}
