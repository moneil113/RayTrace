#include "Object.h"
#include <sstream>
#include <iomanip>

Object::Object() {

}

std::string Object::formatVector(Eigen::Vector3f v) {
    std::stringstream str;
    str << std::setprecision(4) << std::fixed;
    str << "{" << v.x() << " " << v.y() << " " << v.z() << "}";

    return str.str();
}
