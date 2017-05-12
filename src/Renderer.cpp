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

Color_t Renderer::colorFromVector(const Eigen::Vector3f &v) {
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

Vector3f Renderer::reflect(const Ray &r, const Vector3f &p, const shared_ptr<Geometry> object, int depth) {
    Vector3f d = r.direction();
    Vector3f n = object->normalAtPoint(p);
    Vector3f dir = (d - 2 * d.dot(n) * n).normalized();
    Ray reflectRay = Ray(p + epsilon * dir, dir);

    floatOptional t;
    auto reflectObject = scene->firstHit(reflectRay, t);

    if (reflectObject) {
        Vector3f rp = reflectRay.getPoint(t.value);
        return calculateColor(reflectRay, rp, reflectObject, depth + 1);
    }
    return Vector3f(0, 0, 0);
}

Vector3f Renderer::refract(const Ray &r, const Vector3f &p, const shared_ptr<Geometry> object, int depth, bool enter) {
    Vector3f d = r.direction();
    Vector3f n = object->normalAtPoint(p);
    float n2 = object->getFinish().ior;
    float n1_n2 = 1 / n2;
    float d_n = d.dot(n);

    if (!enter) {
        n1_n2 = n2;
        n = -n;
    }

    Vector3f t = (n1_n2) * (d + d.dot(n) * n);
    t -= n * sqrtf(1 - (n1_n2) * (n1_n2 * n1_n2) * (1 - d_n * d_n));
    t.normalize();

    Ray refractRay = Ray(p + epsilon * t, t);

    if (!enter) {
        // exiting the object
        floatOptional t;
        auto refractObject = scene->firstHit(refractRay, t);
        if (refractObject) {
            Vector3f rp = refractRay.getPoint(t.value);
            return calculateColor(refractRay, rp, refractObject, depth + 1);
        }
        else {
            return Vector3f(0, 0, 0);
        }
    }
    else {
        floatOptional t = object->intersect(refractRay);
        Vector3f rp = refractRay.getPoint(t.value);
        return refract(refractRay, rp, object, depth + 1, false);
    }
}

Vector3f Renderer::calculateColor(Ray &r, const Vector3f &p, shared_ptr<Geometry> object, int depth) {
    Vector3f local = (this->*localColor)(r, object, p);

    if (depth <= maxBounces) {
        Finish_t finish = object->getFinish();

        Vector3f reflectColor;
        if (finish.reflection > 0) {
            reflectColor = reflect(r, p, object, depth);
        }

        Vector3f refractColor;
        if (finish.filter > 0) {
            refractColor = refract(r, p, object, depth, true);
        }

        float localContribution = (1 - finish.filter) * (1 - finish.reflection);
        float reflectContribution = (1 - finish.filter) * finish.reflection;
        float refractContribution = finish.filter * (1 - finish.reflection);

        // cout << "local:\n" << local << "\nat " << localContribution << endl;
        // cout << "reflect:\n" << reflectColor << "\nat " << reflectContribution << endl;
        // cout << "refract:\n" << refractColor << "\nat " << refractContribution << endl;
        return localContribution * local +
            reflectContribution * reflectColor +
            refractContribution * refractColor;
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
                Vector3f p = r.getPoint(t.value);
                Vector3f col = calculateColor(r, p, hitObject, 0);
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
        Vector3f p = r.getPoint(t.value);
        Vector3f col = calculateColor(r, p, hitObject, 0);
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

void Renderer::pixelTraceTest(int x, int y) {
    trace = true;
    Ray r = scene->camera.rayToPixel(x, y);
    floatOptional t;
    auto hitObject = scene->firstHit(r, t);

    if (hitObject) {
        Vector3f p = r.getPoint(t.value);
        calculateColor(r, p, hitObject, 0);
    }
    else {
        cout << "no hit\n";
    }
}

void Renderer::printRayInfo(const Ray &r, const Vector3f &color, int type, int depth) {
    string indent = "";
    for (int i = 0; i < depth; i++) {
        indent += " ";
    }
    if (type == 0) {
        cout << "o - Type: Primary\n";
    }
    else if (type == 1) {
        cout << indent << "\\\n";
        indent = " " + indent;
        cout << indent << "o - Type: Reflection\n";
    }
    else if (type == 2) {
        cout << indent << "\\\n";
        indent += "|";
        cout << "o - Type: Refraction\n";
    }
    cout << indent << "|   Ray: " << r.to_string() << endl;
    cout << indent << "|   Color: " << color.x() << ", " << color.y() << ", " << color.z() << endl;;
}
