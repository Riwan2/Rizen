#ifndef RENDERABLE
#define RENDERABLE

#include "../../rizen/opengl/model.h"

struct Renderable {
    Renderable() {}
    Renderable(Model* p_model) : model(p_model) {}
    Model* model;
};

#endif //RENDERABLE