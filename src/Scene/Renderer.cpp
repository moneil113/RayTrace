#include "Renderer.h"
#include "Scene.h"
#include "Ray.h"

#include <math.h>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Util/stb_image_write.h"

using namespace std;
using namespace Eigen;

Renderer::Renderer(Scene *sc) {
    pixels = NULL;
    scene = sc;
    brdf = 0;
    localColor = &Renderer::blinnPhongColor;
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
        std::cerr << "Unknown brdf. Exiting" << '\n';
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

void Renderer::setSuperSamples(int n) {
    superSamples = n;
}

void Renderer::setFresnel() {
    doFresnel = true;
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
        if (trace) {
            printRayInfo(reflectRay, reflectObject, 1, depth);
        }
        return calculateColor(reflectRay, rp, reflectObject, depth + 1);
    }
    return Vector3f(0, 0, 0);
}

// Vector3f Renderer::refract(const Ray &r, const Vector3f &p, float dist, const shared_ptr<Geometry> object, int depth) {
//     Vector3f d = r.direction();
//     Vector3f n = object->normalAtPoint(p);
//     float ior = object->getFinish().ior;
//     float n1_n2 = 1 / ior;
//
//     // exiting object
//     if (r.refracted) {
//         n1_n2 = ior;
//         n = -n;
//     }
//     float d_n = d.dot(n);
//
//     Vector3f t = (n1_n2) * (d - d_n * n);
//     t -= n * sqrtf(1 - (n1_n2 * n1_n2) * (1 - d_n * d_n));
//     t.normalize();
//
//     Ray refractRay = Ray(p + epsilon * t, t);
//
//     if (r.refracted) {
//         // exiting the object
//         floatOptional t;
//         auto refractObject = scene->firstHit(refractRay, t);
//         if (refractObject) {
//             Vector3f rp = refractRay.getPoint(t.value);
//             if (trace) {
//                 printRayInfo(refractRay, object, 2, depth);
//             }
//             // Beer's law
//             Vector3f otherColor = calculateColor(refractRay, rp, refractObject, depth + 1);
//             Vector3f absorbance = (Vector3f(1, 1, 1) - object->color()) * 0.15 * -dist;
//             // cout << "d = " << dist << endl;
//             // cout << "absorbance: " << object->formatVector(absorbance) << endl;
//             // Vector3f local = (this->*localColor)(r, object, p);
//             Vector3f attnColor;
//             attnColor = otherColor.cwiseProduct(Vector3f(exp(absorbance.x()),
//                                                          exp(absorbance.y()),
//                                                          exp(absorbance.z())));
//             // cout << "otherColor: " << object->formatVector(otherColor) << endl;
//             // cout << "attnColor: " << object->formatVector(attnColor) << endl;
//             return attnColor;
//         }
//         else {
//             if (trace) {
//                 printRayInfo(refractRay, object, 2, depth);
//             }
//             return Vector3f(0, 0, 0);
//         }
//     }
//     else {
//         floatOptional t = object->intersect(refractRay);
//         Vector3f rp = refractRay.getPoint(t.value);
//         refractRay.refracted = true;
//         if (trace) {
//             printRayInfo(refractRay, object, 2, depth);
//         }
//         return refract(refractRay, rp, t.value, object, depth + 1);
//         // return calculateColor(refractRay, rp, object, depth + 1);
//     }
// }

Vector3f Renderer::refract(const Ray &r, const Vector3f &p, const shared_ptr<Geometry> object, int depth) {
    return Vector3f(0, 0, 0);
}

Vector3f Renderer::calculateColor(const Ray &r, const Vector3f &p, shared_ptr<Geometry> object, int depth) {
    Vector3f local = (this->*localColor)(r, object, p);

    if (depth <= maxBounces) {
        Finish_t finish = object->getFinish();

        Vector3f reflectColor;
        if (finish.reflection > 0 && !r.refracted) {
            reflectColor = reflect(r, p, object, depth);
        }
        else {
            reflectColor << 0, 0, 0;
        }

        // Vector3f refractColor;
        // if (finish.filter > 0) {
        //     floatOptional t;
        //     t = object->intersect(r);
        //     refractColor = refract(r, p, t.value, object, depth);
        // }
        // else {
        //     refractColor << 0, 0, 0;
        // }

        Vector3f v = -r.direction();
        Vector3f n = object->normalAtPoint(p);

        float localContribution = (1 - 0/*finish.filter*/) * (1 - finish.reflection);
        float reflectContribution = (1 - 0/*finish.filter*/) * finish.reflection;
        if (doFresnel) {
            reflectContribution += finish.filter * fresnel(finish.ior, v, n);
        }
        float refractContribution = finish.filter * (1 - finish.reflection);

        if (trace) {
            cout << "      Reflection: " << object->formatVector(reflectColor) << endl;
            // cout << "      Refraction: " << object->formatVector(refractColor) << endl;
            cout << fixed << setprecision(4);
            cout << "   Contributions: " << localContribution << " Local, ";
            cout << reflectContribution << " Reflection, ";
            cout << refractContribution << " Transmission\n";
        }

        return localContribution * local +
            reflectContribution * reflectColor.cwiseProduct(object->color());// +
            // refractContribution * refractColor;
    }
    else {
        return local;
    }
}

Eigen::Vector3f Renderer::blinnPhongColor(const Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p) {
    Vector3f pigment = object->color();

    Vector3f ka = object->getFinish().ambient * pigment;
    Vector3f kd = object->getFinish().diffuse * pigment;
    Vector3f ks = object->getFinish().specular * pigment;
    float power = object->getFinish().roughness;
    power = (2 / (power * power) - 2);

    Vector3f color = ka;

    Vector3f n = object->normalAtPoint(p);
    // if we are look at the back of a plane/triangle, we want the opposite of the normal
    if (n.dot(r.direction()) > 0) {
        n = -n;
    }
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

            if (trace) {
                cout << "         Ambient: " << object->formatVector(ka) << endl;
                cout << "         Diffuse: " << object->formatVector(diff) << endl;
                cout << "        Specular: " << object->formatVector(spec) << endl;
            }
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

Eigen::Vector3f Renderer::cookTorranceColor(const Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p) {
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

            float s = finish.specular;

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

Eigen::Vector3f Renderer::pixelColor(int x, int y) {
    floatOptional t;
    const Ray r = scene->camera.rayToPixel(x, y);
    const auto hitObject = scene->firstHit(r, t);

    if (hitObject) {
        Vector3f p = r.getPoint(t.value);
        return calculateColor(r, p, hitObject, 0);
    }
    else {
        return Vector3f(0, 0, 0);
    }
}

Eigen::Vector3f Renderer::averagePixelColor(int x, int y) {
    floatOptional t;
    Vector3f color(0, 0, 0);
    for (int subX = 0; subX < superSamples; subX++) {
        for (int subY = 0; subY < superSamples; subY++) {
            const Ray r = scene->camera.rayToSubPixel(x, y, subX, subY, superSamples);
            const auto hitObject = scene->firstHit(r, t);
            if (hitObject) {
                Vector3f p = r.getPoint(t.value);
                color += calculateColor(r, p, hitObject, 0);
            }
        }
    }

    return color / (superSamples * superSamples);
}

void Renderer::renderScene(std::string output) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vector3f color;
            if (superSamples != 1) {
                color = averagePixelColor(x, y);
            }
            else {
                color = pixelColor(x, y);
            }

            pixels[width * (height - 1 - y) + x] = colorFromVector(color);
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

void Renderer::printRaysTest(int x, int y) {
    Ray r = scene->camera.rayToPixel(x, y);
    floatOptional t;
    auto hitObject = scene->firstHit(r, t);

    cout << "Pixel: [" << x << ", " << y << "] ";

    if (hitObject) {
        Vector3f p = r.getPoint(t.value);
        Vector3f color = calculateColor(r, p, hitObject, 0);
        cout << "Color: " << colorFromVector(color) << endl;
        cout << "----\n";
        printRayInfo(r, hitObject, 0, 0);
        trace = true;
        calculateColor(r, p, hitObject, 0);
    }
    else {
        cout << "no hit\n";
    }

    cout << endl << endl;
    cout << "--------------------------------------------------------------------------------";
    cout << endl << endl;
}

void Renderer::printRayInfo(const Ray &r, std::shared_ptr<Geometry> object, int type, int depth) {
    if (type == 0) {
        cout << "  Iteration type: Primary\n";
    }
    else if (type == 1) {
        cout << "----\n";
        cout << "  Iteration type: Reflection\n";
    }
    else if (type == 2) {
        cout << "----\n";
        cout << "  Iteration type: Refraction\n";
    }
    floatOptional t;
    t = object->intersect(r);
    Vector3f p = r.getPoint(t.value);
    Vector3f n = object->normalAtPoint(p);
    cout << "             Ray: " << r.to_string() << endl;
    cout << " Transformed Ray: " << object->getTransformedRay(r).to_string() << endl;;
    cout << "      Hit Object: " << object->type() << endl;;
    cout << "    Intersection: " << object->formatVector(p) << " at T = " << t.value << endl;;
    cout << "          Normal: " << object->formatVector(n) << endl;
}
