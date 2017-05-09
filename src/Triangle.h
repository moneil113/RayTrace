#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Geometry.h"

class Triangle : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f v1;
    Eigen::Vector3f v2;
    Eigen::Vector3f v3;

    Eigen::Vector3f normal;
    bool normSet = false;

public:
    Triangle ();

    std::string to_string();
    std::string type();

    floatOptional intersect(Ray &r);
    Eigen::Vector3f normalAtPoint(Eigen::Vector3f p);
};
#endif
