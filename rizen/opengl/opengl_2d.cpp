#include "opengl_2d.h"

/*
    Mesh2D
*/

Mesh2D::Mesh2D() {}
Mesh2D::~Mesh2D()  
{
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh2D::init(glm::vec2* vertices, int numVertices, GLuint* indices, int numIndices)
{
    m_numVertices = numVertices;
    m_numIndices = numIndices;
    
    GLuint vbo, ebo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec2), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh2D::render()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
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

/*
    Shapes: Square
*/

void create_square_mesh(Mesh2D* mesh)
{
    const int numVertices = 4;
    const int numIndices = 6;

    glm::vec2 vertices[numVertices] {
        glm::vec2(-0.5,  0.5),
        glm::vec2(-0.5, -0.5),
        glm::vec2( 0.5, -0.5),
        glm::vec2( 0.5,  0.5),
    };

    GLuint indices[numIndices] = {
        0, 1, 2,
        2, 3, 0,
    };

    mesh->init(vertices, numVertices, indices, numIndices);
}

/*
    Renderer2D
*/

Renderer2D::Renderer2D() {}
Renderer2D::~Renderer2D() 
{
    delete m_square;
}

void Renderer2D::init() 
{
    m_projection = glm::mat4(1.0);
    m_square = new Mesh2D();
    create_square_mesh(m_square);
}

void Renderer2D::begin(const glm::vec2& displaySize, Shader* shader) 
{
    glDisable(GL_CULL_FACE);
    m_projection = glm::ortho(0.0f, displaySize.x, displaySize.y, 0.0f, -1.0f, 1.0f);

    m_shader = shader;
    m_shader->bind();
    m_shader->set_mat4("projection", m_projection);
}

void Renderer2D::renderSquare(const Square& square) 
{
    m_shader->set_mat4("model", square.transform.model);
    m_shader->set_vec3("color", square.color);
    m_square->render();
}

void Renderer2D::end() 
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

