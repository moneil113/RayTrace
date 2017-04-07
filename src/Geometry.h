#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <Eigen/Dense>
#include <Eigen/StdVector>

#include "Object.h"

#define TRANSFORM_TRANSLATE 0
#define TRANSFORM_ROTATE 1
#define TRANSFORM_SCALE 2

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

    virtual void trace() = 0;
    void addTransform(int type, Eigen::Vector3f value);
};

#endif
