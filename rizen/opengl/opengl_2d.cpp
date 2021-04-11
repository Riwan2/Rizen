#include "opengl_2d.h"

/*
    Mesh2D
*/

Mesh2D::Mesh2D() {}
Mesh2D::~Mesh2D()  
{
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh2D::init(glm::vec2* vertices, int num_vertices, GLuint* indices, int num_indices)
{
    m_num_vertices = num_vertices;
    m_num_indices = num_indices;
    
    GLuint vbo, ebo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(glm::vec2), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh2D::render()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/*
    Transform2D
*/

Transform2D::Transform2D() 
{
    model = glm::mat4(1.0);
    pos = glm::vec2(0.0);
    size = glm::vec2(50.0);
    origin = glm::vec2(0.0);
    angle = 0;
}

void Transform2D::update() 
{
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(pos.x + size.x * (0.5 - origin.x), pos.y + size.y * (0.5 - origin.y), 0.0f));
	model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));
}

void Transform2D::update_rotated()
{
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(pos.x + size.x * (0.5 - origin.x), pos.y + size.y * (0.5 - origin.y), 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));
}

/*
    Shapes: Square
*/

void create_square_mesh(Mesh2D* mesh)
{
    const int num_vertices = 4;
    const int num_indices = 6;

    glm::vec2 vertices[num_vertices] {
        glm::vec2(-0.5,  0.5),
        glm::vec2(-0.5, -0.5),
        glm::vec2( 0.5, -0.5),
        glm::vec2( 0.5,  0.5),
    };

    GLuint indices[num_indices] = {
        0, 1, 2,
        2, 3, 0,
    };

    mesh->init(vertices, num_vertices, indices, num_indices);
}

/*
    Renderer2D
*/

Renderer2D::Renderer2D() {}
Renderer2D::~Renderer2D() 
{
    glDeleteBuffers(1, &m_inst_square_vbo);
    glDeleteBuffers(1, &m_squares_color_vbo);
    delete m_square;
}

void Renderer2D::init() 
{
    m_projection = glm::mat4(1.0);
    m_square = new Mesh2D();
    create_square_mesh(m_square);
    init_inst_square();
}

void Renderer2D::init_inst_square()
{
    glBindVertexArray(m_square->vao());

    glGenBuffers(1, &m_inst_square_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_inst_square_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

    std::size_t s_vec4 = sizeof(glm::vec4);
    std::size_t s_vec3 = sizeof(glm::vec3);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)(1 * s_vec4));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)(2 * s_vec4));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)(3 * s_vec4));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glGenBuffers(1, &m_squares_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_squares_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, s_vec4, (void*)0);
    
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);
}

void Renderer2D::begin(const glm::vec2& display_size, Shader* shader) 
{
    glDisable(GL_CULL_FACE);
    m_projection = glm::ortho(0.0f, display_size.x, display_size.y, 0.0f, -1.0f, 1.0f);

    m_shader = shader;
    m_shader->bind();
    m_shader->set_mat4("projection", m_projection);
    m_shader->set_float("u_time", Time::time() / 1000.0);;
    m_shader->set_vec2("u_resolution", display_size);
}

void Renderer2D::render_square(const Square& square, Texture* texture) 
{
    m_shader->set_bool("texture", false);
    if (texture != nullptr) {
        texture->bind();
        m_shader->set_bool("textured", true);
    }

    m_shader->set_mat4("model", square.transform.model);
    m_shader->set_vec4("color", square.color);
    m_square->render();

    if (texture != nullptr)
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer2D::render_instanced_squares(Square* squares, int num_squares, Texture* texture)
{
    glm::mat4* models = new glm::mat4[num_squares];
    glm::vec4* colors = new glm::vec4[num_squares];
    for (int i = 0; i < num_squares; i++) {
        models[i] = squares[i].model();
        colors[i] = squares[i].color;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_inst_square_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_squares * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_squares * sizeof(glm::mat4), models);

    glBindBuffer(GL_ARRAY_BUFFER, m_squares_color_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_squares * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_squares * sizeof(glm::vec4), colors);

    m_shader->set_bool("textured", false);
    if (texture != nullptr) {
        texture->bind();
        m_shader->set_bool("textured", true);
    }

    glBindVertexArray(m_square->vao());
    glDrawElementsInstanced(GL_TRIANGLES, m_square->num_indices(), GL_UNSIGNED_INT, 0, num_squares);
    glBindVertexArray(0);

    if (texture != nullptr)
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer2D::end() 
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

