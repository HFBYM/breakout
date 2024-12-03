#include<iostream>
#include<GLAD\glad.h>  //���ӵ�ͷ�ļ��Ϳ�·��Ӧ��C/C++���������������� glew����32λ�Ĳ���
#include<GLFW\glfw3.h> //�Ժ���û��ǲ�Ҫ���Ƹ��ӿ��Ĭ�ϰ�����ϵ
#include"Debug.h"
#include"Resource_manager.h"
#include"Game.h"
const static GLuint init_screen_width = 800;
const static GLuint init_screen_height = 600;
static Game breakout(init_screen_width, init_screen_height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) //�����ص����� �ļ������� �����ǻص�����������
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)	//��⵽esc�͹رմ���
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)			//keys���洢�������б����µ��ַ���
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
	glViewport(0, 0, width, height);	//�����ӿ�
	breakout.reset_screen_size(width, height);
}
GLFWwindow* glInit()
{
	//���ó�ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//3.3�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		//����ģʽ

	GLFWwindow* window = glfwCreateWindow(init_screen_width, init_screen_height, "Breakout", nullptr, nullptr);
	if (!window)
	{
		std::cout << "ERROR::WINDOW: fail to create the window" << std::endl;
		glfwTerminate();
		__debugbreak();
	}
	glfwMakeContextCurrent(window);		//�øô��ڳ�Ϊ��ǰ������

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//glad��ʼ����ʽ��glew��һ��
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
	glfwSetKeyCallback(window, key_callback);	//ע�ᰴ���ص�������ÿ�ΰ����ͻᴥ��
	glViewport(0, 0, init_screen_width, init_screen_height);	//�����ӿ�	�����½�Ϊԭ��
	glfwSetWindowSizeCallback(window, size_callback);	//ÿ�δ��ڴ�С���ı�ʱ����

	glEnable(GL_CULL_FACE);		//���޳�
	glEnable(GL_BLEND);			//�������  ע�ⲻ������Ȳ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//���õĻ�Ϻ���

	breakout.init();

	//֡ʱ�� ����ƽ������
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	glfwSwapInterval(1);	//����Ϊÿ֡����һ��	�ı�����ѭ�����õ�ʱ�� ��˻���ѭ���ı���������Ӱ��

	while (!glfwWindowShouldClose(window))
	{
		//����֡ʱ��
		GLfloat currentTime = (GLfloat)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		glfwPollEvents();		//����¼�����

		//��������
		breakout.princessInput(deltaTime);	//�����û�����
		breakout.update(deltaTime);		//������Ϸ״̬

		//��Ⱦ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	//������Ļ���������
		breakout.render();

		glfwSwapBuffers(window);
	}
	ResourceManager::clear();
	glfwTerminate();
	return 0;
}