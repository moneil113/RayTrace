#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

class Ray {
private:
    Eigen::Vector3f _origin;
    Eigen::Vector3f _direction;

public:
    Ray(Eigen::Vector3f origin, Eigen::Vector3f direction);

    const Eigen::Vector3f &origin() const { return _origin; }
    const Eigen::Vector3f &direction() const { return _direction; }
    Eigen::Vector3f getPoint(float t) const;

    bool refracted = false;

    std::string to_string() const;
};

#endif
