#include"game.h"
#include<glad.h>	//the glew lib is 32bit so we use glad
#include<glfw3.h>	//this lib shuld be included after glad
#include"keyboard.h"
#include"resource_manager.h"
#include"renderer.h"
#include"level.h"
#include"check.h"
#include"player.h"

/// @brief the width and height of the window
static int init_screen_width = 800;
static int init_screen_height = 600;
static int screen_width;
static int screen_height;
static bool isInit = false;
static Level level(0);
static Player* player = nullptr;

static GLFWwindow* window = nullptr;

static GLFWwindow* gl_init()
{
	//initialize the glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//core module

	//create the window hint and check
	GLFWwindow* window = glfwCreateWindow(init_screen_width, init_screen_height, 
		"Breakout", nullptr, nullptr);
	ASSERT_LOG(window, "ERROR::WINDOW: fail to create the window");

	//make this window the current context
	glfwMakeContextCurrent(window);

	//it's different from glew
	ASSERT_LOG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "ERROR::GLAD: faid to initialize glad");

	return window;
}

/// @brief save the width and height resized
static void size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	screen_height = height;
	screen_width = width;
}

Game& Game:: get_instance()
{
	static Game m_game;
	return m_game;
}

void Game::init()
{
	ASSERT_LOG(!isInit, "ERROR::GAME: game should not be initialized more times");
	isInit = true;

	window = gl_init();

	//set the viewport leftdown is origin
	glViewport(0, 0, init_screen_width, init_screen_height);	

	//function will work whenever keys pressed
	glfwSetKeyCallback(window, KeyBoard::key_callback);		

	//function will work each time window resized
	glfwSetWindowSizeCallback(window, size_callback);

	// to reduce the cost
	glEnable(GL_CULL_FACE);

	// enable blend and caution not to test the depth
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ResourceManager::init();
	Renderer::init();

	level.init(PROJECT_DIR"/assets/levels/level_1.lvl", init_screen_width, 
		init_screen_height/2);
	level.log_renderer();

	player = new Player(init_screen_width, init_screen_height);
	player->init();
	player->log_all();
}

void Game::run()
{
	ASSERT_LOG(isInit, "ERROR::GAME: " << __FUNCTION__ << " operate game not initialized");

	// use frame time to make it smooth
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;

	//60hz it will change the time the loop spend
	glfwSwapInterval(1);	

	while (!glfwWindowShouldClose(window))
	{
		// calculate frame time
		GLfloat currentTime = (GLfloat)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// trigger the events such as mouse and keyboard
		glfwPollEvents();

		////处理输入
		//breakout.princessInput(deltaTime);	//处理用户输入
		//breakout.update(deltaTime);		//更新游戏状态

		Renderer::render(init_screen_width, init_screen_height);
		level.Rotate();
		// swap the two buffers
		glfwSwapBuffers(window);
	}
}

Game::~Game()
{
	level.clear();
	player->detach_all();
	ResourceManager::clear();
	Renderer::clear();
	KeyBoard::clear();
	glfwTerminate();
}
