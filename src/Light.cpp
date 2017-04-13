#include "Light.h"

Light::Light() {

}

std::string Light::to_string() {
    std::stringstream str;

    str << "- Location: " << formatVector(location) << '\n';
    str << "- Color: " << formatVector(color) << '\n';

    return str.str();
}
