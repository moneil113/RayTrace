#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"
#include <Eigen/Dense>

class Light : public Object{
friend class Parser;

private:
    Eigen::Vector3f location;
    Eigen::Vector3f color;

public:
    Light ();

    Eigen::Vector3f getLocation() { return location; }
    Eigen::Vector3f getColor() { return color; }

    std::string to_string();
};

#endif
