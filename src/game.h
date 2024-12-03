#pragma once
/// @brief This is a singleton rather than a static one cause some functions can't be called anywhere
class Game
{
public:
	/// @brief get the reference of the only instance
	static Game& get_instance();
	
	/// @brief Game should be initialized at first then run
	void init();

	/// @brief make sure Game is initialized before running
	void run();

private:
	/// @brief if this function is public then the singleton can be delete without reconstruction
	~Game();

	Game() = default;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
};
