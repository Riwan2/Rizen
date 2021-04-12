#ifndef G_COMPONENT_H
#define G_COMPONENT_H

#include "../rizen/rizen.h"

class MoveComponent : public Component {
public:
    MoveComponent();
    ~MoveComponent();

    void update() override;

private:

};

#endif //G_COMPONENT_H