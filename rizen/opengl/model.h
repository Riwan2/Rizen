#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <fstream>
#include <sstream>

#include "../utils/log.h"
#include "../utils/rizen_time.h"
#include "shader.h"
#include "texture.h"

const std::string MODEL_PATH = "../models/";
//#define LOADING_INFO

/*
    Vertex & Triangle index
*/

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

struct TriangleIndex {
    GLuint index1;
    GLuint index2;
    GLuint index3;
};

/*
    Mesh
*/

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool init(Vertex* vertices, int num_vertices, TriangleIndex* indices, int num_indices);
    bool init(const std::string& p_filename);
    void render();
    void render_instanced(int num_entities);

    const GLuint vao() const { return m_vao; }
    const int height() const { return m_height; }
    
private:
    GLuint m_vao;
    int m_num_vertices;
    int m_num_indices;
    int m_height = 0;
};

/*
    Material
*/

struct MaterialInfo {
    Texture* texture = nullptr;
    glm::vec4 color = glm::vec4(1.0);
    float shine_damper = 8.0;
    float reflectivity = 1.0;
    float ambient = 0.5;
};

class Material {
public:
    Material();
    ~Material();

    bool init(Shader* shader, float ambient = 0.3, float reflectivity = 1.0, float shine_damper = 8.0,
              const glm::vec4& color = glm::vec4(1.0));
    void init(Shader* shader, const MaterialInfo& info);
    void populate();

    void set_texture(Texture* texture);

    void set_ambient(float ambient) { m_ambient = ambient; }
    void set_color(const glm::vec4& color) { m_color = color; }
    void set_shine_damper(float shine_damper) { m_shine_damper = shine_damper; }
    void set_reflectivity(float reflectivity) { m_reflectivity = reflectivity; }

    const bool textured() { return (m_texture != nullptr); } 
    Texture* texture() const { return m_texture; }
    Shader* shader() const { return m_shader; }

    const glm::vec4& color() const { return m_color; }

private:
    Texture* m_texture;
    Shader* m_shader;
    glm::vec4 m_color;
    float m_shine_damper;
	float m_ambient;
	float m_reflectivity;
};

/*
    Model
*/

class Model {
public:
    Model();
    ~Model();

    void init(Mesh* mesh, Material* material);
    void init_instanced(Mesh* mesh, Material* material);

    void set_mesh(Mesh* mesh) { m_mesh = mesh; }

    Mesh* mesh() const { return m_mesh; }
    Material* material() const { return m_material; }

    const bool instanced() const { return m_instanced; }
    const GLuint instanced_vbo() const { return m_inst_vbo; }

private:
    Mesh* m_mesh;
    Material* m_material;
    bool m_instanced;
    GLuint m_inst_vbo;
};

#endif //MODEL_H