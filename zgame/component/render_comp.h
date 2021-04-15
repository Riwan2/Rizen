#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "../../rizen/opengl/model.h"

struct RenderComponent {
    RenderComponent() {}
    RenderComponent(Model* p_model) : model(p_model) {}
    Model* model;
};

#endif //RENDER_COMPONENT_H