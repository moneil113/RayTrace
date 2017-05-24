#include <sstream>
#include <iomanip>
#include "Ray.h"

Ray::Ray(Eigen::Vector3f origin, Eigen::Vector3f direction) {
    _origin = origin;
    _direction = direction;
}

Eigen::Vector3f Ray::getPoint(float t) const {
    return _origin + t * _direction;
}

std::string formatVector(Eigen::Vector3f v) {
    std::stringstream str;
    str << std::fixed << std::setprecision(4);
    str << v.x() << " " << v.y() << " " << v.z();

    return str.str();
}

std::string Ray::to_string() const {
    std::stringstream str;
    str << "{" << formatVector(_origin) << "} -> {" << formatVector(_direction) << "}";
    return str.str();
}
