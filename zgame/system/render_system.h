#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../component/transform.h"
#include "../component/renderable.h"

#include "../../rizen/opengl/camera.h"
#include "../../rizen/utils/rizen_time.h"

#include "../../extern/entt.h"

#include <queue>
//#include <unordered_map>

class RenderSystem {
public:
    void init();
    void render(Camera* camera, entt::registry& registry);

    void bind_ubo(Material* material);

private:
    GLuint m_ubo;
    std::unordered_map<Model*, std::queue<Transform*>> m_render_map;

    void set_ubo();
    void begin(Camera* camera);
    void end();

    void render(Model* model, std::queue<Transform*>& batch);
    void render_instanced(Model* model, std::queue<Transform*>& batch);
};

#endif //RENDER_SYSTEM_H