#ifndef CONE_H
#define CONE_H

#include "Geometry.h"

class Cone : public Geometry {
friend class Parser;
private:
    Eigen::Vector3f base;
    Eigen::Vector3f cap;
    float baseRadius;
    float capRadius;

public:
    Cone ();

    std::string to_string();

    void trace() {}
};
#endif
