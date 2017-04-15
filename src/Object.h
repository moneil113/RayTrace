#ifndef OBJECT_H
#define OBJECT_H

#include <Eigen/Dense>
#include <iomanip>

class Object {
friend class Parser;

public:
    Object();
    virtual std::string to_string() = 0;
    virtual std::string formatVector(Eigen::Vector3f v);
};

#endif
