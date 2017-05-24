#include <sstream>
#include <iostream>
#include "Plane.h"
#include "Ray.h"

using namespace Eigen;
using namespace std;

Plane::Plane() {

}

std::string Plane::to_string() {
    stringstream str;
    str << setprecision(4);
    str << "- Type: Plane\n";
    str << "- Normal: " << formatVector(normal) << "\n";
    str << "- Distance: " << distance << "\n";
    str << Geometry::to_string();
    return str.str();
}

std::string Plane::type() {
    return "Plane";
}

floatOptional Plane::objectIntersect(const Ray &r) {
    if (r.direction().dot(normal) == 0) {
        return {false, 0.0f};
    }
    Vector3f p1 = normal * distance;
    Vector3f temp = p1 - r.origin();

    float t = temp.dot(normal) / r.direction().dot(normal);
    if (t < 0) {
        return {false, t};
    }
    return {true, t};
}

Eigen::Vector3f Plane::objectNormal(const Eigen::Vector3f &p) {
    return normal;
}
