#pragma once
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

    floatOptional objectIntersect(const Ray &r) { return {false, 0.0f}; }
    Eigen::Vector3f objectNormal(const Eigen::Vector3f &p) { return Eigen::Vector3f(0, 0, 0); };
    void objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) { return; }
    Eigen::Vector3f objectCenter();

public:
    Cone ();

    std::string to_string();
    std::string type();
};
#endif
