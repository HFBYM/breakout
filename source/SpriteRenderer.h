#pragma once
#include"Shader.h"
#include"Texture.h"
class SpriteRenderer	//���ڱ����Ⱦ����	����Ϸ������Ⱦʹ��
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();
	void drawSprite(Texture2D& texture, glm::vec2 pos, glm::vec2 size = glm::vec2(10.0f, 10.0f), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));		//Ĭ�ϲ��� ��δ����proj����
private:
	Shader shader;
	GLuint va;

	void initRenderData();		//��ʼ��vao vbo����
};