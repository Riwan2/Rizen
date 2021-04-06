#ifndef OPENGL_2D_H
#define OPENGL_2D_H

#include "shader.h"

/*
    Mesh2D
*/

class Mesh2D {
public:
    Mesh2D();
    ~Mesh2D();

    void init(glm::vec2* vertices, int numVertices, GLuint* indices, int numIndices);
    virtual void render();

protected:
    GLuint m_vao;
    int m_numIndices;
    int m_numVertices;
};

/*
    Transform2D
*/

struct Transform2D {
    Transform2D();
    void update();

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
    glm::vec3 color;
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
    void begin(const glm::vec2& displaySize, Shader* shader);
    void renderSquare(const Square& square);
    void end();
    
private:
    Mesh2D* m_square;
    Shader* m_shader;
    glm::mat4 m_projection;
};

#endif //OPENGL_2D_H