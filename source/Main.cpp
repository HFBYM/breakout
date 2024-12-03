#include<iostream>
#include<GLAD\glad.h>  //附加的头文件和库路径应在C/C++和链接器里面配置 glew库是32位的不符
#include<GLFW\glfw3.h> //以后最好还是不要打破附加库的默认包含关系
#include"Debug.h"
#include"Resource_manager.h"
#include"Game.h"
const static GLuint init_screen_width = 800;
const static GLuint init_screen_height = 600;
static Game breakout(init_screen_width, init_screen_height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) //按键回调函数 四键按不到 可能是回调函数的问题
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)	//检测到esc就关闭窗口
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)			//keys里面储存着所有被按下的字符键
	{
		if (action == GLFW_PRESS)
		{
			breakout.keys[key] = GL_TRUE;
			breakout.keys_status[key].first = GL_TRUE;
		}
		else if (action == GLFW_RELEASE)
		{
			breakout.keys[key] = GL_FALSE;
			breakout.keys_status[key].second = GL_TRUE;
		}
	}
}
void size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	//设置视口
	breakout.reset_screen_size(width, height);
}
GLFWwindow* glInit()
{
	//配置初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//3.3版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		//核心模式

	GLFWwindow* window = glfwCreateWindow(init_screen_width, init_screen_height, "Breakout", nullptr, nullptr);
	if (!window)
	{
		std::cout << "ERROR::WINDOW: fail to create the window" << std::endl;
		glfwTerminate();
		__debugbreak();
	}
	glfwMakeContextCurrent(window);		//让该窗口成为当前上下文

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//glad初始化方式与glew不一样
	{
		std::cout << "ERROR::GLAD: faid to initialize glad" << std::endl;
		glfwTerminate();
		__debugbreak();
	}
	Check();
	return window;
}
int main()
{
	GLFWwindow* window = glInit();
	glfwSetKeyCallback(window, key_callback);	//注册按键回调函数，每次按键就会触发
	glViewport(0, 0, init_screen_width, init_screen_height);	//设置视口	以左下角为原点
	glfwSetWindowSizeCallback(window, size_callback);	//每次窗口大小被改变时调用

	glEnable(GL_CULL_FACE);		//面剔除
	glEnable(GL_BLEND);			//开启混合  注意不进行深度测试
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//常用的混合函数

	breakout.init();

	//帧时间 用于平滑绘制
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	glfwSwapInterval(1);	//设置为每帧更新一次	改变整个循环所用的时间 因此基于循环的变量都会受影响

	while (!glfwWindowShouldClose(window))
	{
		//计算帧时间
		GLfloat currentTime = (GLfloat)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		glfwPollEvents();		//检测事件触发

		//处理输入
		breakout.princessInput(deltaTime);	//处理用户输入
		breakout.update(deltaTime);		//更新游戏状态

		//渲染
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	//不清除的话缓冲会溢出
		breakout.render();

		glfwSwapBuffers(window);
	}
	ResourceManager::clear();
	glfwTerminate();
	return 0;
}