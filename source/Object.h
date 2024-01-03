#pragma once
#include<glm\glm.hpp>
#include"Debug.h"
#include"Texture.h"
#include"SpriteRenderer.h"
class Object	//游戏里的所有对象都是这个类的派生类  使各对象更关注于自身属性而不是渲染逻辑
{
public:	
	//确定对象数据和性质
	glm::vec2 pos, size, velocity;	//速度
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid;		//是否可摧毁 
	GLboolean destroyed;	//是否已被摧毁

	//渲染状态
	Texture2D tex_sprite;
	Object();
	Object(glm::vec2 pos, glm::vec2 size, Texture2D tex, glm::vec3 color = glm::vec3(1.0f),
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f));	//两种初始化方式
	virtual void draw(SpriteRenderer &renderer);	//虚函数 可以派生
};