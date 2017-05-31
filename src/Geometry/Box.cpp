#include <sstream>
#include "Box.h"

using namespace Eigen;

Box::Box() {

}

std::string Box::to_string() {
    std::stringstream str;
    str << "box:\n";
    str << "      corner1  = " << formatVector(corner1) << "\n";
    str << "      corner2  = " << formatVector(corner2) << "\n";
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

    float t1 = (corner1.x() - origin.x()) / dx;
    float t2 = (corner2.x() - origin.x()) / dx;

    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    t1 = (corner1.y() - origin.y()) / dy;
    t2 = (corner2.y() - origin.y()) / dy;
    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    t1 = (corner1.z() - origin.z()) / dz;
    t2 = (corner2.z() - origin.z()) / dz;
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

    return {true, tgmin};
}

bool floatEqual(float a, float b) {
    const float epsilon = 0.0001f;
    return std::abs(a - b) <= epsilon;
}

Eigen::Vector3f Box::objectNormal(const Eigen::Vector3f &p) {
    if (floatEqual(p.x(), corner1.x())) {
        if (corner1.x() < corner2.x()) {
            return Vector3f(-1, 0, 0);
        }
        else {
            return Vector3f(1, 0, 0);
        }
    }
    else if (floatEqual(p.x(), corner2.x())) {
        if (corner1.x() < corner2.x()) {
            return Vector3f(1, 0, 0);
        }
        else {
            return Vector3f(-1, 0, 0);
        }
    }
    else if (floatEqual(p.y(), corner1.y())) {
        if (corner1.y() < corner2.y()) {
            return Vector3f(0, -1, 0);
        }
        else {
            return Vector3f(0, 1, 0);
        }
    }
    else if (floatEqual(p.y(), corner2.y())) {
        if (corner1.y() < corner2.y()) {
            return Vector3f(0, 1, 0);
        }
        else {
            return Vector3f(0, -1, 0);
        }
    }
    else if (floatEqual(p.z(), corner1.z())) {
        if (corner1.z() < corner2.z()) {
            return Vector3f(0, 0, -1);
        }
        else {
            return Vector3f(0, 0, 1);
        }
    }
    else {
        if (corner1.z() < corner2.z()) {
            return Vector3f(0, 0, 1);
        }
        else {
            return Vector3f(0, 0, -1);
        }
    }
}

void Box::objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) {
    min = corner1;
    max = corner2;
}

Eigen::Vector3f Box::objectCenter() {
    return (corner1 + corner2) / 2;
}
