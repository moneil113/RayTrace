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

    Eigen::Vector3f corner;

    int rowSamples = 1, columnSamples = 1;

public:
    Light ();

    const Eigen::Vector3f &getLocation() const { return location; }
    const Eigen::Vector3f &getColor() const { return color; }

    int getRowSamples() const { return rowSamples; }
    int getColumnSamples() const { return columnSamples; }

    Eigen::Vector3f samplePosition(const int i, const int j) const;
    float sampleWeight() const;

    std::string to_string();
};

#endif
