#pragma once
#include"renderObj.h"
#include"mString.h"
#include"glm.hpp"
class Brick:public RenderObj
{
public:
	enum BrickType {
		NONE = 0, SOLID, BLUE, GREEN, YELLOW, RED
	};

	Brick(const glm::vec2& pos, const glm::vec2& size, const mString& tex, BrickType type, 
		bool isSolid = false, glm::vec3 color = glm::vec3(1.0f)) :Object(pos,size,"Brick"),
		RenderObj(tex, "sprite", color), type(type), isSolid(isSolid){ }

	/// @brief when a brick is distructed it should have cleaned all the logging
	~Brick() = default;

	virtual void log_all();
	virtual void detach_all();
private:
	BrickType type = NONE;

	bool isSolid = false;
};