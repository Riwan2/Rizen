#ifndef MAP_H
#define MAP_H

#include "../opengl/model.h"

class Map {
public:
    ~Map();
    void init(Material* material);

    Model* model() const { return m_model; }

private:
    glm::vec2 m_size;
    glm::vec2 m_resolution;
    glm::vec2 m_vertices_distance;
    glm::vec2 m_position;

    Mesh* m_mesh;
    Model* m_model;
};

#endif //MAP_H