#include "ecs.h"

/*
    Entity Manager
*/

void EntityManager::init() 
{
    for (Entity i = 0; i < MAX_ENTITIES; i++)
        m_free_entities.push(i);
}

Entity EntityManager::create_entity()
{
    if (m_num_entities >= MAX_ENTITIES) { std::cout << "too many entity\n"; abort(); }

    m_num_entities++;
    Entity entity = m_free_entities.front();
    m_free_entities.pop();
    return entity;
}

void EntityManager::destroy_entity(Entity entity)
{
    if (entity >= MAX_ENTITIES) { std::cout << "entity out of range\n"; abort(); }

    m_num_entities--;
    m_free_entities.push(entity);
    m_signatures[entity].reset();
}

void EntityManager::set_signature(Entity entity, Signature signature)
{
    if (entity >= MAX_ENTITIES) { std::cout << "entity out of range\n"; abort(); }
    m_signatures[entity] = signature;
}

Signature EntityManager::get_signature(Entity entity)
{
    if (entity >= MAX_ENTITIES) { std::cout << "entity out of range\n"; abort(); }
    return m_signatures[entity];
}

/*
    Component Manager
*/

ComponentManager::~ComponentManager() 
{
    for (auto element : m_component_arrays)
        delete element.second;
}

void ComponentManager::entity_destroyed(Entity entity) 
{
    for (auto pair : m_component_arrays)
        pair.second->entity_destroyed(entity);
}

/*
    System manager
*/

SystemManager:: ~SystemManager()
{
    for (auto pair : m_systems)
        delete pair.second;
}

void SystemManager::entity_destroyed(Entity entity) 
{
    for (auto pair : m_systems)
        pair.second->m_entities.erase(entity);
}

void SystemManager::entity_signature_changed(Entity entity, Signature entity_signature) 
{
    for (auto pair : m_systems) {
        auto name = pair.first;
        auto system = pair.second;
        auto system_signature = m_system_signatures[name];

        if ((entity_signature & system_signature) == system_signature)
            system->m_entities.insert(entity);
        else
            system->m_entities.erase(entity);
    }
}

/*
    Coordinator
*/

EntityManager* Coordinator::m_entities;
ComponentManager* Coordinator::m_components;
SystemManager* Coordinator::m_systems;

void Coordinator::clean_up() 
{
    delete m_entities;
    delete m_components;
    delete m_systems;
}

void Coordinator::init() 
{
    m_entities = new EntityManager();
    m_components = new ComponentManager();
    m_systems = new SystemManager();
    m_entities->init();
}

Entity Coordinator::create_entity()
{
    return m_entities->create_entity();
}

void Coordinator::destroy_entity(Entity entity)
{
    m_entities->destroy_entity(entity);
    m_components->entity_destroyed(entity);
    m_systems->entity_destroyed(entity);
}
