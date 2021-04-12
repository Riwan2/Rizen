#include "entity.h"

/*
    Transform
*/

glm::mat4 UNIT = glm::mat4(1.0);

void Transform::update()
{
    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    model *= glm::toMat4(quat);
}

/*
    Component manager
*/

ComponentManager::ComponentManager() {}
ComponentManager::~ComponentManager() 
{
    for (auto component : m_components)
        delete component;
}

void ComponentManager::init(Entity* entity) 
{
    m_entity = entity;
}

void ComponentManager::update()
{
    for (auto component : m_components)
        component->update();
}

/*
    Entity
*/

Entity::Entity() {}
Entity::~Entity() 
{
    delete m_components;
}

void Entity::init(Model* model)
{
    m_model = model;
    m_components = new ComponentManager();
    m_components->init(this);
}

void Entity::update()
{
    m_components->update();
}

void Entity::set_rotation(const glm::vec3& rotation)
{
    m_transform.rotation.x = glm::radians(fmod(rotation.x, 360));
    m_transform.rotation.y = glm::radians(fmod(rotation.y, 360));
    m_transform.rotation.z = glm::radians(fmod(rotation.z, 360));
	m_transform.quat = glm::quat(m_transform.rotation);
}


void Entity::move_rotation(const glm::vec3& rotation)
{
    m_transform.rotation.x += glm::radians(fmod(rotation.x, 360));
    m_transform.rotation.y += glm::radians(fmod(rotation.y, 360));
    m_transform.rotation.z += glm::radians(fmod(rotation.z, 360));
	m_transform.quat = glm::quat(m_transform.rotation);
}