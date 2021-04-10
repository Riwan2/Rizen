#ifndef OPENGL_2D_H
#define OPENGL_2D_H

#include "shader.h"
#include "texture.h"
#include "../utils/rizen_time.h"

/*
    Mesh2D
*/

class Mesh2D {
public:
    Mesh2D();
    ~Mesh2D();

    void init(glm::vec2* vertices, int numVertices, GLuint* indices, int numIndices);

    virtual void render();
    const int num_indices() const { return m_num_indices; }
    const int vao() const { return m_vao; }

protected:
    GLuint m_vao;
    int m_num_indices;
    int m_num_vertices;
};

/*
    Transform2D
*/

struct Transform2D {
    Transform2D();
    void update();
    // rotation takes long time
    void update_rotated();

    glm::mat4 model;
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 origin;
    float angle;
};

/*
    Shapes2D
*/

struct Shape2D {
    void update_transform() { transform.update(); }
    void update_rotated() { transform.update_rotated(); }

    void set_pos(const glm::vec2& pos) { transform.pos = pos; }
    void set_angle(const float& angle) { transform.angle = angle; }
    void set_size(const glm::vec2& size) { transform.size = size; }
    void set_origin(const glm::vec2& origin) { transform.origin = origin; }
    
    void move(const glm::vec2& pos) { transform.pos += pos; }
    void rotate(const float& angle ) { transform.angle += angle; }
    void scale(const glm::vec2& scale) { transform.size *= scale; }

    const float& angle() const { return transform.angle; }
    const glm::vec2& pos() const { return transform.pos; }
    const glm::vec2& size() const { return transform.size; }
    const glm::mat4& model() const { return transform.model; }
    const glm::vec2& origin() const { return transform.origin; }

    Transform2D transform;
};

/*
    Shapes2D: Square
*/

struct Square : public Shape2D {
    glm::vec4 color = glm::vec4(1.0);
};

void create_square_mesh(Mesh2D* mesh);

/*
    Renderer2D
*/

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();

    void init();
    void begin(const glm::vec2& display_size, Shader* shader);
    void render_square(const Square& square, Texture* texture = nullptr);
    void render_instanced_squares(Square* squares, int num_squares, Texture* texture = nullptr);
    void end();
    
private:
    Mesh2D* m_square;
    GLuint m_inst_square_vbo;
    GLuint m_squares_color_vbo;
    Shader* m_shader;
    glm::mat4 m_projection;

    void init_inst_square();
};

#endif //OPENGL_2D_H