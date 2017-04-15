#include "Renderer.h"
#include "Scene.h"
#include "Ray.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace Eigen;

Renderer::Renderer(Scene *sc) {
    pixels = NULL;
    scene = sc;
}

Color_t Renderer::colorFromVector(Eigen::Vector3f &v) {
    Color_t color;
    color.r = (unsigned char) round(v.x() * 255.f);
    color.g = (unsigned char) round(v.y() * 255.f);
    color.b = (unsigned char) round(v.z() * 255.f);

    return color;
}

void Renderer::setImageSize(int width, int height) {
    if (pixels) {
        delete pixels;
    }

    pixels = new Color_t[width * height];
    this->width = width;
    this->height = height;
}

void Renderer::renderScene(std::string output) {
    shared_ptr<Geometry> hitObject;
    floatOptional t;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            hitObject = scene->firstHit(x, y, t);
            if (hitObject) {
                Vector3f color = hitObject->color();
                pixels[width * (height - 1- y) + x] = colorFromVector(color);
            }
            else {
                pixels[width * (height - 1- y) + x] = {0, 0, 0};
            }
        }
    }

    stbi_write_png(output.c_str(), width, height, 3, pixels, width * sizeof(Color_t));
    // stbi_write_png(fileName.c_str(), size.x, size.y, numChannels, data, size.x * numChannels);
}
