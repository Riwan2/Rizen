#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H

#include "../opengl/renderer.h"

class RessourceManager {
public:
    RessourceManager(Renderer* renderer);
    ~RessourceManager();

    void add_shader(const std::string& name, Shader* shader);
    void add_shader(const std::string& name, const std::string& vr_filename, const std::string& fa_filename);

    void add_texture(const std::string& name, Texture* texture);
    void add_texture_jpg(const std::string& name, const std::string& filename);

    void add_material(const std::string& name, Material* material);
    void add_material(const std::string& name, Shader* shader, const MaterialInfo& material_info);

    void add_mesh(const std::string& name, Mesh* mesh);
    void add_mesh(const std::string& name, const std::string& filename);

    void add_model(const std::string& name, Model* model);
    void add_model(const std::string& name, const std::string& mesh_name, const std::string& material_name, bool instanced = false);

    Shader* shader(const std::string& name);
    Texture* texture(const std::string& name);
    Material* material(const std::string& name);
    Mesh* mesh(const std::string& name);
    Model* model(const std::string& name);

private:
    std::unordered_map<std::string, Shader*> m_shaders;
    std::unordered_map<std::string, Texture*> m_textures;
    std::unordered_map<std::string, Material*> m_materials;
    std::unordered_map<std::string, Mesh*> m_meshes;
    std::unordered_map<std::string, Model*> m_models;

    Renderer* m_renderer;
};

#endif //RESSOURCE_MANAGER_H