#include "renderer.h"

/*
    Renderer
*/

Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::init()
{
    return true;
}

void Renderer::begin(Camera* camera, Shader* shader)
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

//    m_shader = shader;
//    m_shader->bind();
//    m_shader->set_mat4("projection_view", camera->projection_view());
}

void Renderer::render(Entity* entity, Camera* camera)
{
    entity->model()->material()->shader()->bind();
    entity->model()->material()->shader()->set_mat4("projection_view", camera->projection_view());
    
    entity->model()->material()->shader()->set_mat4("model", entity->transform().model);

    if (entity->model()->material()->texture() != nullptr)
        entity->model()->material()->texture()->bind();

    entity->model()->material()->populate();
    entity->model()->mesh()->render();

    if (entity->model()->material()->texture() != nullptr)
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::render(const std::vector<Entity*>& entities, Camera* camera) 
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
        if (material->texture() != nullptr)
            material->texture()->bind();

        material->populate();
        shader->set_mat4("projection_view", camera->projection_view());

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