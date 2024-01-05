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

void TextRenderer::load(std::string font, GLuint font_size)
{
	this->characters.clear();	//�������ǰ������

}

void TextRenderer::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
}
