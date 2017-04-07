#include "Object.h"

Object::Object() {

}

std::string Object::formatVector(Eigen::Vector3f v) {
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
        Eigen::DontAlignCols,
        ", ", ", ", "", "", "", ";");

    std::stringstream str;
    str << v.format(CommaInitFmt);

    return str.str();
}
