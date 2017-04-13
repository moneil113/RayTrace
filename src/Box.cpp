#include <sstream>
#include "Box.h"

Box::Box() {

}

std::string Box::to_string() {
    std::stringstream str;
    str << "box:\n";
    str << "      corner1  = " << formatVector(corner1) << "\n";
    str << "      corner2  = " << formatVector(corner2) << "\n";
    str << Geometry::to_string();

    return str.str();
}

std::string Box::type() {
    return "Box";
}
