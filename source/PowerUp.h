#pragma once
#include"Object.h"
const glm::vec2 VELOCITY(0.0f, 150.0f);
class PowerUp :public Object
{
public:
	enum BuffType {
		SPEED, STICKY, PASS_THROUGH, PAD_SIZE_INCREASE, CONFUSE, CHAOS
	};
	BuffType m_type;

	PowerUp(BuffType type, glm::vec3 color, glm::vec2 pos, Texture2D tex, glm::vec2 size)
		:Object(pos, size, tex, color, VELOCITY), m_type(type)
	{}
};