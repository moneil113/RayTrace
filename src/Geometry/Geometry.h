#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <Eigen/Dense>

#include "../Util/Object.h"
#include "../Scene/Ray.h"

// "Optional" float. The value should only be considered when valid == true
typedef struct {
    bool valid;
    float value;
} floatOptional;

typedef struct {
    float ambient;
    float diffuse;
    float specular;
    float roughness;
    float metallic;
    float ior;
    float reflection;
    float filter;
} Finish_t;

class Ray;

class Geometry : public Object {
friend class Parser;

protected:
    Eigen::Vector3f pigment;
    Finish_t finish;

    Eigen::Matrix4f modelMatrix = Eigen::Matrix4f::Identity();

    virtual floatOptional objectIntersect(const Ray &r) = 0;
    virtual Eigen::Vector3f objectNormal(const Eigen::Vector3f &p) = 0;

public:

    virtual std::string to_string();
    virtual std::string type() = 0;

    Eigen::Vector3f color() { return pigment; }
    const Finish_t &getFinish() { return finish; }

    void scale(const Eigen::Vector3f);
    void rotate(const Eigen::Vector3f);
    void translate(const Eigen::Vector3f);
    void finalizeTransform();

    // Tests for intersection. If there is no intersection, the floatOptional
    // will not be valid. If there is an intersection, the value of the
    // floatOptional will be the closest nonnegative t value of the input ray.
    virtual floatOptional intersect(const Ray &r);

    // Given a point on the surface, return the normal
    // Assumes that the given point is on the surface of the object
    virtual Eigen::Vector3f normalAtPoint(const Eigen::Vector3f &p);

    Ray getTransformedRay(const Ray &r);

    virtual void boundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) = 0;
};

#endif
