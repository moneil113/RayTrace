#include <sstream>
#include "Camera.h"
#include "Ray.h"

#include <iostream>

using namespace Eigen;

Camera::Camera() {
    width = 640;
    height = 480;
}

Ray Camera::rayToPixel(int x, int y) {
    // std::cout << "in " << '\n';
    float r = right.norm() / 2;
    float l = -r;
    float t = up.norm() / 2;
    float b = -t;

    float u_s = l + (r - l) * (x + 0.5) / width;
    float v_s = b + (t - b) * (y + 0.5) / height;

    Vector3f S = u_s * right + v_s * up + look_at;

    return Ray(location, S);
}

void Camera::setImageSize(int width, int height) {
    this->width = width;
    this->height = height;
}

std::string Camera::to_string() {
    std::stringstream str;
    str << "Camera: \n";
    str << "- Location: " << formatVector(location) << '\n';
    str << "- Up: " << formatVector(up) << '\n';
    str << "- Right: " << formatVector(right) << '\n';
    str << "- Look at: " << formatVector(look_at) << '\n';

    return str.str();
}
