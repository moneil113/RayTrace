#include <sstream>
#include <Eigen/Geometry>
#include "Geometry.h"

#include <iostream>

using namespace Eigen;

floatOptional Geometry::intersect(const Ray &r) {
    if (transformed) {
        Vector4f newOrigin;
        newOrigin << r.origin(), 1;
        Vector4f newDir;
        newDir << r.direction(), 0;

        newOrigin = inverseModelMatrix * newOrigin;
        newDir = inverseModelMatrix * newDir;
        const Ray transformedR = Ray(newOrigin.head(3), newDir.head(3));
        return objectIntersect(transformedR);
    }
    else {
        return objectIntersect(r);
    }
}

Eigen::Vector3f Geometry::normalAtPoint(const Eigen::Vector3f &p) {
    if (transformed) {
        Vector4f localP, n;
        localP << p, 1;
        localP = inverseModelMatrix * localP;
        n << objectNormal(localP.head(3)), 0;
        n = inverseModelMatrix.transpose() * n;
        return n.head(3).normalized();
    }
    else {
        return objectNormal(p);
    }
}

void Geometry::boundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) {
    objectBoundingBox(min, max);
    if (transformed) {
        Vector4f min4, max4;
        min4 << min, 1;
        max4 << max, 1;
        min4 = modelMatrix * min4;
        max4 = modelMatrix * max4;

        // We have bounding coordinates of object in world space,
        // find bounding box of box these describe
        min << (min4.x() < max4.x() ? min4.x() : max4.x()),
               (min4.y() < max4.y() ? min4.y() : max4.y()),
               (min4.z() < max4.z() ? min4.z() : max4.z());

        max << (min4.x() > max4.x() ? min4.x() : max4.x()),
               (min4.y() > max4.y() ? min4.y() : max4.y()),
               (min4.z() > max4.z() ? min4.z() : max4.z());
    }
}

Eigen::Vector3f Geometry::getCenter() {
    Vector4f localCenter;
    localCenter << objectCenter(), 1;
    if (transformed) {
        localCenter = modelMatrix * localCenter;
    }
    return localCenter.head(3);
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
    transformed = true;
}

void Geometry::rotate(const Eigen::Vector3f r) {
    Transform<float, 3, Projective> rot;
    rot = AngleAxisf(r.z() * M_PI / 180, Vector3f::UnitZ())
        * AngleAxisf(r.y() * M_PI / 180, Vector3f::UnitY())
        * AngleAxisf(r.x() * M_PI / 180, Vector3f::UnitX());
    modelMatrix = rot.matrix() * modelMatrix;
    transformed = true;
}

void Geometry::translate(const Eigen::Vector3f t) {
    Matrix4f tr = Matrix4f::Identity();
    tr.col(3).head(3) = t;
    modelMatrix = tr * modelMatrix;
    transformed = true;
}

void Geometry::finalizeTransform() {
    if (transformed) {
        inverseModelMatrix = modelMatrix.inverse();
    }
}

Ray Geometry::getTransformedRay(const Ray &r) {
    if (transformed) {
        Vector4f newOrigin;
        newOrigin << r.origin(), 1;
        Vector4f newDir;
        newDir << r.direction(), 0;

        newOrigin = modelMatrix * newOrigin;
        newDir = modelMatrix * newDir;
        return Ray(newOrigin.head(3), newDir.head(3));
    }
    else {
        return r;
    }
}
