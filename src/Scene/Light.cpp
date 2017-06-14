#include "Light.h"

Light::Light() {

}

Eigen::Vector3f Light::samplePosition(const int i, const int j) const {
    if (rowSamples == 1 && columnSamples == 1) {
        return location;
    }
    Eigen::Vector3f pos = corner + i * (axis1 / rowSamples);
    pos += j * (axis2 / columnSamples);
    return pos;
}

float Light::sampleWeight() const {
    return 1.0 / (rowSamples * columnSamples);
}

std::string Light::to_string() {
    std::stringstream str;

    str << "- Location: " << formatVector(location) << '\n';
    str << "- Color: " << formatVector(color) << '\n';
    if (areaLight) {
        str << "- Axis 1: " << formatVector(axis1) << '\n';
        str << "- Axis 2: " << formatVector(axis2) << '\n';
        str << "- Row samples: " << rowSamples << '\n';
        str << "- Column samples: " << columnSamples << '\n';
    }

    return str.str();
}
