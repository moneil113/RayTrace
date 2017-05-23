#include <sstream>
#include <Eigen/Geometry>
#include "Geometry.h"
#include "Ray.h"

#include <iostream>

using namespace Eigen;

floatOptional Geometry::intersect(const Ray &r) {
    Vector4f newOrigin;
    newOrigin << r.origin(), 1;
    Vector4f newDir;
    newDir << r.direction(), 0;

    newOrigin = modelMatrix * newOrigin;
    newDir = modelMatrix * newDir;
    const Ray transformedR = Ray(newOrigin.head(3),
                                 newDir.head(3));
    // std::cout << r.to_string() << " vs " << transformedR.to_string() << std::endl;
    return objectIntersect(transformedR);
}

std::string Geometry::to_string() {
    std::stringstream str;
    str << "- Color: " << formatVector(pigment) << "\n";
    str << "- Material:\n";
    str << "  - Ambient: " << finish.ambient << "\n";
    str << "  - Diffuse: " << finish.diffuse << "\n";
    str << "  - Specular: " << finish.specular << "\n";
    str << "  - Roughness: " << finish.roughness << "\n";
    str << "  - Metallic: " << finish.metallic << "\n";
    str << "  - IOR: " << finish.ior << "\n";
    str << "  - Reflection: " << finish.reflection << "\n";
    str << "  - Refraction: " << finish.filter << "\n";

    return str.str();
}

void Geometry::scale(const Eigen::Vector3f s) {
    Transform<float, 3, Projective> sc;
    sc = Scaling(s);
    modelMatrix = sc.matrix() * modelMatrix;
}

void Geometry::rotate(const Eigen::Vector3f r) {
    Transform<float, 3, Projective> rot;
    rot = AngleAxisf(r.z() * M_PI / 180, Vector3f::UnitZ())
        * AngleAxisf(r.y() * M_PI / 180, Vector3f::UnitY())
        * AngleAxisf(r.x() * M_PI / 180, Vector3f::UnitX());
    modelMatrix = rot.matrix() * modelMatrix;
}

void Geometry::translate(const Eigen::Vector3f t) {
    // Transform<float, 3, Projective> tr;
    // tr = Translation3f(t);
    Matrix4f tr = Matrix4f::Identity();
    tr.col(3).head(3) = t;
    modelMatrix = tr * modelMatrix;
}

void Geometry::finalizeTransform() {
    modelMatrix = modelMatrix.inverse().eval();
}
