#pragma once
#include"renderObj.h"
#include"keyboardObj.h"
class Player:public RenderObj, public KeyboardObj
{
public:
	Player(unsigned int width, unsigned int height, glm::vec3 color = glm::vec3(1.0f), glm::vec2 size = getInitSize()) 
		:Object(glm::vec2((width - size.x)/2, height-size.y), size, "Player"), RenderObj("paddle", "sprite", color), init_screen_width(width),
	init_screen_height(height){ }
	~Player() = default;
	void init();

	virtual void log_all();
	virtual void detach_all();

	virtual void processInput(int key, int action);

private:
	unsigned int init_screen_width = 0;
	unsigned int init_screen_height = 0;

	inline static glm::vec2 getInitSize() { static glm::vec2 init_size(100.0, 20.0); return init_size; }
};
