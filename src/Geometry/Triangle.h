#pragma once
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

    void computeDeterminants();
    bool determinantsSet = false;
    Eigen::Matrix3f mat;

    // determinants of submatrices excluding row 2, column 0, 1, 2
    // compute these once, then just fetch them on subsequent calls
    // (we can do this because det(A) == det(transpose(A)) )
    // [[a, b, c],      [[a, d, g],
    //  [d, e, f], =>    [b, e, h],     c(dh-eg)-f(ah-bg)+i(ae-bd)
    //  [g, h, i]]       [c, f, i]]
    // [a-b, a-c, d]
    float det1, det2, det3;

    floatOptional objectIntersect(const Ray &r);
    Eigen::Vector3f objectNormal(const Eigen::Vector3f &p);
    void objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max);
    Eigen::Vector3f objectCenter();

public:
    Triangle ();

    std::string to_string();
    std::string type();
};
#endif
