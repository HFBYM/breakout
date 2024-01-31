#pragma once
#include"Shader.h"
#include"Texture.h"
class SpriteRenderer	//用于便捷渲染精灵	给游戏对象渲染使用
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();
	void drawSprite(Texture2D& texture, glm::vec2 pos, glm::vec2 size = glm::vec2(10.0f, 10.0f), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));		//默认参数 还未传入proj矩阵
private:
	Shader shader;
	GLuint va;

	void initRenderData();		//初始化vao vbo参数
};