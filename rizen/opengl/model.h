#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <fstream>
#include <sstream>

#include "../utils/log.h"
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
    
private:
    GLuint m_vao;
    int m_num_vertices;
    int m_num_indices;
};

/*
    Material
*/

/*
    Model
*/

#endif //MODEL_H