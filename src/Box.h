#ifndef BOX_H
#define BOX_H

#include "Geometry.h"
#include <Eigen/Dense>

class Box : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f corner1;
    Eigen::Vector3f corner2;

public:
    Box ();

    std::string to_string();
    std::string type();

    floatOptional intersect(const Ray &r) { return {false, 0.0f}; }
    Eigen::Vector3f normalAtPoint(Eigen::Vector3f p) { return Eigen::Vector3f(0, 0, 0); };
};
#endif
