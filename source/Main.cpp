#include<iostream>
#include<GLAD\glad.h>  //���ӵ�ͷ�ļ��Ϳ�·��Ӧ��C/C++���������������� glew����32λ�Ĳ���
#include<GLFW\glfw3.h> //�Ժ���û��ǲ�Ҫ���Ƹ��ӿ��Ĭ�ϰ�����ϵ
#include"Debug.h"
#include"Resource_manager.h"
#include"Game.h"
const static GLuint init_screen_width = 800;
const static GLuint init_screen_height = 600;
static Game breakout(init_screen_width, init_screen_height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) //�����ص����� ??�ļ�������
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
//����ȫ��д �ṹ�塢������ͷȫ��д �����»��� �������շ�
//[]��Ӧprivate��Ա ()��Ӧoperator()���β��б� mutable����Ӧconst����
//<>�������ǲ�����Ŀ�е��ļ� ""������������Ŀ�е��ļ� dll�Ƕ�̬���п� Ҫ���ڵ�ǰ��Ŀexe�ļ��� lib�Ǿ�̬��������������
//�ص������ĵ����������߳�֮�ⷢ��

//������ɫ��������ᱻ�����դ���׶�,��ͼԪӳ��Ϊ������Ļ����Ӧ�����أ�����Ƭ�Ρ�
//��Ƭ����ɫ������֮ǰ��ִ�в��С����лᶪ�����������ͼ������������أ���������ִ��Ч�ʡ�
//ͼ��ѧ�д��������ڲü�����������Ŀ��� Ȼ��Ͷ�䵽�ӿ��� ���ͨ��window���ü�
//�ü���������跶Χ�ڵ�����任��-1��1֮�� �������ֱ䵽����1 Ȼ��ִ�в��� ��դ��(���һ��������) Ȼ��͸�ӳ���
//�����ж�Ӧ��ɫֵȷ���Ժ�,���Ƭ�ε���ȣ���ģ�壩ֵ��Ҳ����alphaֵ����������л��(���ڽ�����gl_FragColor)
//�����е�������������ᱻ���Բ�ֵ��Ƭ����ɫ��ʹ��

//MSAA:ÿ�������ж���Ӳ����� ����һ��������ʱ�Ż�Ը����ص����Ĳ��� ����ʱ����δ������ ��GPU��ѡ��������ǵ������������ 
//Ȼ�������� ģ����Դ洢����ɫ������ ����Ҫ��������ز����Ļ��巢�͸���ͨ����(������) �������ƽ��(����С��ͬ��ֱ�����)
//�����Զ���֡������ʵ���������λ��� ��������δ�view�ӿڵ�����
//�����ڻ��ƽ�msFBOǰ�����ӿڴ�С �ڻ���Ĭ��֡����ǰ�ٸ�һ�δ�С �Ϳ���ʵ�������ӿڴ�С��ͬ 
//����Ϊǰ�ߵ����� ����������껹��Ҫ��ǰ�� ��Ҫ����ߵı仯���仯 ֻ����Ĭ��֡�������ʱ����Ҫ�仯���ӿڵĴ�С

//��д����ǰ�����ʵ�ּܹ�		��vao������û��Ҫ��װ ����� ��Ϊ�Ѿ���spriteRenderer
//������ֻ����ֱ�����ַ������ɹ��� ���ṩʹ�õķ��� ��Դ�����ฺ����ļ��ж�ȡ����̬�洢��������
//��Ⱦ���鸺����Ⱦ������λ������ ��������ĵײ���Ⱦ�߼�  ��Ϸ�ฺ�������� ���״̬ �ܵ���Ⱦ ����Ϸ��ĳ�ʼ���н�����Դ�ĵ���
//��Ϸ������������������Ϸ��Ӧ�еĶ��� �Ⱦ�����ϲ�һ��	�м�˵��඼����ô�洢�����������Ϣ main����ֻҪ�����ܹ��ͺ�
//�ؿ��洢ÿ���ؿ���Ķ�����Ϣ ��ͬ�ĵ�ͼ ����Ϣ���ļ��е��� ����Ⱦ�����ⲿ���ݽ��� ��ʼ���͹��캯����������
//�ð󶨵İ����ص������������а������¼�������Ϸ���д洢	�ø��º���������Ϸ�ڲ�����		ע��dt�Ĳ��� dx dy
//���Ӿ�����һ�����������ֲ���Ⱦ�������������ʱ���͸�����ٶȽ�С������ glBlendFunc(GL_SRC_ALPHA, GL_ONE)��ǿЧ��
//֡������һ����� ����Ҫ����������ɫ���� ������Ϊ�������Ⱦ�������(ֻд������)	�ò��ֿ�����Ϸ����֮�����
//ѭ�������ʱ��ֻ��Ҫ�ڴ�������� ����GPU���ǲ���ֵ���� ������ͨ������ֵ��ʵ�ֲٿ� ���������rand
//��ײ���ĵײ���͹����εķ������㷨 ���չ��Ϊ�������θ����ⷨʸƽ�е�ƽ���Ͻ���ͶӰȻ����
//Բ���ⷨʸ����Բ����������������