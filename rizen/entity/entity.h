#ifndef ENTITY_H
#define ENTITY_H

#include "../utils/rizen_math.h"
#include "../opengl/model.h"

/*
    Transform
*/ 

struct Transform {
    void update();

    glm::vec3 pos = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0);
    glm::vec3 rotation = glm::vec3(0.0);

    glm::quat quat = glm::quat(1.0, 0.0, 0.0, 0.0);
    glm::mat4 model = glm::mat4(1.0);
};

/*
    Component
*/

class Entity;

class Component {
public:
    Component() {}
    virtual ~Component() {}

    void init(Entity* entity) { m_entity = entity; }
    virtual void update() = 0;

protected:
    Entity* entity() const { return m_entity; };
private:
    Entity* m_entity;
};

/*
    Component Manager
*/

class ComponentManager {
public:
    ComponentManager();
    ~ComponentManager();

    void init(Entity* entity);
    void update();

    template<class T>
    bool add(T* component)
    {
        std::string name = typeid(T).name();

        for (auto component : m_components) {
            if (dynamic_cast<T*>(component))
                return rizen_error("component add already exist error: " + name);
        }

        component->init(m_entity);
        m_components.push_back(component);
        return true;
    }

    template<class T>
    Component* get()
    {
        std::string name = typeid(T).name();

        for (auto component : m_components) {
            if (dynamic_cast<T*>(component))
                return component;
        }

        rizen_error("component get doesn't exist error: " + name);
        return nullptr;
    }

    template<class T>
    bool remove()
    {
        std::string name = typeid(T).name();
        Component* target = get<T>();
        if (target == nullptr) 
            return rizen_error("no target for deletion error: " + name);
        
        int index = -1;
        for (int i = 0; i < m_components.size(); i++) {
            if (m_components[i] == target) {
                index = i;
                break;
            }
        }
        
        if(index == -1)
            return rizen_error("component delete doesn't exist error: " + name);

        m_components.erase(m_components.begin() + index);
        delete target;
        return true;
    }

private:
    std::vector<Component*> m_components;
    Entity* m_entity;
};

/*
    Entity
*/

class Entity {
public:
    Entity();
    ~Entity();

    void init(Model* model);
    void update();
    void update_transform() { m_transform.update(); }

    const Transform& transform() const { return m_transform; }
    Model* model() const { return m_model; }

    ComponentManager* components() const { return m_components; };

    // rotation is in degree
    void set_rotation(const glm::vec3& rotation);
    void set_position(const glm::vec3& position) { m_transform.pos = position; }
    void set_scale(const glm::vec3& scale) { m_transform.scale = scale; }

    void move_rotation(const glm::vec3& rotation);
    void move_position(const glm::vec3& position) { m_transform.pos += position; }
    void move_scale(const glm::vec3& scale) { m_transform.scale *= scale; }

    const glm::vec3& position() const { return m_transform.pos; }
    const glm::vec3& scale() const { return m_transform.scale; }
    const glm::vec3& rotation() const { return m_transform.rotation; }

private:
    Transform m_transform;
    Model* m_model;
    ComponentManager* m_components;
};

#endif //ENTITY_H