#include "Renderer.h"
#include "Scene.h"
#include "Ray.h"

#include <math.h>

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

Color_t Renderer::calculateColor(Ray &r, float t, std::shared_ptr<Geometry> object) {
    Vector3f pigment = object->color();

    Vector3f ka = object->getFinish().ambient * pigment;
    Vector3f kd = object->getFinish().diffuse * pigment;
    // Vector3f ks = object->getFinish().specular * pigment;

    Color_t color = colorFromVector(ka);
    shared_ptr<Geometry> hit;

    Vector3f p = r.getPoint(t);
    Vector3f n = object->normalAtPoint(p);
    Vector3f v = (r.origin() - p).normalized();

    Vector3f l, h, lColor, temp;
    for (int i = 0; i < scene->lights.size(); i++) {
        l = (scene->lights.at(i).getLocation() - p).normalized();
        floatOptional ft;
        hit = scene->firstHit(Ray(p, l), ft);
        if (!hit) {
            h = (l + v).normalized();
            lColor = scene->lights.at(i).getColor();
            temp = kd * fmaxf(0, n.dot(l));
            color += colorFromVector(temp);
        }
    }

    return color;
}

void Renderer::renderScene(std::string output) {
    shared_ptr<Geometry> hitObject;
    floatOptional t;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray r = scene->camera.rayToPixel(x, y);
            hitObject = scene->firstHit(r, t);
            if (hitObject) {
                // Vector3f color = hitObject->color();
                // pixels[width * (height - 1- y) + x] = colorFromVector(color);
                pixels[width * (height - 1- y) + x] = calculateColor(r, t.value, hitObject);
            }
            else {
                pixels[width * (height - 1- y) + x] = {0, 0, 0};
            }
        }
    }

    stbi_write_png(output.c_str(), width, height, 3, pixels, width * sizeof(Color_t));
}
