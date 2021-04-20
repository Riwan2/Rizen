#ifndef SKYBOX_H
#define SKYBOX_H

#include "../opengl/texture.h"
#include "../opengl/shader.h"
#include "../opengl/camera.h"

class SkyBox {
public:
    ~SkyBox();
    
    void init(Shader* shader, const std::vector<std::string>& faces_filename);
    void render(Camera* camera);

private:
    GLuint m_vao;
    int m_num_vertices;
    CubeMap* m_cube_map;
    Shader* m_shader;

	void init_mesh();
};

#endif //SKYBOX_H