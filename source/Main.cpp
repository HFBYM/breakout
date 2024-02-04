#include<iostream>
#include<GLAD\glad.h>  //附加的头文件和库路径应在C/C++和链接器里面配置 glew库是32位的不符
#include<GLFW\glfw3.h> //以后最好还是不要打破附加库的默认包含关系
#include"Debug.h"
#include"Resource_manager.h"
#include"Game.h"
const static GLuint init_screen_width = 800;
const static GLuint init_screen_height = 600;
static Game breakout(init_screen_width, init_screen_height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) //按键回调函数 ??四键按不到
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
//常量全大写 结构体、类名开头全大写 变量下划线 函数名驼峰
//[]对应private成员 ()对应operator()的形参列表 mutable，对应const属性
//<>包含的是不在项目中的文件 ""包含的是在项目中的文件 dll是动态运行库 要放在当前项目exe文件旁 lib是静态库在设置里链接
//回调函数的调用是在主线程之外发生

//几何着色器的输出会被传入光栅化阶段,把图元映射为最终屏幕上相应的像素，生成片段。
//在片段着色器运行之前会执行裁切。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。
//图形学中窗口类似于裁剪矩阵定义出来的口子 然后投射到视口上 最后通过window来裁剪
//裁剪矩阵把所需范围内的坐标变换到-1到1之间 其他部分变到大于1 然后执行裁切 光栅化(变成一个个像素) 然后透视除法
//在所有对应颜色值确定以后,检测片段的深度（和模板）值，也会检查alpha值并对物体进行混合(用内建变量gl_FragColor)
//顶点中的纹理、法线坐标会被线性插值给片段着色器使用

//MSAA:每个像素有多个子采样点 至少一个被覆盖时才会对该像素的中心采样 若此时中心未被覆盖 则GPU挑选最近被覆盖的子样本点采样 
//然后进行深度 模板测试存储进颜色缓冲中 接着要把这个多重采样的缓冲发送给普通缓冲(过滤器) 对其进行平均(若大小不同则直接填充)
//若有自定义帧缓冲则实际上是两次绘制 因此有两次次view视口的限制
//可以在绘制进msFBO前更改视口大小 在绘制默认帧缓冲前再改一次大小 就可以实现两次视口大小不同 
//后者为前者的拉伸 因此世界坐标还是要看前者 不要因后者的变化而变化 只有在默认帧缓冲绘制时才需要变化后视口的大小

//在写代码前最好先实现架构		像vao这样的没必要封装 更灵活 因为已经有spriteRenderer
//工具类只负责直接用字符串构成工具 并提供使用的方法 资源管理类负责从文件中读取并静态存储、管理工具
//渲染精灵负责渲染基本单位和纹理 是其他类的底层渲染逻辑  游戏类负责处理输入 变更状态 总的渲染 在游戏类的初始化中进行资源的导入
//游戏对象负责派生所有在游戏中应有的对象 比精灵更上层一点	中间端的类都不怎么存储自身的数据信息 main函数只要基本架构就好
//关卡存储每个关卡里的对象信息 像不同的地图 其信息从文件中导入 将渲染器从外部传递进入 初始化和构造函数分离更灵活
//用绑定的按键回调函数处理所有按键的事件并在游戏类中存储	用更新函数处理游戏内部运算		注意dt的参与 dx dy
//粒子就是在一个物体后随机分布渲染多个有生命且随时间变透明的速度较小的物体 glBlendFunc(GL_SRC_ALPHA, GL_ONE)加强效果
//帧缓冲是一个框架 必须要有完整的颜色附件 附件分为纹理和渲染缓冲对象(只写不采样)	该部分可在游戏主体之后添加
//循环所需的时间只需要在代码出调控 传给GPU的是布尔值就行 道具类通过布尔值来实现操控 随机数就用rand
//碰撞检测的底层是凸多边形的分离轴算法 最终归结为在与多边形各个外法矢平行的平面上进行投影然后检测
//圆的外法矢就找圆心与最近顶点的连线