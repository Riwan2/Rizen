#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/opengl/camera.h"
#include "../../rizen/utils/rizen_time.h"

#include "../../extern/entt/entity/registry.hpp"

#include <queue>
#include <unordered_map>

class RenderSystem {
public:
    void init();
    void render(Camera* camera, entt::registry& registry);

    void bind_ubo(Material* material);

private:
    GLuint m_ubo;
    std::unordered_map<Model*, std::queue<TransformComponent*>> m_render_map;

    void set_ubo();
    void begin(Camera* camera);
    void end();

    void render(Model* model, std::queue<TransformComponent*>& batch);
    void render_instanced(Model* model, std::queue<TransformComponent*>& batch);
};

#endif //RENDER_SYSTEM_H