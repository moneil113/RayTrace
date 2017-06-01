#pragma once
#ifndef BOX_H
#define BOX_H

#include "Geometry.h"
#include <Eigen/Dense>

class Box : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f min;
    Eigen::Vector3f max;

    floatOptional objectIntersect(const Ray &r);
    Eigen::Vector3f objectNormal(const Eigen::Vector3f &p);
    void objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max);
    Eigen::Vector3f objectCenter();

public:
    Box ();

    std::string to_string();
    std::string type();
};
#endif
