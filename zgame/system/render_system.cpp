#include "render_system.h"

/*
    Render System
*/

#define MATRICES_SIZE 64
#define MATRICES_INDEX 0

void RenderSystem::render(Renderer* renderer, entt::registry& registry)
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
            render(renderer, model, batch);
        else
            render_instanced(renderer, model, batch);
    }

    m_render_map.clear();
}

void RenderSystem::render(Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch)
{
     while (!batch.empty()) {
        auto transform = batch.front();
        renderer->simple_render(model, transform->model());
        batch.pop();
    }
}

void RenderSystem::render_instanced(Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch)
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