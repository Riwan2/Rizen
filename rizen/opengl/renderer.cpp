#include "renderer.h"

/*
    Renderer
*/

Renderer::Renderer() {}
Renderer::~Renderer() 
{
    m_render_map.clear();
    glDeleteBuffers(1, &m_ubo);
}

bool Renderer::init()
{
    set_ubo();
    return true;
}

void Renderer::bind_ubo(Material* material)
{
    Shader* shader = material->shader();
    GLuint index = glGetUniformBlockIndex(shader->program_id(), "Matrices");
	glUniformBlockBinding(shader->program_id(), index, MATRICES_INDEX);
}

void Renderer::set_ubo()
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, MATRICES_SIZE, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, MATRICES_INDEX, m_ubo, 0, MATRICES_SIZE);
}

void Renderer::begin(Camera* camera)
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->projection_view()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::render(const std::vector<Entity*>& entities) 
{
    for (auto entity : entities) {
        Material* material = entity->model()->material();
        auto batch = m_render_map.find(material);

        if (batch == m_render_map.end()) {
            std::vector<Entity*> new_batch;
            new_batch.push_back(entity);
            m_render_map.emplace(material, new_batch);
        } else {
            batch->second.push_back(entity);
        }
    }

    for (const auto& batch : m_render_map) {
        Material* material = batch.first;
        Shader* shader = material->shader();

        shader->bind();
        material->populate();

        if (material->texture() != nullptr)
            material->texture()->bind();

        for (const auto& entity : batch.second) {
            shader->set_mat4("model", entity->transform().model);
            entity->model()->mesh()->render();
        }
    }
}

void Renderer::end()
{
    m_render_map.clear();
    glDisable(GL_DEPTH_TEST);
}