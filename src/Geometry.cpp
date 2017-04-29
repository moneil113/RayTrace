#include <sstream>
#include "Geometry.h"

static const std::string transformString(int type) {
    switch (type) {
        case TRANSFORM_TRANSLATE:
            return "translate: ";
        case TRANSFORM_ROTATE:
            return "rotate:    ";
        case TRANSFORM_SCALE:
            return "scale:     ";
        default:
            return "?";
    }
}

std::string Geometry::to_string() {
    std::stringstream str;
    str << "- Color: " << formatVector(pigment) << "\n";
    str << "- Material:\n";
    str << "  - Ambient: " << finish.ambient << "\n";
    str << "  - Diffuse: " << finish.diffuse << "\n";
    str << "  - Specular: " << finish.specular << "\n";
    str << "  - Roughness: " << finish.roughness << "\n";
    // for (size_t i = 0; i < transformTypes.size(); i++) {
    //     str << "        " << transformString(transformTypes.at(i));
    //     str << formatVector(transformValues.at(i)) << "\n";
    // }

    return str.str();
}

void Geometry::addTransform(int type, Eigen::Vector3f value) {
    transformTypes.push_back(type);
    transformValues.push_back(value);
}
