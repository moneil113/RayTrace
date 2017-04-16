#include <sstream>
#include <iomanip>
#include "Ray.h"

Ray::Ray(Eigen::Vector3f origin, Eigen::Vector3f direction) {
    _origin = origin;
    _direction = direction.normalized();
}

Eigen::Vector3f Ray::getPoint(float t) {
    return _origin + t * _direction;
}

std::string formatVector(Eigen::Vector3f v) {
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
        Eigen::DontAlignCols,
        " ", " ", "", "", "", "");

    std::stringstream str;
    str << std::setprecision(4);
    str << v.format(CommaInitFmt);

    return str.str();
}

std::string Ray::to_string() {
    std::stringstream str;
    str << "{" << formatVector(_origin) << "} -> {" << formatVector(_direction) << "}";
    return str.str();
}
