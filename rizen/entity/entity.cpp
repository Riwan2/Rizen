#include "entity.h"

/*
    Transform
*/

void Transform::update()
{
    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    model *= glm::toMat4(quat);
}

/*
    Entity
*/

Entity::Entity() {}
Entity::~Entity() {}

void Entity::init()
{
    
}