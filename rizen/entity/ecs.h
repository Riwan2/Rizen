#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <queue>
#include <bitset>
#include <unordered_map>
#include <set>

#include "../utils/log.h"

// 2 ^ 32 Entities
typedef uint32_t Entity;
const Entity MAX_ENTITIES = 100000;

// 2 ¨ 8 Components
typedef uint8_t ComponentType;
const ComponentType MAX_COMPONENTS = 10;

// Signature
typedef std::bitset<MAX_COMPONENTS> Signature;

// Entity Manager
class EntityManager {
public:
    void init();
    Entity create_entity();
    void destroy_entity(Entity entity);
    void set_signature(Entity entity, Signature signature);
    Signature get_signature(Entity entity);

private:
    int m_num_entities = 0;
    std::queue<Entity> m_free_entities;
    Signature m_signatures[MAX_ENTITIES];
};

/* 
    Interface Component Array
*/

class IComponentArray {
public:
    virtual ~IComponentArray() {}
    virtual void entity_destroyed(Entity entity) = 0;
};

/*
    Component Array
*/

template <class T>
class ComponentArray : public IComponentArray {
public:
    void add_component(Entity entity, const T& component);
    void remove_component(Entity entity);
    T* get_component(Entity entity);
    void entity_destroyed(Entity entity) override;

private:
    int m_num_components = 0;
    T m_components[MAX_ENTITIES];
    std::unordered_map<int, Entity> m_index_to_entity;
    std::unordered_map<Entity, int> m_entity_to_index;
};

/*
    Component Manager
*/

class ComponentManager {
public:
    ~ComponentManager();

    template <class T>
    void register_component();
    
    template <class T>
    ComponentType get_component_type();
    
    template <class T>
    void add_component(Entity entity, T component);
    
    template <class T>
    void remove_component(Entity entity);
    
    template <class T>
    T* get_component(Entity entity);

    void entity_destroyed(Entity entity);

private:
    ComponentType m_next_component_type = 0;
    std::unordered_map<const char*, IComponentArray*> m_component_arrays;
    std::unordered_map<const char*, ComponentType> m_component_types;

    template <class T>
    ComponentArray<T>* get_component_array();
};

/*
    System
*/

class System {
public:
    std::set<Entity> m_entities;
};

/*
    System manager
*/

class SystemManager {
public:
    ~SystemManager();

    template <class T>
    T* register_system();

    template <class T>
    void set_signature(Signature signature);

    void entity_destroyed(Entity entity) ;
    void entity_signature_changed(Entity entity, Signature entity_signature);

private:
    std::unordered_map<const char*, System*> m_systems;
    std::unordered_map<const char*, Signature> m_system_signatures;
};

/*
    Coordinator
*/

class Coordinator {
public:
    static void init();
    static void clean_up();
    static Entity create_entity();
    static void destroy_entity(Entity entity);

    template <class T>
    static void register_component();

    template <class T>
    static void add_component(Entity entity, T component);

    template <class T>
    static void remove_component(Entity entity, T component);

    template <class T>
    static T* get_component(Entity entity);

    template <class T>
    static ComponentType get_component_type();

    template <class T>
    static T* register_system();

    template <class T>
    static void set_system_signature(Signature signature);

private:
    static EntityManager* m_entities;
    static ComponentManager* m_components;
    static SystemManager* m_systems;
};

#include "ecs.tpp"


#endif //ECS_H