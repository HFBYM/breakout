#pragma once
#include"glm.hpp"
#include"mString.h"
class Object
{
public:
	~Object() = default;

	/// @brief by default the id_num will add up from 0
	Object(const glm::vec2& pos, const glm::vec2& size, const mString& id_name, 
		unsigned int id_num = getId_num()):
		pos(pos), size(size), id_name(id_name), id_num(id_num){ }

	virtual void log_all() = 0;
	virtual void detach_all() = 0;
protected:
	/// @brief pos is the leftdown point of the object
	glm::vec2 pos = glm::vec2(0.0f);

	glm::vec2 size = glm::vec2(0.0f);

	/// @brief if it's false, this object will just be stored and seem like there isn't one??
	bool to_work = true;
	mString id_name;
	unsigned int id_num = 0;
private:
	inline static unsigned int getId_num() { static unsigned int i = 0; return i++; }
};
