#ifndef CONE_H
#define CONE_H

#include "Geometry.h"

class Cone : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f base;
    Eigen::Vector3f cap;
    float baseRadius;
    float capRadius;

public:
    Cone ();

    std::string to_string();
    std::string type();

    floatOptional intersect(const Ray &r) { return {false, 0.0f}; }
    Eigen::Vector3f normalAtPoint(Eigen::Vector3f p) { return Eigen::Vector3f(0, 0, 0); };
};
#endif
