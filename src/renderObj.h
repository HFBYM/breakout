#pragma once
#include"object.h"
class RenderObj:virtual public Object
{
protected:
	mString tex;
	mString shader;
	glm::vec3 color = glm::vec3(1.0f);

	bool is_log_renderer = false;
	float rotate = 0.0f;
public:
    /// @brief the first can let RenderObj have instance and the secend let its children construct Object
    RenderObj(const mString& id_name, const glm::vec2& pos, const glm::vec2& size, const mString& tex, 
		const mString& shader, glm::vec3 color = glm::vec3(1.0f), float rotate = 0.0f) :Object(pos, size, id_name),
		color(color), tex(tex), shader(shader), rotate(rotate){ }
	RenderObj(const mString& tex, const mString& shader, glm::vec3 color = glm::vec3(1.0f), float rotate = 0.0f) :
		color(color), tex(tex), shader(shader), rotate(rotate) {}
	void Rotate() { static unsigned int i = 1; rotate += static_cast<float>(i) / 100.0f; }

    ~RenderObj();
		
	/// @brief user can log and detach any times
	void log_renderer();
	void detach_renderer();
};
