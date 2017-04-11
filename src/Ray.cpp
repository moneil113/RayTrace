#include <sstream>
#include "Ray.h"

Ray::Ray(Eigen::Vector3f origin, Eigen::Vector3f direction) {
    this->origin = origin;
    this->direction = direction.normalized();
}

Eigen::Vector3f Ray::point(float t) {
    return origin + t * direction;
}

std::string formatVector(Eigen::Vector3f v) {
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
        Eigen::DontAlignCols,
        ", ", ", ", "", "", "", "");

    std::stringstream str;
    str << v.format(CommaInitFmt);

    return str.str();
}


std::string Ray::to_string() {
    std::stringstream str;
    str << "{" << formatVector(origin) << "} -> {" << formatVector(direction) << "}";
    return str.str();
}
