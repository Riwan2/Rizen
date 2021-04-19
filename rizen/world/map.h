#ifndef MAP_H
#define MAP_H

#include "../opengl/renderer.h"
#include "../utils/rizen_math.h"

/*
    Height Map
*/

class HeightMap {
public:
    ~HeightMap();

    void init(const glm::vec2& size);
    void generate_random(float octave, float roughness, float amplitude);
    float get_height(int x, int y);

private:
    float* m_heights;
    int m_num_heights;
    glm::vec2 m_size;
    int m_seed;

    void generate_noise();
    float generate_height(int x, int y, float octave, float roughness, float amplitude);
    float interpolated_noise(float x, float y);
    float smooth(int x, int y);
    float interpolate(float a, float b, float blend);
    float get_noise(int x, int y);
};

/*
    Map
*/

class Map {
public:
    ~Map();
    void init(Material* material, const glm::vec2& size, const glm::vec2& resolution, const glm::vec3& position);
    void generate_random_heightmap(float octave = 10, float roughness = 0.2, float amplitude = 100);
    void reset_random();

    void render(Renderer* renderer);

    float get_heigth(const glm::vec2& world_pos);
    glm::vec3 get_precise_normal(const glm::vec2& world_pos, float offset = 1.0);
    glm::vec3 get_fast_normal(const glm::vec2& world_pos);

    void set_position(const glm::vec3& position);
    void move_position(const glm::vec3& position);
    
    const glm::vec3& position() const { return m_position; }
    const glm::vec2& size() const { return m_size; }

private:
    glm::vec2 m_size;
    glm::vec2 m_resolution;
    glm::vec2 m_vertices_distance;

    glm::vec3 m_position;
    glm::mat4 m_model;

    Mesh* m_mesh;
    HeightMap* m_height_map;
    Model* m_render_model;

    float m_octave;
    float m_amplitude;
    float m_roughness;

    void load_mesh();
    float get_heightmap(int x, int y);
    glm::vec3 calculate_normal(float x, float y);
    void update_model();
};

#endif //MAP_H