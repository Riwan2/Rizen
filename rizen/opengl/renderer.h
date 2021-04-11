#ifndef RENDERER_H
#define RENDERER_H

#include "../entity/entity.h"
#include "shader.h"
#include "camera.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void render();
    void begin(Camera* camera, Shader* shader);
    void render_mesh(Mesh* mesh);
    void end();

private:
    Shader* m_shader;
};

#endif //RENDERER_H