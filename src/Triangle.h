#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Geometry.h"

class Triangle : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f corner1;
    Eigen::Vector3f corner2;
    Eigen::Vector3f corner3;

public:
    Triangle ();

    std::string to_string();
    std::string type();

    floatOptional intersect(Ray &r) { return {false, 0.0f}; }
    Eigen::Vector3f normalAtPoint(Eigen::Vector3f p) { return Eigen::Vector3f(0, 0, 0); };
};
#endif
