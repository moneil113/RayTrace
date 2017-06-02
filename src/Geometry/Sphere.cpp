#include <sstream>
#include "Sphere.h"

using namespace Eigen;

Sphere::Sphere() {

}

std::string Sphere::to_string() {
    std::stringstream str;
    str << "- Type: Sphere\n";
    str << "- Center: " << formatVector(center) << "\n";
    str << "- Radius: " << radius << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Sphere::type() {
    if (finish.filter < 1) {
        return "refract sphere";
    }
    else {
        return "solid sphere";
    }
    return "Sphere";
}

floatOptional Sphere::objectIntersect(const Ray &r) {
    float A = r.direction().dot(r.direction());
    Vector3f temp = (r.origin() - center);
    float B = 2 * temp.dot(r.direction());
    float C = temp.dot(temp) - radius * radius;

    float determinant = B * B - 4 * A * C;

    if (determinant < 0) {
        return {false, 0.0f};
    }

    floatOptional ret = {true, 0.0f};
    ret.value = (-B - sqrtf(determinant)) / (2 * A);

    // check if smallest intersection is behind ray origin
    if (ret.value < 0) {
        ret.value = (-B + sqrtf(determinant)) / (2 * A);
    }
    // if both intersections are behind ray origin, no intersect
    if (ret.value < 0) {
        ret.valid = false;
    }

    return ret;
}

Eigen::Vector3f Sphere::objectNormal(const Eigen::Vector3f &p) {
    return (p - center).normalized();
}

void Sphere::objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) {
    min = center - Vector3f(radius, radius, radius);
    max = center + Vector3f(radius, radius, radius);
}

Eigen::Vector3f Sphere::objectCenter() {
    return center;
}
