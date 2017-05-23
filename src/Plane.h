#ifndef PLANE_H
#define PLANE_H

#include "Geometry.h"

class Plane : public Geometry {
friend class Parser;

private:
    Eigen::Vector3f normal;
    float distance;

public:
    Plane ();

    std::string to_string();
    std::string type();

    floatOptional objectIntersect(const Ray &r);
    Eigen::Vector3f normalAtPoint(Eigen::Vector3f p);
};
#endif
