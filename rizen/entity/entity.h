#ifndef ENTITY_H
#define ENTITY_H

#include "../utils/rizen_math.h"
#include "../opengl/model.h"

/*
    Transform
*/ 

struct Transform {
    void update();

    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotation;

    glm::quat quat;
    glm::mat4 model;
};

/*
    Entity
*/

class Entity {
public:
    Entity();
    ~Entity();

    void init();

    const Transform& transform() const { return m_transform; }

private:
    Transform m_transform;
    Model* m_model;
};

#endif //ENTITY_H