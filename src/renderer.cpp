#include"renderer.h"
#include"glad.h"
#include<map>
#include"resource_manager.h"
#include"shader.h"
#include"texture.h"
#include<gtc\type_ptr.hpp>
#include<gtc/matrix_transform.hpp>
#include"mString.h"
#include"check.h"
#include"debug.h"
#define CHECK_STATUS() ASSERT_LOG(isInit, "ERROR::RENDERER: function " << __FUNCTION__ << " uses renderer not initialized");\
    ASSERT_LOG(!isClear, "ERROR::RENDERER: function " << __FUNCTION__ << " uses renderer cleared")

//??logger
struct LogData
{
    mString shader_name;
    mString texture_name;
    const glm::vec2& pos;       
    const glm::vec2& size;
    const float& rotate;
    glm::vec3 color;
    LogData(const mString& shader_name, const mString& texture, const glm::vec2& pos, 
        const glm::vec2& size, float& rotate, const glm::vec3& color):shader_name(shader_name), 
        texture_name(texture), pos(pos), size(size), rotate(rotate), color(color){ }
};

/// @brief there is only one va globally
static GLuint va;
static bool isInit = false, isClear = false;
static std::map<mString, std::map<unsigned int, LogData*>> log_datas;

void Renderer::init()
{
    isInit = true;
    // the leftup is origin after turning up and down
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,     //0
        1.0f, 0.0f, 1.0f, 0.0f,     //1
        0.0f, 0.0f, 0.0f, 0.0f,     //2
        1.0f, 1.0f, 1.0f, 1.0f,     //3
    };

    //caution the face cull
    GLuint indices[] = {
        0,1,2,
        0,3,1
    };
    GLuint vb;
    GLuint eb;
    glGenVertexArrays(1, &va);
    glGenBuffers(1, &vb);
    glGenBuffers(1, &eb);

    // bind the va first
    glBindVertexArray(va);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // enable the vertex attribute
    glEnableVertexAttribArray(0);

    // four float for one vertex
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

    // finish binding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::render(unsigned int width, unsigned int height)
{
    Check();
    CHECK_STATUS();
    // set the color to clear the color buffer and avoid overflowing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& i : log_datas["Brick"])
    {
        const LogData& temp = *i.second;
        ResourceManager::getShader(temp.shader_name).use();     //??优化按照着色器分类

        // first move, then rotate, scale at last
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(temp.pos, 0.0f));

        // make it rotate to the middle
        model = glm::translate(model, glm::vec3(0.5 * temp.size.x, 0.5 * temp.size.y, 0.0f)); 
        model = glm::rotate(model, temp.rotate, glm::vec3(0.0f, 0.0f, 1.0f));

        // move it back
        model = glm::translate(model, glm::vec3(-0.5f * temp.size.x, -0.5f * temp.size.y, 0.0f)); 

        model = glm::scale(model, glm::vec3(temp.size, 1.0f));   

        ResourceManager::getShader(temp.shader_name).setUniform("model", model);
        ResourceManager::getShader(temp.shader_name).setUniform("spriteColor", temp.color);


        glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width),
	        static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
        ResourceManager::getShader("sprite").use().setUniform("image", 0);	
        ResourceManager::getShader("sprite").use().setUniform("proj", proj);	


        // bind the texture to slot 0
        glActiveTexture(GL_TEXTURE0);   
        ResourceManager::getTexture(temp.texture_name).bind();
        glBindVertexArray(va);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }
    for (auto& i : log_datas["Player"])
    {
        const LogData& temp = *i.second;
        ResourceManager::getShader(temp.shader_name).use();

        // first move, then rotate, scale at last
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(temp.pos, 0.0f));

        // make it rotate to the middle
        model = glm::translate(model, glm::vec3(0.5 * temp.size.x, 0.5 * temp.size.y, 0.0f));
        model = glm::rotate(model, temp.rotate, glm::vec3(0.0f, 0.0f, 1.0f));

        // move it back
        model = glm::translate(model, glm::vec3(-0.5f * temp.size.x, -0.5f * temp.size.y, 0.0f));

        model = glm::scale(model, glm::vec3(temp.size, 1.0f));

        ResourceManager::getShader(temp.shader_name).setUniform("model", model);
        ResourceManager::getShader(temp.shader_name).setUniform("spriteColor", temp.color);


        glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width),
            static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
        ResourceManager::getShader("sprite").use().setUniform("image", 0);
        ResourceManager::getShader("sprite").use().setUniform("proj", proj);


        // bind the texture to slot 0
        glActiveTexture(GL_TEXTURE0);
        ResourceManager::getTexture(temp.texture_name).bind();
        glBindVertexArray(va);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }

    Check();
}

void Renderer::clear()
{
    CHECK_STATUS();
    isClear = true;
    glDeleteVertexArrays(1, &va);
    for (auto& i : log_datas)
    {
        for (auto& j : i.second)
            if (j.second)
                delete j.second;
        i.second.clear();
    }
    log_datas.clear();
}

void Renderer::log(const mString& id_name, unsigned int id_num, const mString& shader_name,
    const mString& texture, const glm::vec2& pos, const glm::vec2& size, float& rotate,
    const glm::vec3& color)
{
    CHECK_STATUS();
    log_datas[id_name][id_num] = new LogData({ shader_name, texture, pos, size, rotate, color });
}

void Renderer::detach(const mString& id_name, unsigned int id_num)
{
    CHECK_STATUS();
    log_datas[id_name].erase(id_num);
}
