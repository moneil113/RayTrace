#include <sstream>
#include "Camera.h"
#include "Ray.h"

using namespace Eigen;
using namespace std;

Camera::Camera() {
    width = 640;
    height = 480;
}

Ray Camera::rayToPixel(int x, int y) {
    float u_s = -0.5f + (x + 0.5) / width;
    float v_s = -0.5f + (y + 0.5) / height;

    Vector3f S = u_s * right + v_s * up + look_at;

    return Ray(location, S.normalized());
}

Ray Camera::rayToSubPixel(int x, int y, int subX, int subY, int samples) {
    float subU = -0.5f + (subX + 0.5) / samples;
    float u_s = -0.5f + (x + 0.5 + subU) / width;
    float subV = -0.5f + (subY + 0.5) / samples;
    float v_s = -0.5f + (y + 0.5 + subV) / height;

    Vector3f S = u_s * right + v_s * up + look_at;

    return Ray(location, S.normalized());
}

void Camera::setImageSize(int width, int height) {
    this->width = width;
    this->height = height;

    look_at -= location;
    look_at.normalize();
}

std::string Camera::to_string() {
    stringstream str;
    str << "Camera:\n";
    str << "- Location: " << formatVector(location) << '\n';
    str << "- Up: " << formatVector(up) << '\n';
    str << "- Right: " << formatVector(right) << '\n';
    str << "- Look at: " << formatVector(look_at) << '\n';

    return str.str();
}
