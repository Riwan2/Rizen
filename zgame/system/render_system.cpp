#include "render_system.h"

/*
    Render System
*/

#define MATRICES_SIZE 64
#define MATRICES_INDEX 0

void RenderSystem::init()
{
    set_ubo();
}

void RenderSystem::set_ubo()
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, MATRICES_SIZE, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, MATRICES_INDEX, m_ubo, 0, MATRICES_SIZE);
}

void RenderSystem::bind_ubo(Material* material)
{
    Shader* shader = material->shader();
    GLuint index = glGetUniformBlockIndex(shader->program_id(), "Matrices");
	glUniformBlockBinding(shader->program_id(), index, MATRICES_INDEX);
}

void RenderSystem::render(Camera* camera)
{
    begin(camera);

    for (auto entity : m_entities) {
        auto transform = Coordinator::get_component<Transform>(entity);
        auto renderable = Coordinator::get_component<Renderable>(entity);

        Model* model = renderable->model;
        auto pair = m_render_map.find(model);

        if (pair == m_render_map.end()) {
            std::queue<Transform*> batch;
            batch.push(transform);
            m_render_map[model] = batch;
        } else {
            pair->second.push(transform);
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

        if (texture)
            texture->bind();

        if (!model->instanced())
            render(model, batch);
        else
            render_instanced(model, batch);
    }

    end();
}

void RenderSystem::begin(Camera* camera)
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->projection_view()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderSystem::end()
{
    m_render_map.clear();
    glDisable(GL_DEPTH_TEST);
}

void RenderSystem::render(Model* model, std::queue<Transform*>& batch)
{
     while (!batch.empty()) {
        auto transform = batch.front();
        model->material()->shader()->set_mat4("model", transform->model);
        model->mesh()->render();
        batch.pop();
    }
}

void RenderSystem::render_instanced(Model* model, std::queue<Transform*>& batch)
{
    int num_entities = batch.size();

    glm::mat4* models = new glm::mat4[num_entities];
    for (int i = 0; i < num_entities; i++) {
        auto transform = batch.front();
        models[i] = transform->model;
        batch.pop();
    }

    glBindBuffer(GL_ARRAY_BUFFER, model->instanced_vbo());
    glBufferData(GL_ARRAY_BUFFER, num_entities * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_entities * sizeof(glm::mat4), models);

    model->mesh()->render_instanced(num_entities);
}