#include <sstream>
#include "Cone.h"

Cone::Cone() {

}

std::string Cone::to_string() {
    std::stringstream str;
    str << "cone:\n";
    str << "      base     = " << formatVector(base) << "\n";
    str << "      |radius  = " << baseRadius << "\n";
    str << "      cap      = " << formatVector(cap) << "\n";
    str << "      |radius  = " << capRadius << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Cone::type() {
    return "Cone";
}
