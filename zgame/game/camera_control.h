#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include "../../rizen/opengl/camera.h"
#include "../../rizen/app/input.h"
#include "../../rizen/utils/rizen_math.h"
#include "../../rizen/utils/rizen_time.h"

class CameraControl {
public:
    void init();
    void update();

    Camera* camera() { return &m_camera_tps; }

private:
    CameraTPS m_camera_tps;

    void check_resize();
};

#endif //CAMERA_CONTROL_H