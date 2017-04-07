#ifndef SPHERE_H
#define SPHERE_H

#include "Geometry.h"

class Sphere : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f center;
    float radius;

public:
    Sphere ();

    std::string to_string();

    void trace() {}
};
#endif
