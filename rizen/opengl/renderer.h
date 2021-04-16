#ifndef RENDERER_H
#define RENDERER_H

#include <queue>
#include <unordered_map>

#include "camera.h"
#include "model.h"

/*
    Renderer
*/

#define MATRICES_SIZE 64
#define MATRICES_INDEX 0

class Renderer {
public:
    ~Renderer();

    /**
     * @brief Init the uniform buffer object
     */
    void init();
    
    /**
     * @brief Bind the uniform buffer to the material shader
     * @param material
     */
    void bind_ubo(Material* material);

    /**
     * @brief Begin rendering sequence.
     * @param camera
     */
    void begin(Camera* camera);

    /**
     * @brief End rendering sequence.
     */
    void end();
    

    /**
     * @brief Simply render the object and bind the transform matrix.
     * @param render_model
     * @param model
     */
    void simple_render(Model* render_model, const glm::mat4& model);

    /**
     * @brief Render the object with the render model and material.
     * @param render_model
     * @param model
     */
    void render(Model* render_model, const glm::mat4& model);

    /**
    * @brief Simply render the instanced object and bind the transform matrices.
    * @param render_model
    * @param models array of model matrices
    * @param num_models num of model matrices
    */
    void simple_render_instanced(Model* render_model, glm::mat4* models, int num_models);
    
    /**
     * @brief Render the instanced object with the render model and material.
     * @param render_model
     * @param models array of model matrices
     * @param num_models num of model matrices
     */
    void render_instanced(Model* render_model, glm::mat4* models, int num_models);

private:
    GLuint m_ubo;

    void set_ubo();
};

#endif //RENDERER_H