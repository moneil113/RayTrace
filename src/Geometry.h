#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <Eigen/Dense>
#include <Eigen/StdVector>

#include "Object.h"

#define TRANSFORM_TRANSLATE 0
#define TRANSFORM_ROTATE 1
#define TRANSFORM_SCALE 2

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

    std::vector<Eigen::Vector3f> transformValues;
    std::vector<int> transformTypes;

public:
    Geometry() {}

    virtual std::string to_string();
    virtual std::string type() = 0;

    void addTransform(int type, Eigen::Vector3f value);
    Eigen::Vector3f color() { return pigment; }
    const Finish_t &getFinish() { return finish; }

    // Tests for intersection. If there is no intersection, the floatOptional
    // will not be valid. If there is an intersection, the value of the
    // floatOptional will be the closest nonnegative t value of the input ray.
    virtual floatOptional intersect(Ray &r) = 0;

    // Given a point on the surface, return the normal
    // Assumes that the given point is on the surface of the object
    virtual Eigen::Vector3f normalAtPoint(Eigen::Vector3f p) = 0;
};

#endif
