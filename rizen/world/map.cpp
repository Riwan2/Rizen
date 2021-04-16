#include "map.h"

Map::~Map()
{
    delete m_mesh;
    delete m_model;
}

void Map::init(Material* material)
{
    m_size = glm::vec2(20, 20);
    m_resolution = glm::vec2(3, 3);
    m_position = glm::vec2(-10, 0);

    m_vertices_distance = m_size / (m_resolution - glm::vec2(1.0));

    int num_vertices = m_resolution.x * m_resolution.y;
    // 2 triangles per quad
    int num_triangle_indices = (m_resolution.x - 1) * (m_resolution.y - 1) * 2;

    Vertex* vertices = new Vertex[num_vertices];
    TriangleIndex* triangle_indices = new TriangleIndex[num_triangle_indices];

    int triangle_index = 0;

    for (int y = 0; y < m_resolution.y; y++) {
        for (int x = 0; x < m_resolution.x; x++) {
            int index = y * m_resolution.x + x;

            vertices[index].pos.x = m_position.x + m_vertices_distance.x * x;
            vertices[index].pos.y = 0;
            vertices[index].pos.z = m_position.y + m_vertices_distance.y * y;

            vertices[index].normal.x = 0;
            vertices[index].normal.y = 0;
            vertices[index].normal.z = 0;

            vertices[index].tex_coord.x = (1 / m_resolution.x) * x;
            vertices[index].tex_coord.y = (1 / m_resolution.y) * y;

            if (x < m_resolution.x - 1 && y < m_resolution.y - 1) {
                int top_left = index;
                int top_right = top_left + 1;
                int bot_left = top_left + m_resolution.x;
                int bot_right = bot_left + 1;

                triangle_indices[triangle_index].index1 = top_right;
                triangle_indices[triangle_index].index2 = top_left;
                triangle_indices[triangle_index].index3 = bot_left;
                
                triangle_index++;

                triangle_indices[triangle_index].index1 = bot_left;
                triangle_indices[triangle_index].index2 = bot_right;
                triangle_indices[triangle_index].index3 = top_right;

                triangle_index++;
            }
        }
    }

    m_mesh = new Mesh();

    m_mesh->init(vertices, num_vertices, triangle_indices, num_triangle_indices);

    m_model = new Model();
    m_model->init(m_mesh, material);

    delete[] vertices;
    delete[] triangle_indices;    
}

// void Map::render(Shader* shader)
// {
//     glDisable(GL_CULL_FACE);
//     m_mesh->render();
//     glEnable(GL_CULL_FACE);
// }