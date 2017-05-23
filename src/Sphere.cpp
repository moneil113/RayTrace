#include <sstream>
#include "Sphere.h"
#include "Ray.h"

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
    return "Sphere";
}

floatOptional Sphere::objectIntersect(const Ray &r) {
    float A = r.direction().dot(r.direction());
    Eigen::Vector3f temp = (r.origin() - center);
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

Eigen::Vector3f Sphere::normalAtPoint(Eigen::Vector3f p) {
    Eigen::Vector4f n;
    n << (p - center).normalized(), 0;
    n = modelMatrix.transpose() * n;
    return n.head(3);
}
