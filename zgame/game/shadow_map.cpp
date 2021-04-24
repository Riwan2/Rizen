#include "shadow_map.h"

/*
    Destructor
*/

ShadowMap::~ShadowMap()
{
    delete m_frame_buffer;
}

/*
    Init
*/

void ShadowMap::init(Shader* depth_shader, const glm::vec2& resolution)
{
    m_depth_shader = depth_shader;
    m_resolution = resolution;

    m_near_plane = 1.0f;
    m_far_plane = 1000.0f;
    m_distance = 120.0f;

    m_light_projection = glm::ortho(-m_distance, m_distance, -m_distance, m_distance, m_near_plane, m_far_plane);
    m_light_view = glm::mat4(1.0);
    m_light_projection_view = m_light_projection * m_light_view;

    m_frame_buffer = new FrameBuffer();
    m_frame_buffer->init_depth(m_resolution);
}

/*
    Resize
*/

void ShadowMap::resize(const glm::vec2& resolution)
{
    m_resolution = resolution;
    m_frame_buffer->resize_depth(m_resolution);
}

/*
    Imgui
*/

void ShadowMap::update_imgui()
{
    ImGui::Begin("Shadow Map Texture");
    ImGui::Image((void*)(intptr_t)texture_id(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
    
    static float resolution = m_resolution.x;
    static float last_resolution = resolution;
    last_resolution = resolution;
    resolution = m_resolution.x;

    ImGui::SliderFloat("Shadow Map Resolution", &resolution, 800, 6400);

    if (last_resolution != resolution)
        resize(glm::vec2(resolution));

    ImGui::End();
}

/*
    Begin depth render
*/

void ShadowMap::begin_depth_render(const glm::vec3& light_direction)
{
    m_light_view = glm::lookAt(light_direction * m_distance, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_light_projection_view = m_light_projection * m_light_view;

    m_depth_shader->bind();
    m_depth_shader->set_mat4("projection_view", m_light_projection_view);

    m_frame_buffer->bind();

    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_resolution.x, m_resolution.y);
}

/*
    End depth render
*/

void ShadowMap::end_depth_render()
{
    glCullFace(GL_BACK);
    m_frame_buffer->unbind();
}