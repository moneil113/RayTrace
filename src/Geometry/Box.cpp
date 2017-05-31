#include <sstream>
#include "Box.h"

Box::Box() {

}

std::string Box::to_string() {
    std::stringstream str;
    str << "box:\n";
    str << "      corner1  = " << formatVector(corner1) << "\n";
    str << "      corner2  = " << formatVector(corner2) << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Box::type() {
    return "Box";
}

void Box::objectBoundingBox(Eigen::Vector3f &min, Eigen::Vector3f &max) {
    min = corner1;
    max = corner2;
}

Eigen::Vector3f Box::objectCenter() {
    return (corner1 + corner2) / 2;
}
