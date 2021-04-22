#include "map.h"

/*
    Height Map
*/

HeightMap::~HeightMap()
{
    delete[] m_heights;
}

void HeightMap::init(const glm::vec2& size)
{
    m_size = size;
    m_num_heights = m_size.x * m_size.y;
    m_heights = new float[m_num_heights];

    for (int y = 0; y < m_size.y; y++) {
        for (int x = 0; x < m_size.x; x++) {
            int index = y * m_size.x + x;
            m_heights[index] = 0;
        }
    }
}

void HeightMap::generate_random(float octave, float roughness, float amplitude)
{
    m_seed = rand_int(0, 10000000);
    set_seed(m_seed);
    for (int y = 0; y < m_size.y; y++) {
        for (int x = 0; x < m_size.x; x++) {
            int index = y * m_size.x + x;
            m_heights[index] = generate_height(x, y, octave, roughness, amplitude);
        }
    }
}

float HeightMap::get_height(int x, int y)
{
    if (x < 0 || x > m_size.x || y < 0 || y > m_size.y) return 0;
    int index = y * m_size.x + x;
    return m_heights[index];
}

/*
    Noise
*/

float HeightMap::generate_height(int x, int y, float octave, float roughness, float amplitude)
{
    float total = 0;
    float d = pow(2, octave - 1);

    for (int i = 0; i < octave; i++) {
        float freq = pow(2, i) / d;
        float amp = pow(roughness, i) * amplitude;
        total += interpolated_noise(x * freq, y * freq) * amp;
    }
    //return interpolated_noise(x / octave, y / octave) * amplitude - amplitude / 2;
    return total - amplitude / 2;
}

float HeightMap::interpolated_noise(float x, float y)
{
    int int_x = int(x);
    int int_y = int(y);
    float frac_x = x - int_x;
    float frac_y = y - int_y;

    float v1 = smooth(int_x, int_y);
    float v2 = smooth(int_x + 1, int_y);
    float v3 = smooth(int_x, int_y + 1);
    float v4 = smooth(int_x + 1, int_y + 1);
    float i1 = interpolate(v1, v2, frac_x);
    float i2 = interpolate(v3, v4, frac_x);

    return interpolate(i1, i2, frac_y);
}

float HeightMap::smooth(int x, int y) {
    float corners = (get_noise(x - 1, y - 1) + get_noise(x + 1, y - 1) 
                    + get_noise(x - 1, y + 1) + get_noise(x + 1, y + 1)) / 16.0f;
    float sides = (get_noise(x - 1, y) + get_noise(x + 1, y) 
                    + get_noise(x, y - 1) + get_noise(x, y + 1)) / 8.0f;
    float center = get_noise(x, y) / 4.0f;
    return corners + sides + center;
}

float HeightMap::interpolate(float a, float b, float blend) 
{
    double theta = blend * PI;
    float f = (float)(1 - cos(theta)) * 0.5;
    return a * (1 - f) + b * f;
}

float HeightMap::get_noise(int x, int y)
{
    int n = (x + m_seed) + (y + m_seed) * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0);
}

/*
    Map
*/

Map::~Map()
{
    delete m_height_map;
    delete m_mesh;
    delete m_render_model;
}

void Map::init(Material* material, const glm::vec2& size, const glm::vec2& resolution, const glm::vec3& position)
{
    m_size = size;
    m_resolution = resolution;
    set_position(position);
    ////set_position(glm::vec3(-m_size.x / 2, -10, -m_size.y / 2));
    m_vertices_distance = m_size / (m_resolution - glm::vec2(1.0));

    m_height_map = new HeightMap();
    m_height_map->init(m_resolution);

    load_mesh();

    m_render_model = new Model();
    m_render_model->init(m_mesh, material);   
}

void Map::generate_random_heightmap(float octave, float roughness, float amplitude)
{
    delete m_mesh;
    m_octave = octave;
    m_amplitude = amplitude;
    m_roughness = roughness;
    m_height_map->generate_random(octave, roughness, amplitude);
    load_mesh();
    m_render_model->set_mesh(m_mesh);
}

void Map::reset_random()
{
    generate_random_heightmap(m_octave, m_roughness, m_amplitude);
}

void Map::render(Renderer* renderer)
{
    glDisable(GL_CULL_FACE);
    renderer->render(m_render_model, m_model);
    glEnable(GL_CULL_FACE);
}

void Map::render_depth(Shader* depth_shader, Renderer* renderer)
{
    glDisable(GL_CULL_FACE);
    renderer->render_depth(depth_shader, m_render_model, m_model);
    glEnable(GL_CULL_FACE);
}

float Map::get_heigth(const glm::vec2& world_pos)
{
    glm::vec2 map_pos = world_pos - glm::vec2(m_position.x, m_position.z);
    if (map_pos.x < 0 || map_pos.y < 0 || map_pos.x > m_size.x || map_pos.y > m_size.y)
        return 0;

    glm::vec2 grid_pos;
    grid_pos.x = floor(map_pos.x / m_vertices_distance.x);
    grid_pos.y = floor(map_pos.y / m_vertices_distance.y);

    glm::vec2 quad_pos;
    quad_pos.x = fmod(map_pos.x, m_vertices_distance.x) / m_vertices_distance.x;
    quad_pos.y = fmod(map_pos.y, m_vertices_distance.y) / m_vertices_distance.y;

    if (quad_pos.x < 1 - quad_pos.y) {
        // top left triangle
        return barry_centric_height(
            glm::vec3(0, get_heightmap(grid_pos.x, grid_pos.y), 0),
            glm::vec3(1, get_heightmap(grid_pos.x + 1, grid_pos.y), 0),
            glm::vec3(0, get_heightmap(grid_pos.x, grid_pos.y + 1), 1),
            quad_pos
        ) + m_position.y;

    } else {
        // bot right triangle
        return barry_centric_height(
            glm::vec3(1, get_heightmap(grid_pos.x + 1, grid_pos.y), 0),
            glm::vec3(0, get_heightmap(grid_pos.x, grid_pos.y + 1), 1),
            glm::vec3(1, get_heightmap(grid_pos.x + 1, grid_pos.y + 1), 1),
            quad_pos
        ) + m_position.y;
    }

    return 0;
}

glm::vec3 Map::get_fast_normal(const glm::vec2& world_pos)
{
    // Fast but not precise normal
    glm::vec2 map_pos = world_pos - glm::vec2(m_position.x, m_position.z);
    if (map_pos.x < 0 || map_pos.y < 0 || map_pos.x > m_size.x || map_pos.y > m_size.y)
        return glm::vec3(0, 1, 0);

    glm::vec2 grid_pos;
    grid_pos.x = floor(map_pos.x / m_vertices_distance.x);
    grid_pos.y = floor(map_pos.y / m_vertices_distance.y);

    float v1, v2, v3, v4;
    float offset = 1.0f;
    v1 = get_heightmap(grid_pos.x - 1, grid_pos.y);
    v2 = get_heightmap(grid_pos.x + 1, grid_pos.y);
    v3 = get_heightmap(grid_pos.x, grid_pos.y - 1);
    v4 = get_heightmap(grid_pos.x, grid_pos.y + 1);

    glm::vec3 normal = glm::vec3(v1 - v2, 2.0, v3 - v4);
    return glm::normalize(normal);
}

glm::vec3 Map::get_precise_normal(const glm::vec2& world_pos, float offset)
{
    // TODO: lot of fmod operation bc of get_height move all the info in this function
    // really precise normal
    float v1, v2, v3, v4;
    glm::vec2 h_offset = glm::vec2(offset, 0);
    glm::vec2 v_offset = glm::vec2(0, offset);

    v1 = get_heigth(world_pos - h_offset);
    v2 = get_heigth(world_pos + h_offset);
    v3 = get_heigth(world_pos - v_offset);
    v4 = get_heigth(world_pos + v_offset);

    glm::vec3 normal = glm::vec3(v1 - v2, 2.0, v3 - v4);
    return glm::normalize(normal);
}

void Map::set_position(const glm::vec3& position)
{
    m_position = position;
    update_model();
}

void Map::move_position(const glm::vec3& position)
{
    m_position += position;
    update_model();
}


void Map::load_mesh()
{
    int num_vertices = m_resolution.x * m_resolution.y;
    // 2 triangles per quad
    int num_triangle_indices = (m_resolution.x - 1) * (m_resolution.y - 1) * 2;

    Vertex* vertices = new Vertex[num_vertices];
    TriangleIndex* triangle_indices = new TriangleIndex[num_triangle_indices];

    int triangle_index = 0;
    for (int y = 0; y < m_resolution.y; y++) {
        for (int x = 0; x < m_resolution.x; x++) {
            int index = y * m_resolution.x + x;

            vertices[index].pos.x = m_vertices_distance.x * x;
            vertices[index].pos.y = get_heightmap(x, y);
            vertices[index].pos.z = m_vertices_distance.y * y;

            glm::vec3 normal = calculate_normal(x, y);
            vertices[index].normal.x = normal.x;
            vertices[index].normal.y = normal.y;
            vertices[index].normal.z = normal.z;

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

    delete[] vertices;
    delete[] triangle_indices; 
}

float Map::get_heightmap(int x, int y)
{
    return m_height_map->get_height(x, y);
}

glm::vec3 Map::calculate_normal(float x, float y)
{
    float v1, v2, v3, v4;
    v1 = m_height_map->get_height(x - 1, y);
    v2 = m_height_map->get_height(x + 1, y);
    v3 = m_height_map->get_height(x, y - 1);
    v4 = m_height_map->get_height(x, y + 1);
    glm::vec3 normal = glm::vec3(v1 - v2, 2.0, v3 - v4);
    return glm::normalize(normal);
}

void Map::update_model()
{
    m_model = glm::mat4(1.0);
    m_model = glm::translate(m_model, m_position);
}