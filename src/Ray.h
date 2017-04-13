#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

class Ray {
private:
    Eigen::Vector3f _origin;
    Eigen::Vector3f _direction;

public:
    Ray(Eigen::Vector3f origin, Eigen::Vector3f direction);

    Eigen::Vector3f &origin() { return _origin; }
    Eigen::Vector3f &direction() { return _direction; }
    Eigen::Vector3f getPoint(float t);

    std::string to_string();
};

#endif
