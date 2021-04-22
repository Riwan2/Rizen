#include "render_system.h"

/*
    Render System
*/

#define MATRICES_SIZE 64
#define MATRICES_INDEX 0

void RenderSystem::begin(entt::registry& registry)
{
    auto group = registry.group<TransformComponent, RenderComponent>();

    for (auto [entity, transform, renderable] : group.each()) {

        Model* model = renderable.model;
        auto pair = m_render_map.find(model);

        transform.update();

        if (pair == m_render_map.end()) {
            std::queue<TransformComponent*> batch;
            batch.push(&transform);
            m_render_map[model] = batch;
        } else {
            pair->second.push(&transform);
        }
    }
}

void RenderSystem::end()
{
    m_render_map.clear();
}

void RenderSystem::render(Renderer* renderer)
{
    for (auto pair : m_render_map) {
        auto batch = pair.second;
        Model* model = pair.first;
        Material* material = model->material();
        Shader* shader = material->shader();
        Texture* texture = material->texture();

        shader->bind();
        material->populate();

        if (texture != nullptr)
            texture->bind();

        if (!model->instanced())
            render_batch(renderer, model, batch);
        else
            render_instanced_batch(renderer, model, batch);
    }
}

void RenderSystem::render_depth(Shader* depth_shader, Renderer* renderer)
{
     for (auto pair : m_render_map) {
        auto batch = pair.second;
        Model* model = pair.first;

        if (!model->instanced())
            render_batch_depth(depth_shader, renderer, model, batch);
        else
            render_instanced_batch_depth(depth_shader, renderer, model, batch);
    }
}

void RenderSystem::render_batch(Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch)
{
     while (!batch.empty()) {
        auto transform = batch.front();
        renderer->simple_render(model, transform->model());
        batch.pop();
    }
}

void RenderSystem::render_batch_depth(Shader* depth_shader, Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch)
{
    while (!batch.empty()) {
        auto transform = batch.front();
        renderer->render_depth(depth_shader, model, transform->model());
        batch.pop();
    }
}


void RenderSystem::render_instanced_batch(Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch)
{
    int num_entities = batch.size();

    glm::mat4* models = new glm::mat4[num_entities];
    for (int i = 0; i < num_entities; i++) {
        auto transform = batch.front();
        models[i] = transform->model();
        batch.pop();
    }

    renderer->simple_render_instanced(model, models, num_entities);
    delete[] models;
}

void RenderSystem::render_instanced_batch_depth(Shader* depth_shader, Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch)
{
    int num_entities = batch.size();

    glm::mat4* models = new glm::mat4[num_entities];
    for (int i = 0; i < num_entities; i++) {
        auto transform = batch.front();
        models[i] = transform->model();
        batch.pop();
    }

    renderer->render_instanced_depth(depth_shader, model, models, num_entities);
    delete[] models;   
}
