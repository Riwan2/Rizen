#include "skybox.h"

SkyBox::~SkyBox()
{
    delete m_cube_map;
    glDeleteVertexArrays(1, &m_vao);
}

void SkyBox::init(Shader* shader, const std::vector<std::string>& faces_filename)
{
    m_cube_map = new CubeMap();
    m_cube_map->init_png(faces_filename);

    init_mesh();

    m_shader = shader;
}

void SkyBox::render(Camera* camera, float night_factor)
{
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    m_shader->bind();
    glm::mat4 view = glm::mat4(glm::mat3(camera->view()));
	
	static float rotate = 0;
	static float last = 0;
	static float factor = 0;
	
	last = factor;
	factor = night_factor;

	if (factor > last)
		rotate = night_factor;

    view = glm::rotate(view, rotate, glm::vec3(0, 1, 0));

    m_shader->set_mat4("projection", camera->projection());
    m_shader->set_mat4("view", view);
	m_shader->set_float("u_time", Time::time_sec());
	m_shader->set_float("night_factor", factor);

    m_cube_map->bind();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_num_vertices);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);
}

void SkyBox::init_mesh()
{
	float size = 1.0;
	const int num_floats = 108;
    m_num_vertices = num_floats / 3;

	float* vertices = new float[num_floats] {
		-size,  size, -size,
		-size, -size, -size,
		 size, -size, -size,
		 size, -size, -size,
		 size,  size, -size,
		-size,  size, -size,

		-size, -size,  size,
		-size, -size, -size,
		-size,  size, -size,
		-size,  size, -size,
		-size,  size,  size,
		-size, -size,  size,

		 size, -size, -size,
		 size, -size,  size,
		 size,  size,  size,
		 size,  size,  size,
		 size,  size, -size,
		 size, -size, -size,

		-size, -size,  size,
		-size,  size,  size,
		 size,  size,  size,
		 size,  size,  size,
		 size, -size,  size,
		-size, -size,  size,

		-size,  size, -size,
		 size,  size, -size,
		 size,  size,  size,
		 size,  size,  size,
		-size,  size,  size,
		-size,  size, -size,

		-size, -size, -size,
		-size, -size,  size,
		 size, -size, -size,
		 size, -size, -size,
		-size, -size,  size,
		 size, -size,  size
	};

	GLuint vbo;
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(m_vao);
		
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, num_floats * sizeof(float), vertices, GL_STATIC_DRAW);
		
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			
	glBindVertexArray(0);
	
	delete[] vertices;
	glDeleteBuffers(1, &vbo);
}