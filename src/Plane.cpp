#include <sstream>
#include "Plane.h"
#include "Ray.h"

using namespace Eigen;

Plane::Plane() {

}

std::string Plane::to_string() {
    std::stringstream str;
    str << "- Type: Plane:\n";
    str << "- Normal: " << formatVector(normal) << "\n";
    str << "- Distance: " << distance << "\n";
    str << Geometry::to_string();
    return str.str();
}

std::string Plane::type() {
    return "Plane";
}

floatOptional Plane::intersect(Ray &r) {
    if (r.direction().dot(normal) == 0) {
        return {false, 0.0f};
    }
    Vector3f p1 = normal * distance;
    Vector3f temp = r.origin() - p1;

    float t = temp.dot(normal) / r.direction().dot(normal);
    if (t < 0) {
        return {false, t};
    }
    return {true, t};
}
