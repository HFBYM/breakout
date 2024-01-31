#include "SpriteRenderer.h"
#include"Debug.h"
SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->shader = shader;
	this->initRenderData();
}
SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->va);
}
void SpriteRenderer::drawSprite(Texture2D& texture, glm::vec2 pos, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    this->shader.use(); //����ɫ��
    glm::mat4 model;    //�ھ�����Ҫ��ƽ�� ����ת ������
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f)); //����ת��������
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // �ƻ���

    model = glm::scale(model, glm::vec3(size, 1.0f));   //�Ը�ֵ�������� ԭ�������½�Ϊ����

    this->shader.setMatrix4("model", model);
    this->shader.setVector3f("spriteColor", color);
    glActiveTexture(GL_TEXTURE0);   //�󶨵�0��
    texture.bind();
    glBindVertexArray(this->va);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    Check();
}
void SpriteRenderer::initRenderData()
{
    GLfloat vertices[] = {          //��ת�������Ͻ�Ϊԭ��
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,     //0
        1.0f, 0.0f, 1.0f, 0.0f,     //1
        0.0f, 0.0f, 0.0f, 0.0f,     //2
        1.0f, 1.0f, 1.0f, 1.0f,     //3
    };                          //ע�����޳� ����Ӧͬ��
    GLuint indices[] = {
        0,1,2,
        0,3,1
    };
    GLuint vb;
    GLuint eb;
    glGenVertexArrays(1, &this->va);
    glGenBuffers(1, &vb);
    glGenBuffers(1, &eb);

    glBindVertexArray(this->va);        //�Ȱ�va�ٰ���Ӧ��eb��vb

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);   //���ö�������
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);   
    //�ĸ��������Ϊһ������
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //����󶨷�ֹ���޸�
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
