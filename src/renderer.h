#pragma once
#include"glm.hpp"
class mString;
class Renderer
{
public:
	static void init();
	static void render(unsigned int width, unsigned int height);
	static void clear();

	/// @brief each varient logging will render using its params and it should detach before logging again
	static void log(const mString& id_name, unsigned int id_num, const mString& shader_name, 
		const mString& texture, const glm::vec2& pos, const glm::vec2& size, float& rotate, 
		const glm::vec3& color);

	static void detach(const mString& id_name, unsigned int id_num);
private:
	Renderer() = delete;
	~Renderer() = delete;
	Renderer& operator=(Renderer&) = delete;
};
