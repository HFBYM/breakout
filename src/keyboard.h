# pragma once
struct GLFWwindow;
class KeyboardObj;
/// @brief store and process the keyboard input
class KeyBoard
{
public:
	/// @brief mention	that it won't work when more than 3 keys pressed, it might be due to GLFW
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	/// @brief set the func_p as one of the callback
	static void log(KeyboardObj& obj, void(KeyboardObj::*func_p)(int, int), unsigned int id);

	static void detach(unsigned int id);

	/// @brief it can't clear one more time
	static void clear();
private:
	KeyBoard() = default;
	~KeyBoard() = default;
};
