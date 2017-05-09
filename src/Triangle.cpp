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
    str << "      corner1  = " << formatVector(v1) << "\n";
    str << "      corner1  = " << formatVector(v2) << "\n";
    str << "      corner1  = " << formatVector(v3) << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Triangle::type() {
    return "Triangle";
}

floatOptional Triangle::intersect(Ray &r) {
    float beta, gamma, t;
    // RHS of Ax = B matrix equation
    Vector3f rhs = v1 - r.origin();

    Matrix3f mat = Matrix3f();
    mat.col(0) = v1 - v2;
    mat.col(1) = v1 - v3;
    mat.col(2) = r.direction();

    float det = mat.determinant();
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



    return {true, t};
}

Eigen::Vector3f Triangle::normalAtPoint(Eigen::Vector3f p) {
    if (!normSet) {
        normal = (v2 - v1).cross(v3 - v2);
        normSet = true;
    }
    return normal;
}
