#ifndef RENDERER_H
#define RENDERER_H

#include "../entity/entity.h"
#include "camera.h"

#include <map>

/*
    Renderer
*/

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void begin(Camera* camera, Shader* shader);
    void render(Entity* model, Camera* camera);
    void render(const std::vector<Entity*>& entities, Camera* camera);
    void end();

private:
    Shader* m_shader;
    std::map<Material*, std::vector<Entity*>> m_render_map;
};

#endif //RENDERER_H