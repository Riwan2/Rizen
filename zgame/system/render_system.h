#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/opengl/camera.h"
#include "../../rizen/utils/rizen_time.h"

#include "../../rizen/opengl/renderer.h"

#include "../../extern/entt/entity/registry.hpp"

#include <queue>
#include <unordered_map>

static entt::entity renderable_blueprint(entt::registry& registry, Model* model)
{
    entt::entity entity = registry.create();
    registry.emplace<RenderComponent>(entity, model);
    registry.emplace<TransformComponent>(entity);
    return entity;
}

class RenderSystem {
public:
    void render(Renderer* renderer, entt::registry& registry);

private:
    GLuint m_ubo;
    std::unordered_map<Model*, std::queue<TransformComponent*>> m_render_map;

    void render(Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch);
    void render_instanced(Renderer* renderer, Model* model, std::queue<TransformComponent*>& batch);
};

#endif //RENDER_SYSTEM_H