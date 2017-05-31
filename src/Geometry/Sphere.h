#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Geometry.h"

class Sphere : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f center;
    float radius;

    floatOptional objectIntersect(const Ray &r);
    Eigen::Vector3f objectNormal(const Eigen::Vector3f &p);
    void objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max);
    Eigen::Vector3f objectCenter();

public:
    Sphere ();

    std::string to_string();
    std::string type();

};
#endif
