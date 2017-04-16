#include "Object.h"

Object::Object() {

}

std::string Object::formatVector(Eigen::Vector3f v) {
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
        Eigen::DontAlignCols,
        "", "", "", "", "{", "}");

    std::stringstream str;
    str << std::setprecision(4);
    str << v.format(CommaInitFmt);

    return str.str();
}
