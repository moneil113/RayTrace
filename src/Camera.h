#ifndef CAMERA
#define CAMERA

#include "Object.h"

class Camera : public Object {
friend class Parser;
private:
    Eigen::Vector3f location;
    Eigen::Vector3f up;
    Eigen::Vector3f right;
    Eigen::Vector3f look_at;

public:
    Camera();

    std::string to_string();
};

#endif
