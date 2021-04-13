#pragma once

/*
    Component array
*/

template <class T>
void ComponentArray<T>::add_component(Entity entity, const T& component) 
{
    if (m_entity_to_index.find(entity) != m_entity_to_index.end()) { std::cout << "entity already have component\n"; abort(); }

    m_index_to_entity[m_num_components] = entity;
    m_entity_to_index[entity] = m_num_components;
    m_components[m_num_components] = component;
    m_num_components++;
}

template <class T>
void ComponentArray<T>::remove_component(Entity entity)
{
    if (m_entity_to_index.find(entity) == m_entity_to_index.end()) { std::cout << "removing non existant component\n"; abort(); }

    // move last component at remove index
    int last_index = m_num_components - 1;
    int cur_index = m_entity_to_index[entity];
    m_components[cur_index] = m_components[last_index];

    // set last entity for this index
    Entity last_entity = m_index_to_entity[last_index];
    m_entity_to_index[last_entity] = cur_index;
    m_index_to_entity[cur_index] = last_entity;

    // remove current entity who is placed at last index
    m_entity_to_index.erase(entity);
    m_index_to_entity.erase(last_index);
    
    m_num_components--;
}

template <class T>
T* ComponentArray<T>::get_component(Entity entity)
{
    if (m_entity_to_index.find(entity) == m_entity_to_index.end()) { std::cout << "getting non existant component\n"; abort(); }
    int index = m_entity_to_index[entity];
    std::cout << "index: " << index << " | size: " << m_entity_to_index.size() << ", " << m_index_to_entity.size() << "\n";
    return &m_components[index];
}

template <class T>
void ComponentArray<T>::entity_destroyed(Entity entity)
{
    if (m_entity_to_index.find(entity) != m_entity_to_index.end())
        remove_component(entity);
}

/*
    Component Manager
*/

template <class T>
void ComponentManager::register_component()
{
    const char* name = typeid(T).name();
    if (m_component_arrays.find(name) != m_component_arrays.end()) { std::cout << "component already registered\n"; abort(); }

    IComponentArray* array = new ComponentArray<T>();
    m_component_arrays[name] = array;
    m_component_types[name] = m_next_component_type;
    m_next_component_type++;
}

template <class T>
ComponentType ComponentManager::get_component_type() 
{
    const char* name = typeid(T).name();
    if (m_component_arrays.find(name) == m_component_arrays.end()) { std::cout << "component type not registered\n"; abort(); }
    return m_component_types[name];
}

template <class T>
void ComponentManager::add_component(Entity entity, T component)
{
    get_component_array<T>()->add_component(entity, component);
}

template <class T>
void ComponentManager::remove_component(Entity entity)
{
    get_component_array<T>()->remove_component(entity);
}

template <class T>
T* ComponentManager::get_component(Entity entity) 
{
    return get_component_array<T>()->get_component(entity);
}

template <class T>
ComponentArray<T>* ComponentManager::get_component_array()
{
    const char* name = typeid(T).name();
    if (m_component_arrays.find(name) == m_component_arrays.end()) { std::cout << "component not registered\n"; abort(); }
    return (ComponentArray<T>*)m_component_arrays[name];
}

/*
    System Manager
*/

template <class T>
T* SystemManager::register_system()
{
    const char* name = typeid(T).name();
    if (m_systems.find(name) != m_systems.end()) { std::cout << "system already registered\n"; abort(); }

    System* system = new T();
    m_systems[name] = system;
    return (T*)system;
}

template <class T>
void SystemManager::set_signature(Signature signature)
{
    const char* name = typeid(T).name();
    if (m_systems.find(name) == m_systems.end()) { std::cout << "system isn't registered\n"; abort(); }

    m_system_signatures[name] = signature;
}

/*
    Coordinator
*/

template <class T>
void Coordinator::register_component()
{
    m_components->register_component<T>();
}

template <class T>
void Coordinator::add_component(Entity entity, T component)
{
    m_components->add_component(entity, component);
    
    // update entity signature to add the new component
    Signature signature = m_entities->get_signature(entity);
    signature.set(m_components->get_component_type<T>(), 1);
    m_entities->set_signature(entity, signature);
    
    m_systems->entity_signature_changed(entity, signature);
}

template <class T>
void Coordinator::remove_component(Entity entity, T component)
{
    m_components->remove_component(entity, component);

    // update entity signature to remove the new component
    Signature signature = m_entities->get_signature(entity);
    signature.set(m_components->get_component_type<T>(), 0);
    m_entities->set_signature(entity, signature);
    
    m_systems->entity_signature_changed(entity, signature);
}

template <class T>
T* Coordinator::get_component(Entity entity) 
{
    return m_components->get_component<T>(entity);
}

template <class T>
ComponentType Coordinator::get_component_type()
{
    return m_components->get_component_type<T>();
}

template <class T>
T* Coordinator::register_system()
{
    return m_systems->register_system<T>();
}

template <class T>
void Coordinator::set_system_signature(Signature signature)
{
    m_systems->set_signature<T>(signature);
}