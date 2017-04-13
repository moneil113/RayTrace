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

class Ray;

class Geometry : public Object {
friend class Parser;

protected:
    Eigen::Vector3f pigment;
    float ambient;
    float diffuse;

    std::vector<Eigen::Vector3f> transformValues;
    std::vector<int> transformTypes;

public:
    Geometry() {}

    virtual std::string to_string();
    virtual std::string type() = 0;

    void addTransform(int type, Eigen::Vector3f value);
    Eigen::Vector3f color() const { return pigment; }

    // Tests for intersection. If there is no intersection, the floatOptional
    // will not be valid. If there is an intersection, the value of the
    // floatOptional will be the closest nonnegative t value of the input ray.
    virtual floatOptional intersect(Ray &r) = 0;
};

#endif
