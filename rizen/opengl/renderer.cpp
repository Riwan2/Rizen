#include "renderer.h"

/*
    Renderer
*/

Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_ubo);
}

void Renderer::init()
{
    set_ubo();
}

void Renderer::set_ubo()
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, MATRICES_SIZE + LIGHT_DIR_SIZE + SKY_COLOR_SIZE, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, MATRICES_INDEX, m_ubo, 0, MATRICES_SIZE);
    glBindBufferRange(GL_UNIFORM_BUFFER, LIGHT_DIR_INDEX, m_ubo, 0, LIGHT_DIR_SIZE);
    glBindBufferRange(GL_UNIFORM_BUFFER, SKY_COLOR_INDEX, m_ubo, 0, SKY_COLOR_SIZE);
}

void Renderer::begin(Camera* camera, const glm::vec3& sun_direction, const glm::vec3& sky_color)
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    
    size_t offset = 0;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(camera->projection_view()));
    
    offset += sizeof(glm::mat4);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(sun_direction));

    offset += sizeof(glm::vec4);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(sky_color));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::end()
{
    glDisable(GL_DEPTH_TEST);
}

void Renderer::simple_render(Model* render_model, const glm::mat4& model)
{
    render_model->material()->shader()->set_mat4("model", model);
    render_model->mesh()->render();
}

void Renderer::render(Model* render_model, const glm::mat4& model)
{
    render_model->material()->shader()->bind();
    render_model->material()->populate();

    if (render_model->material()->texture())
        render_model->material()->texture()->bind();

    render_model->material()->shader()->set_mat4("model", model);
    render_model->mesh()->render();
}

void Renderer::simple_render_instanced(Model* render_model, glm::mat4* models, int num_models)
{
    glBindBuffer(GL_ARRAY_BUFFER, render_model->instanced_vbo());
    glBufferData(GL_ARRAY_BUFFER, num_models * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_models * sizeof(glm::mat4), models);

    render_model->mesh()->render_instanced(num_models);
}

void Renderer::render_instanced(Model* render_model, glm::mat4* models, int num_models)
{
    render_model->material()->shader()->bind();
    render_model->material()->populate();

    if (render_model->material()->texture())
        render_model->material()->texture()->bind();

    glBindBuffer(GL_ARRAY_BUFFER, render_model->instanced_vbo());
    glBufferData(GL_ARRAY_BUFFER, num_models * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_models * sizeof(glm::mat4), models);

    render_model->mesh()->render_instanced(num_models);
}

void Renderer::bind_ubo(Shader* shader)
{
    GLuint index = glGetUniformBlockIndex(shader->program_id(), "Matrices");
	glUniformBlockBinding(shader->program_id(), index, MATRICES_INDEX);
}