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
        Model* model = entity->model();
        auto batch = m_render_map.find(model);

        if (batch == m_render_map.end()) {
            std::vector<Entity*> new_batch;
            new_batch.push_back(entity);
            m_render_map.emplace(model, new_batch);
        } else {
            batch->second.push_back(entity);
        }
    }

    for (const auto& render_batch : m_render_map) {
        auto batch = render_batch.second;
        Model* model = render_batch.first;
        Material* material = model->material();
        Shader* shader = material->shader();

        shader->bind();
        material->populate();

        if (material->textured())
            material->texture()->bind();

        if (model->instanced())
            render_instanced(model, batch);
        else
            render(model, batch);
    }
}

void Renderer::end()
{
    m_render_map.clear();
    glDisable(GL_DEPTH_TEST);
}

void Renderer::render(Model* model, const std::vector<Entity*> batch)
{
    for (const auto& entity : batch) {
        model->material()->shader()->set_mat4("model", entity->transform().model);
        model->mesh()->render();
    }
}

void Renderer::render_instanced(Model* model, const std::vector<Entity*> batch)
{
    int num_entities = batch.size();
    glm::mat4* models = new glm::mat4[num_entities];
    for (int i = 0; i < num_entities; i++)
        models[i] = batch[i]->transform().model;

    glBindBuffer(GL_ARRAY_BUFFER, model->instanced_vbo());
    glBufferData(GL_ARRAY_BUFFER, num_entities * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_entities * sizeof(glm::mat4), models);

    model->mesh()->render_instanced(num_entities);
}