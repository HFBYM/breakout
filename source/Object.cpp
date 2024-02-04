#include"Object.h"
Object::Object() : pos(0, 0), size(1, 1), velocity(0.0f), color(1.0f), 
rotation(0.0f), tex_sprite(), isSolid(GL_FALSE), destroyed(GL_FALSE)	//�������������Ϊ��
{

}
Object::Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: pos(pos), size(size), velocity(velocity), color(color), rotation(0.0f), tex_sprite(sprite), 
	isSolid(GL_FALSE), destroyed(GL_FALSE)	//Ĭ�Ͽɴݻ�
{

}
void Object::draw(SpriteRenderer& renderer)
{
	renderer.drawSprite(this->tex_sprite, this->pos, this->size, this->rotation, this->color);
}
