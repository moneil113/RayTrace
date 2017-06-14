#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "../Util/Object.h"
#include <Eigen/Dense>

class Light : public Object{
friend class Parser;

private:
    Eigen::Vector3f location;
    Eigen::Vector3f color;

    bool areaLight = false;
    Eigen::Vector3f axis1;
    Eigen::Vector3f axis2;

    int rowSamples, columnSamples;

public:
    Light ();

    const Eigen::Vector3f &getLocation() const { return location; }
    const Eigen::Vector3f &getColor() const { return color; }

    std::string to_string();
};

#endif
