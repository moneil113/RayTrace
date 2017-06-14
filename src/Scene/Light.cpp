#include "Light.h"

Light::Light() {

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
