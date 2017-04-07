#include "Light.h"

Light::Light() {

}

std::string Light::to_string() {
    std::stringstream str;

    str << "Light:\n";
    str << "      loc   = " << formatVector(location) << '\n';
    str << "      color = " << formatVector(color) << '\n';

    return str.str();
}
