#include <sstream>
#include <iostream>
#include "Triangle.h"
#include "Ray.h"

using namespace Eigen;

Triangle::Triangle() {

}

std::string Triangle::to_string() {
    std::stringstream str;
    str << "triangle:\n";
    str << "      corner 1  = " << formatVector(v1) << "\n";
    str << "      corner 2  = " << formatVector(v2) << "\n";
    str << "      corner 3  = " << formatVector(v3) << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Triangle::type() {
    return "Triangle";
}

void Triangle::computeDeterminants() {
    float a = v1.x() - v2.x();
    float d = v1.y() - v2.y();
    float g = v1.z() - v2.z();
    float b = v1.x() - v3.x();
    float e = v1.y() - v3.y();
    float h = v1.z() - v3.z();

    det1 = (d * h) - (e * g);
    det2 = (a * h) - (b * g);
    det3 = (a * e) - (b * d);


    determinantsSet = true;

    mat.col(0) = v1 - v2;
    mat.col(1) = v1 - v3;
}

floatOptional Triangle::objectIntersect(const Ray &r) {
    float beta, gamma, t;
    // RHS of Ax = B matrix equation
    Vector3f rhs = v1 - r.origin();

    Matrix3f mat3 = Matrix3f();
    mat3.col(0) = v1 - v2;
    mat3.col(1) = v1 - v3;

    if (!determinantsSet) {
        computeDeterminants();
    }

    Vector3f d = r.direction();
    float det = d.x() * det1 - d.y() * det2 + d.z() * det3;

    mat.col(2) = r.direction();
    Matrix3f mat2 = mat;
    mat2.col(0) = rhs;
    beta = mat2.determinant() / det;

    if (beta < 0 || beta > 1) {
        return {false, 0.0f};
    }

    mat2 = mat;
    mat2.col(1) = rhs;
    gamma = mat2.determinant() / det;

    if (gamma < 0 || gamma > 1 - beta) {
        return {false, 0.0f};
    }

    mat2 = mat;
    mat2.col(2) = rhs;
    t = mat2.determinant() / det;

    if (t < 0) {
        return {false, 0.0f};
    }

    return {true, t};
}

Eigen::Vector3f Triangle::objectNormal(const Eigen::Vector3f &p) {
    if (!normSet) {
        normal = (v2 - v1).cross(v3 - v2);
        normal.normalize();
        normSet = true;
    }
    return normal;
}
