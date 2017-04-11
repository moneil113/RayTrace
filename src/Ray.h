#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

class Ray {
private:
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;

public:
    Ray(Eigen::Vector3f origin, Eigen::Vector3f direction);

    Eigen::Vector3f point(float t);

    std::string to_string();
};

#endif
