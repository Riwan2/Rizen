#include "model.h"

/*
	Material
*/

Material::Material() {}
Material::~Material() {}

bool Material::init(Shader* shader, float ambient, float reflectivity, float shine_damper, const glm::vec4& color) 
{
	m_shader = shader;
	m_color = color;
	m_ambient = ambient;
	m_reflectivity = reflectivity;
	m_shine_damper = shine_damper;
	return true;
}

void Material::set_texture(Texture* texture)
{
	m_texture = texture;
}

void Material::populate()
{
	m_shader->set_bool("textured", textured());	
	m_shader->set_vec4("color", m_color);
	m_shader->set_float("ambient", m_ambient);
	m_shader->set_float("reflectivity", m_reflectivity);
	m_shader->set_float("shine_damper", m_shine_damper);
}

/*
	Model
*/

Model::Model() {}
Model::~Model() {}

void Model::init(Mesh* mesh, Material* material) 
{
	m_instanced = false;
	m_mesh = mesh;
	m_material = material;
}

void Model::init_instanced(Mesh* mesh, Material* material)
{
	m_instanced = true;
	m_mesh = mesh;
	m_material = material;

	glBindVertexArray(m_mesh->vao());

    glGenBuffers(1, &m_inst_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_inst_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

    std::size_t s_vec4 = sizeof(glm::vec4);    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)(1 * s_vec4));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)(2 * s_vec4));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * s_vec4, (void*)(3 * s_vec4));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}


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
    m_num_indices = num_indices;
    
    GLuint vbo, ebo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(TriangleIndex), indices, GL_STATIC_DRAW);

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

	int min = 99999;
	int max = -1;

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

				glm::vec3 position = positions[face_index.pos_index - 1];
				if (position.y <= min)
					min = position.y;

				if (position.y >= max)
					max = position.y;

				vertices[face_index.pos_index - 1].pos = position;
				vertices[face_index.pos_index - 1].normal = normals[face_index.norm_index - 1];
				vertices[face_index.pos_index - 1].tex_coord = tex_coords[face_index.tex_index - 1];
			}
		}
		else
		{
            return rizen_error("invalid line format error: " + filename);
		}
	}
	
	// set mesh height
	m_height = max - min;	

	#ifdef LOADING_INFO
	std::cout << "min: " << min << " | ";
	std::cout << "max: " << max << " | ";
	std::cout << "height: " << m_height << "\n";
	#endif

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
	delete[] indices;
	delete[] vertices;
	
    return true;
}

void Mesh::render()
{
    glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_num_indices * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::render_instanced(int num_entities)
{
    glBindVertexArray(m_vao);
    glDrawElementsInstanced(GL_TRIANGLES, m_num_indices * 3, GL_UNSIGNED_INT, 0, num_entities);
    glBindVertexArray(0);
}