#ifndef CAMERA
#define CAMERA

#include "Object.h"

class Ray;

class Camera : public Object {
friend class Parser;
private:
    Eigen::Vector3f location;
    Eigen::Vector3f up;
    Eigen::Vector3f right;
    Eigen::Vector3f look_at;

    int width;
    int height;

public:
    Camera();

    Ray rayToPixel(int x, int y);

    void setImageSize(int width, int height);
    std::string to_string();
};

#endif
