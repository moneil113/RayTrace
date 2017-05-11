#include "Renderer.h"
#include "Scene.h"
#include "Ray.h"

#include <math.h>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace Eigen;

Renderer::Renderer(Scene *sc) {
    pixels = NULL;
    scene = sc;
    brdf = 0;
}

Color_t Renderer::colorFromVector(Eigen::Vector3f &v) {
    Color_t color;
    color.r = (unsigned char) fminf(round(v.x() * 255.f), 255);
    color.g = (unsigned char) fminf(round(v.y() * 255.f), 255);
    color.b = (unsigned char) fminf(round(v.z() * 255.f), 255);

    return color;
}

void Renderer::setBRDF(int type) {
    brdf = type;
    if (brdf == 0) {
        localColor = &Renderer::blinnPhongColor;
    }
    else if (brdf == 1) {
        localColor = &Renderer::cookTorranceColor;
    }
    else {
        std::cerr << "Unkown brdf. Exiting" << '\n';
        exit(-1);
    }
}

void Renderer::setImageSize(int width, int height) {
    if (pixels) {
        delete pixels;
    }

    pixels = new Color_t[width * height];
    this->width = width;
    this->height = height;
}

bool Renderer::inShadow(const Eigen::Vector3f &point, const Light &light) {
    shared_ptr<Geometry> hit = NULL;

    Vector3f l = (light.getLocation() - point).normalized();
    floatOptional t;
    Ray lightRay = Ray(point, l);
    hit = scene->firstHit(lightRay, t);

    //Check if we hit something behind the light
    if (hit) {
        if ((light.getLocation() - point).norm() < t.value) {
            return false;
        }
        else {
            return true;
        }
    }
    return false;
}

Eigen::Vector3f Renderer::calculateColor(Ray &r, float t, std::shared_ptr<Geometry> object, int depth) {
    Vector3f p = r.getPoint(t);
    Vector3f local = (this->*localColor)(r, object, p);

    if (depth <= maxBounces) {
        Vector3f d = r.direction();
        Vector3f n = object->normalAtPoint(p);
        Ray reflectRay = Ray(p, (d - 2 * d.dot(n) * n).normalized());

        floatOptional rt;
        auto reflectObject = scene->firstHit(reflectRay, rt);

        Vector3f reflectColor;
        if (reflectObject) {
            reflectColor = calculateColor(reflectRay, rt.value, reflectObject, depth + 1);
        }
        else {
            reflectColor << 0, 0, 0;
        }

        Finish_t finish = object->getFinish();
        return (1 - finish.reflection) * local + finish.reflection * reflectColor;
    }
    else {
        return local;
    }
}

Eigen::Vector3f Renderer::blinnPhongColor(Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p) {
    Vector3f pigment = object->color();

    Vector3f ka = object->getFinish().ambient * pigment;
    Vector3f kd = object->getFinish().diffuse * pigment;
    Vector3f ks = object->getFinish().specular * pigment;
    float power = object->getFinish().roughness;
    power = (2 / (power * power) - 2);

    Vector3f color = ka;

    Vector3f n = object->normalAtPoint(p);
    Vector3f v = -r.direction().normalized();

    for (int i = 0; i < scene->lights.size(); i++) {
        auto light = scene->lights.at(i);
        Vector3f l = (light.getLocation() - p).normalized();

        if (!inShadow(p, light)) {
            Vector3f h = (v + l).normalized();
            Vector3f lc = light.getColor();
            Vector3f diff = blinnPhongDiffuse(n, l, kd, lc);
            Vector3f spec = blinnPhongSpecular(n, h, ks, power, lc);
            color += diff;
            color += spec;
        }
    }

    return color;
}

Vector3f Renderer::blinnPhongDiffuse(Eigen::Vector3f &n, Eigen::Vector3f &l,
    Eigen::Vector3f &kd, Eigen::Vector3f &lightColor)
{
    Vector3f temp = kd * fmaxf(0, n.dot(l));
    temp = temp.cwiseProduct(lightColor);
    return temp;
}

Vector3f Renderer::blinnPhongSpecular(Eigen::Vector3f &n, Eigen::Vector3f &h,
    Eigen::Vector3f &ks, float power, Eigen::Vector3f &lightColor)
{
    Vector3f temp = ks * powf(fmaxf(0, h.dot(n)), power);
    temp = temp.cwiseProduct(lightColor);
    return temp;
}

Eigen::Vector3f Renderer::cookTorranceColor(Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p) {
    Vector3f pigment = object->color();

    Vector3f ka = object->getFinish().ambient * pigment;
    Vector3f kd = object->getFinish().diffuse * pigment;
    Vector3f ks = object->getFinish().specular * pigment;
    float roughness = object->getFinish().roughness;

    Vector3f color = ka;

    Vector3f n = object->normalAtPoint(p);
    Vector3f v = -r.direction().normalized();

    for (int i = 0; i < scene->lights.size(); i++) {
        auto light = scene->lights.at(i);
        Vector3f l = (light.getLocation() - p).normalized();

        if (!inShadow(p, light)) {
            Vector3f h = (v + l).normalized();
            Vector3f lc = light.getColor();
            Vector3f rd = kd * fmaxf(0, n.dot(l));

            float d = D_ggx(h, n, roughness);

            float g = G_ggx(l, v, n, roughness);

            Finish_t finish = object->getFinish();
            float ior = finish.ior;
            float f = fresnel(ior, v, h);

            Vector3f rs = ks * d * g * f / (4 * n.dot(v));

            float s = finish.metallic;

            color += lc.cwiseProduct(((1 - s) * rd + s * rs));
        }
    }

    return color;
}

float Renderer::D_ggx(Eigen::Vector3f &m, Eigen::Vector3f &n, float alpha) {
    float m_n = m.dot(n);
    float alpha2 = alpha * alpha;
    float numerator = alpha2 * (m_n > 0);
    float denominator = M_PI * m_n * m_n * powf((alpha2 + (1-(m_n * m_n)) / (m_n * m_n)), 2);

    return numerator / denominator;
}

float Renderer::G_ggx(Eigen::Vector3f &l, Eigen::Vector3f &v, Eigen::Vector3f &n, float alpha) {
    Vector3f h = (v + l).normalized();
    return G_ggxPart(v, h, n, alpha) * G_ggxPart(l, h, n, alpha);
}

float Renderer::G_ggxPart(Eigen::Vector3f &x, Eigen::Vector3f &m, Eigen::Vector3f &n, float alpha) {
    if (x.dot(n) / x.dot(m) > 0) {
        float m_n = n.dot(m);
        float tan2 = (1 - m_n * m_n)/(m_n * m_n);

        return 2 / (1 + sqrtf(1 + alpha * alpha * tan2));
    }
    else {
        return 0;
    }
}

float Renderer::fresnel(float ior, Eigen::Vector3f &v, Eigen::Vector3f &h) {
    float f0 = powf(ior - 1, 2) / powf(ior + 1, 2);
    return f0 + (1 - f0) * powf((1 - (v.dot(h))), 5);
}

void Renderer::renderScene(std::string output) {
    shared_ptr<Geometry> hitObject;
    floatOptional t;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray r = scene->camera.rayToPixel(x, y);
            hitObject = scene->firstHit(r, t);
            if (hitObject) {
                Vector3f col = calculateColor(r, t.value, hitObject, 0);
                pixels[width * (height - 1- y) + x] = colorFromVector(col);
            }
            else {
                pixels[width * (height - 1- y) + x] = {0, 0, 0};
            }
        }
    }

    stbi_write_png(output.c_str(), width, height, 3, pixels, width * sizeof(Color_t));
}

void Renderer::pixelColorTest(int x, int y) {
    Ray r = scene->camera.rayToPixel(x, y);
    floatOptional t;
    shared_ptr<Geometry> hitObject = scene->firstHit(r, t);

    if (hitObject) {
        Vector3f col = calculateColor(r, t.value, hitObject, 0);
        Color_t color = colorFromVector(col);
        cout << "BRDF: ";
        if (brdf == 0) {
            cout << "Blinn-Phong\n";
        }
        else {
            cout << "Cook-Torrance\n";
        }
        cout << "Color: " << color << "\n";
    }
}
