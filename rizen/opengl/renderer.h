// #ifndef RENDERER_H
// #define RENDERER_H

// //#include "../entity/entity.h"
// #include "camera.h"

// #include <unordered_map>

// /*
//     Renderer
// */

// #define MATRICES_SIZE 64
// #define MATRICES_INDEX 0

// class Renderer {
// public:
//     Renderer();
//     ~Renderer();

//     bool init();
//     void begin(Camera* camera);
//     void render(const std::vector<Entity*>& entities);
//     void end();

//     void bind_ubo(Material* material);

// private:
//     Shader* m_shader;
//     std::unordered_map<Model*, std::vector<Entity*>> m_render_map;
//     GLuint m_ubo;

//     void set_ubo();
//     void render(Model* model, const std::vector<Entity*> batch);
//     void render_instanced(Model* model, const std::vector<Entity*> batch);
// };

// #endif //RENDERER_H