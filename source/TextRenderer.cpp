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

void TextRenderer::load(std::string font, GLuint font_size)
{
	this->characters.clear();	//先清除先前的数据

}

void TextRenderer::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
}
