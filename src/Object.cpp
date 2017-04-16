#include "Object.h"

Object::Object() {

}

std::string Object::formatVector(Eigen::Vector3f v) {
    std::stringstream str;
    str << std::setprecision(4);
    str << "{" << v.x() << " " << v.y() << " " << v.z() << "}";

    return str.str();
}
