#include"keyboard.h"
#include<glad.h>
#include<glfw3.h>
#include<map>
#include"check.h"

static bool isClear = false;
/// @brief to call a member func needs an instance
std::map<unsigned int,std::pair<KeyboardObj*, void(KeyboardObj::*)(int, int)>> id_obj_func;
void KeyBoard:: key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	ASSERT_LOG(!isClear, "ERROR::KEYBOARD: " << __FUNCTION__ << "uses keyboard cleared");
	// close the window when esc pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (action == GLFW_PRESS || action == GLFW_RELEASE)
		for (auto& i : id_obj_func)
		{
			auto& obj_func = i.second;
			((obj_func.first)->*(obj_func.second))(key, action);
		}
}
void KeyBoard::log(KeyboardObj& obj, void(KeyboardObj::* func_p)(int, int), unsigned int id)
{
	ASSERT_LOG(!isClear, "ERROR::KEYBOARD: " << __FUNCTION__ << "uses keyboard cleared");
	id_obj_func[id] = std::make_pair(&obj, func_p);
}

void KeyBoard::detach(unsigned int id)
{
	ASSERT_LOG(!isClear, "ERROR::KEYBOARD: " << __FUNCTION__ << "uses keyboard cleared");
	id_obj_func.erase(id);
}

void KeyBoard::clear()
{
	ASSERT_LOG(!isClear, "ERROR::KEYBOARD: " << __FUNCTION__ << "uses keyboard cleared");
	id_obj_func.clear();
	isClear = true;
}
