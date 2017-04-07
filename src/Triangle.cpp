#include <sstream>
#include "Triangle.h"

Triangle::Triangle() {

}

std::string Triangle::to_string() {
    std::stringstream str;
    str << "triangle:\n";
    str << "      corner1  = " << formatVector(corner1) << "\n";
    str << "      corner1  = " << formatVector(corner2) << "\n";
    str << "      corner1  = " << formatVector(corner3) << "\n";
    str << Geometry::to_string();

    return str.str();
}
