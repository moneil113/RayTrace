#include <sstream>
#include "Plane.h"

Plane::Plane() {

}

std::string Plane::to_string() {
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
        Eigen::DontAlignCols,
        ", ", ", ", "", "", "", ";");

    std::stringstream str;
    str << "plane:\n";
    str << "      normal   = " << formatVector(normal) << "\n";
    str << "      distance = " << distance << "\n";
    str << Geometry::to_string();

    return str.str();
}
