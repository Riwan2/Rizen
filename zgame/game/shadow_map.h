#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "../../rizen/opengl/shader.h"
#include "../../rizen/opengl/texture.h"
#include "../../rizen/utils/rizen_math.h"

#include "../../extern/imgui/imgui.h"

class ShadowMap {
public:
    ~ShadowMap();

    void init(Shader* depth_shader, const glm::vec2& resolution);
    void resize(const glm::vec2& resolution);
    
    void update_imgui();

    void begin_depth_render(const glm::vec3& light_direction);
    void end_depth_render();

    Shader* depth_shader() const { return m_depth_shader; }
    const glm::mat4& light_projection_view() const { return m_light_projection_view; }
    const GLuint texture_id() const { return m_frame_buffer->texture()->texture_id(); }
    const glm::vec2& resolution() const { return m_resolution; }

private:
    Shader* m_depth_shader;
    FrameBuffer* m_frame_buffer;
    
    float m_near_plane;
    float m_far_plane;
    float m_distance;
    glm::vec2 m_resolution;

    glm::mat4 m_light_projection;
    glm::mat4 m_light_view;
    glm::mat4 m_light_projection_view;
};

#endif //SHADOW_MAP_H