#include "renderer.h"

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

    m_shader = shader;
    m_shader->bind();
    m_shader->set_mat4("projection_view", camera->projection_view());
}

void Renderer::render(Model* model)
{
    m_shader->set_mat4("model", glm::mat4(1.0));
    model->material()->populate(m_shader);
    model->mesh()->render();
}

void Renderer::end()
{
    glDisable(GL_DEPTH_TEST);
}