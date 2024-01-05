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
    this->shader.use(); //绑定着色器
    glm::mat4 model;    //在矩阵中要先平移 再旋转 最后放缩
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f)); //让旋转沿着中轴
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // 移回来

    model = glm::scale(model, glm::vec3(size, 1.0f));   //对各值进行缩放 原本以左下角为中心

    this->shader.setMatrix4("model", model);
    this->shader.setVector3f("spriteColor", color);
    glActiveTexture(GL_TEXTURE0);   //绑定到0槽
    texture.bind();
    glBindVertexArray(this->va);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void SpriteRenderer::initRenderData()
{
    GLfloat vertices[] = {          //???可用元素索引简化???
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };                          //注意面剔除 方向应同向
    GLuint vb;
    glGenVertexArrays(1, &this->va);
    glGenBuffers(1, &vb);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->va);
    glEnableVertexAttribArray(0);   //启用顶点属性
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);   //四个浮点解释为一个顶点
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //解除绑定防止被修改
    glBindVertexArray(0);
}
