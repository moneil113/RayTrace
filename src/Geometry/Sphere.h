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
    std::string type();

    floatOptional objectIntersect(const Ray &r);
    Eigen::Vector3f objectNormal(const Eigen::Vector3f &p);

    void boundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max);
};
#endif
