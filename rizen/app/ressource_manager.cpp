#include "ressource_manager.h"

RessourceManager::RessourceManager(Renderer* renderer)
{
    m_renderer = renderer;
}

RessourceManager::~RessourceManager()
{
    for (auto pair : m_shaders)
        delete pair.second;
    for (auto pair : m_textures)
        delete pair.second;
    for (auto pair : m_materials)
        delete pair.second;
    for (auto pair : m_meshes)
        delete pair.second;
    for (auto pair : m_models)
        delete pair.second;

    m_shaders.clear();
    m_textures.clear();
    m_materials.clear();
    m_meshes.clear();
    m_models.clear();
}

/*
    Add Ressource
*/

void RessourceManager::add_shader(const std::string& name, Shader* shader)
{
    assert(m_shaders.find(name) == m_shaders.end() || "Shader already existing");
    m_shaders[name] = shader;
    m_renderer->bind_ubo(shader);
}

void RessourceManager::add_shader(const std::string& name, const std::string& vr_filename, const std::string& fa_filename)
{
    assert(m_shaders.find(name) == m_shaders.end() || "Shader already existing");
    Shader* shader = new Shader();
    shader->init(vr_filename, fa_filename);
    m_shaders[name] = shader;
    m_renderer->bind_ubo(shader);
}

void RessourceManager::add_texture(const std::string& name, Texture* texture)
{
    assert(m_textures.find(name) == m_textures.end() || "Texture already existing");
    m_textures[name] = texture;
}

void RessourceManager::add_texture_jpg(const std::string& name, const std::string& filename)
{
    assert(m_textures.find(name) == m_textures.end() || "Texture already existing");
    Texture* texture = new Texture();
    texture->init_jpg(filename);
    m_textures[name] = texture;
}

void RessourceManager::add_material(const std::string& name, Material* material)
{
    assert(m_materials.find(name) == m_materials.end() || "Material already existing");
    m_materials[name] = material;
}

void RessourceManager::add_material(const std::string& name, Shader* shader, const MaterialInfo& material_info)
{
    assert(m_materials.find(name) == m_materials.end() || "Material already existing");
    Material* material = new Material();
    material->init(shader, material_info);
    m_materials[name] = material;
}


void RessourceManager::add_mesh(const std::string& name, Mesh* mesh)
{
    assert(m_meshes.find(name) == m_meshes.end() || "Mesh already existing");
    m_meshes[name] = mesh;
}

void RessourceManager::add_mesh(const std::string& name, const std::string& filename)
{
    assert(m_meshes.find(name) == m_meshes.end() || "Mesh already existing");
    Mesh* mesh = new Mesh();
    mesh->init(filename);
    m_meshes[name] = mesh;
}


void RessourceManager::add_model(const std::string& name, Model* model)
{
    assert(m_models.find(name) == m_models.end() || "Model already existing");
    m_models[name] = model;
}

void RessourceManager::add_model(const std::string& name, const std::string& mesh_name, const std::string& material_name, bool instanced)
{
    assert(m_models.find(name) == m_models.end() || "Model already existing");
    Model* model = new Model();
    if (instanced)
        model->init_instanced(mesh(mesh_name), material(material_name));
    else
        model->init(mesh(mesh_name), material(material_name));
    m_models[name] = model;
}


/*
    Getters
*/

Shader* RessourceManager::shader(const std::string& name)
{
    assert(m_models.find(name) != m_models.end() || "Model doesn't exist");
    return m_shaders[name];
}

Texture* RessourceManager::texture(const std::string& name)
{
    assert(m_textures.find(name) != m_textures.end() || "Texture doesn't exist");
    return m_textures[name];
}

Material* RessourceManager::material(const std::string& name)
{
    assert(m_materials.find(name) != m_materials.end() || "Material doesn't exist");
    return m_materials[name];
}

Mesh* RessourceManager::mesh(const std::string& name)
{
    assert(m_meshes.find(name) != m_meshes.end() || "Mesh doesn't exist");
    return m_meshes[name];
}

Model* RessourceManager::model(const std::string& name)
{
    assert(m_models.find(name) != m_models.end() || "Model doesn't exist");
    return m_models[name];
}