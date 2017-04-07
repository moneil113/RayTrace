#include <sstream>
#include "Sphere.h"

Sphere::Sphere() {

}

std::string Sphere::to_string() {
    std::stringstream str;
    str << "sphere:\n";
    str << "      center   = " << formatVector(center) << "\n";
    str << "      radius   = " << radius << "\n";
    str << Geometry::to_string();

    return str.str();
}
