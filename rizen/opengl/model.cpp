#include "model.h"

/*
    Mesh
*/

Mesh::Mesh() {}
Mesh::~Mesh() 
{
    glDeleteVertexArrays(1, &m_vao);
}

bool Mesh::init(Vertex* vertices, int num_vertices, TriangleIndex* indices, int num_indices)
{
    m_num_vertices = num_vertices;
    m_num_indices = num_indices * 3;
    
    GLuint vbo, ebo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::pos)));

    // texture coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::pos) + sizeof(Vertex::normal)));

    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    return true;
}

/*
    Load from obj file
*/

struct FaceIndex {
	int pos_index;
	int tex_index;
	int norm_index;
};

bool Mesh::init(const std::string& p_filename)
{
    std::string filename = MODEL_PATH + p_filename;
    std::ifstream file(filename);
	if (!file.is_open())
		return rizen_error("obj file opening error: " + filename);

    std::string line;
	bool vertices_created = false;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> tex_coords;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> list_indices;

    Vertex* vertices;
    TriangleIndex* indices;

    // get info from file
    while (std::getline(file, line)) {
		if (line.size() > 64)
            return rizen_error("invalid line length error: " + filename);

		if (line.front() == '#')
		{
            #ifdef LOADING_INFO
			std::cout << "commented area: " << line.substr(2) << "\n";
            #endif
		}
		else if (line.front() == 'o')
		{
            #ifdef LOADING_INFO
			std::cout << "obj name: " << line.substr(2) << "\n";
            #endif
		}
		else if (line.front() == 's')
		{
            #ifdef LOADING_INFO
			std::cout << "smooth mode: " << line.substr(2) << "\n";
            #endif
		}
		else if (line.substr(0, 2) == "v ")
		{
			std::istringstream stream(line.substr(2));
			glm::vec3 pos;
			stream >> pos.x >> pos.y >> pos.z;
			if (stream.fail())
                return rizen_error("invalid position error: " + filename);
			positions.push_back(pos);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream stream(line.substr(3));
			glm::vec2 tex;
			stream >> tex.x >> tex.y;
			if (stream.fail())
                return rizen_error("invalid texture coordinate error: " + filename);
			tex_coords.push_back(tex);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream stream(line.substr(3));
			glm::vec3 norm;
			stream >> norm.x >> norm.y >> norm.z;
			if (stream.fail())
                return rizen_error("invalid normal error: " + filename);
			normals.push_back(norm);
		}
		else if (line.substr(0, 2) == "f ")
		{
			if (!vertices_created) {
				m_num_vertices = positions.size();
				vertices = new Vertex[m_num_vertices];
				vertices_created = true;
			}

			std::istringstream stream(line.substr(2));
			std::string indices_str;

			for (int i = 0; i < 3; i++) {
				std::getline(stream, indices_str, ' ');
				std::istringstream indices_stream(indices_str);
				std::string index_str;

				FaceIndex face_index;
				std::getline(indices_stream, index_str, '/');
				face_index.pos_index = std::stoi(index_str);

				std::getline(indices_stream, index_str, '/');
				face_index.tex_index = std::stoi(index_str);

				std::getline(indices_stream, index_str, '/');
				face_index.norm_index = std::stoi(index_str);

				list_indices.push_back(face_index.pos_index - 1);
				vertices[face_index.pos_index - 1].pos = positions[face_index.pos_index - 1];
				vertices[face_index.pos_index - 1].normal = normals[face_index.norm_index - 1];
				vertices[face_index.pos_index - 1].tex_coord = tex_coords[face_index.tex_index - 1];
			}
		}
		else
		{
            return rizen_error("invalid line format error: " + filename);
		}
	}

    // There is 3 indices in a triangle
    m_num_indices = list_indices.size();
	int num_triangle_indices = m_num_indices / 3;
	indices = new TriangleIndex[num_triangle_indices];

	for (int i = 0; i < num_triangle_indices; i++) {
		int index = i * 3;
		indices[i].index1 = list_indices[index];
		indices[i].index2 = list_indices[++index];
        indices[i].index3 = list_indices[++index];
	}

    init(vertices, m_num_vertices, indices, m_num_indices);

    return true;
}

void Mesh::render()
{
    glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}