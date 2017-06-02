#include <sstream>
#include "Box.h"

using namespace Eigen;

Box::Box() {

}

std::string Box::to_string() {
    std::stringstream str;
    str << "box:\n";
    str << "      min  = " << formatVector(min) << "\n";
    str << "      max  = " << formatVector(max) << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Box::type() {
    return "Box";
}

void swap(float &a, float &b) {
    float temp = a;
    a = b;
    b = temp;
}

floatOptional Box::objectIntersect(const Ray &r) {
    float tgmin = -INFINITY, tgmax = INFINITY;
    Vector3f d = r.direction();
    float dx = d.x();
    float dy = d.y();
    float dz = d.z();
    if (dx == 0 || dy == 0 || dz == 0) {
        return {false, 0.0f};
    }

    Vector3f origin = r.origin();

    float t1 = (min.x() - origin.x()) / dx;
    float t2 = (max.x() - origin.x()) / dx;

    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    t1 = (min.y() - origin.y()) / dy;
    t2 = (max.y() - origin.y()) / dy;
    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    t1 = (min.z() - origin.z()) / dz;
    t2 = (max.z() - origin.z()) / dz;
    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    if (tgmin > tgmax || tgmax < 0) {
        return {false, 0.0f};
    }

    if (tgmin > 0) {
        return {true, tgmin};
    }
    else {
        return {true, tgmax};
    }
}

bool floatEqual(float a, float b) {
    const float epsilon = 0.0001f;
    return std::abs(a - b) <= epsilon;
}

Eigen::Vector3f Box::objectNormal(const Eigen::Vector3f &p) {
    if (floatEqual(p.x(), min.x())) {
        return Vector3f(-1, 0, 0);
    }
    else if (floatEqual(p.x(), max.x())) {
        return Vector3f(1, 0, 0);
    }
    else if (floatEqual(p.y(), min.y())) {
        return Vector3f(0, -1, 0);
    }
    else if (floatEqual(p.y(), max.y())) {
        return Vector3f(0, 1, 0);
    }
    else if (floatEqual(p.z(), min.z())) {
        return Vector3f(0, 0, -1);
    }
    else {
        return Vector3f(0, 0, 1);
    }
}

void Box::objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) {
    min = this->min;
    max = this->max;
}

Eigen::Vector3f Box::objectCenter() {
    return (min + max) / 2;
}
