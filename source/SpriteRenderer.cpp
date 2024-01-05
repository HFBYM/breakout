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
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void SpriteRenderer::initRenderData()
{
    GLfloat vertices[] = {          //???����Ԫ��������???
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };                          //ע�����޳� ����Ӧͬ��
    GLuint vb;
    glGenVertexArrays(1, &this->va);
    glGenBuffers(1, &vb);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->va);
    glEnableVertexAttribArray(0);   //���ö�������
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);   //�ĸ��������Ϊһ������
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //����󶨷�ֹ���޸�
    glBindVertexArray(0);
}
