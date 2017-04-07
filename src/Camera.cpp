#include <sstream>
#include "Camera.h"

Camera::Camera() {

}

std::string Camera::to_string() {
    std::stringstream str;
    str << "Camera: \n";
    str << "      loc     = " << formatVector(location) << '\n';
    str << "      up      = " << formatVector(up) << '\n';
    str << "      right   = " << formatVector(right) << '\n';
    str << "      look_at = " << formatVector(look_at) << '\n';

    return str.str();
}
