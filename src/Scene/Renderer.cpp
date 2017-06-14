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

void Renderer::useGlobalIllumination() {
    globalIlluminationOn = true;
}

void Renderer::setGISamples(int n) {
    giSamples = n;
}

void Renderer::setGIBounces(int n) {
    giBounces = n;
}

void Renderer::setGIRatio(int n) {
    giRatio = n;
}

bool Renderer::inShadow(const Eigen::Vector3f &point, const Eigen::Vector3f &lightPos) {
    shared_ptr<Geometry> hit = NULL;

    Vector3f l = (lightPos - point).normalized();
    floatOptional t;
    Ray lightRay = Ray(point + epsilon * l, l);
    hit = scene->firstHit(lightRay, t);

    //Check if we hit something behind the light
    if (hit) {
        if ((lightPos - point).norm() < t.value) {
            return false;
        }
        else {
            return true;
        }
    }
    return false;
}

Vector3f Renderer::monteCarloSample(const float u, const float v) {
    const float radial = sqrt(u);
    const float theta = 2.0 * M_PI * v;

    const float x = radial * cos(theta);
    const float y = radial * sin(theta);

    return Vector3f(x, y, sqrt(1 - u));
}

Ray Renderer::monteCarloRay(const Matrix4f &rotation, const Vector3f &n, const Vector3f &p) {
    const float u = rand() / (float) RAND_MAX;
    const float v = rand() / (float) RAND_MAX;

    Vector4f dir;
    dir << monteCarloSample(u, v), 0;
    dir = rotation * dir;

    return Ray(p + epsilon * dir.head(3), dir.head(3));
}


Vector3f Renderer::monteCarloAmbient(const Vector3f &p, const std::shared_ptr<Geometry> object, const int depth, const int samples) {
    Vector3f ambient = Vector3f(0, 0, 0);

    const Vector3f n = object->normalAtPoint(p);

    float angle = acos(Vector3f::UnitZ().dot(n));
    Vector3f axis = Vector3f::UnitZ().cross(n);

    Matrix4f rotation = Matrix4f::Identity();
    rotation.block<3, 3>(0, 0) = (AngleAxisf(angle, axis)).toRotationMatrix();

    for (int i = 0; i < samples; i++) {
        const Ray r = monteCarloRay(rotation, n, p);
        floatOptional t;
        auto hitObj = scene->firstHit(r, t);
        if (hitObj) {
            const Vector3f hitP = r.getPoint(t.value);
            const Vector3f color = calculateColor(r, hitP, hitObj, depth + 1);
            ambient += (1.0 / samples) * color;
        }
    }

    return ambient;
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

Vector3f Renderer::beersLawColor(const Eigen::Vector3f &refractColor, const Eigen::Vector3f &objectColor, float distance) {
    Vector3f absorbance = (Vector3f(1, 1, 1) - objectColor) * 0.15f * -distance;
    Vector3f attn = Vector3f(exp(absorbance.x()), exp(absorbance.y()), exp(absorbance.z()));
    return refractColor.cwiseProduct(attn);
}

Vector3f Renderer::refract(const Ray &r, const Vector3f &p, const shared_ptr<Geometry> object, int depth) {
    Vector3f d = r.direction();
    Vector3f n = object->normalAtPoint(p);
    float ior = object->getFinish().ior;
    float n1_n2 = 1 / ior;

    // inside object
    if (r.refracted) {
        n1_n2 = ior;
        n = -n;
    }
    float d_n = d.dot(n);

    Vector3f t = (n1_n2) * (d - d_n * n);
    t -= n * sqrtf(1 - (n1_n2 * n1_n2) * (1 - d_n * d_n));
    t.normalize();

    Ray refractRay = Ray(p + epsilon * t, t);
    // if incoming ray is inside object, this ray is outside (and vice versa)
    refractRay.refracted = !r.refracted;

    // inside object
    if (r.refracted) {
        floatOptional t;
        auto refractObject = scene->firstHit(refractRay, t);
        if (refractObject) {
            Vector3f rp = refractRay.getPoint(t.value);
            if (trace) {
                printRayInfo(refractRay, object, 2, depth);
            }
            return calculateColor(refractRay, rp, refractObject, depth + 1);
        }
        else {
            if (trace) {
                printRayInfo(refractRay, object, 2, depth);
            }
            return Vector3f(0, 0, 0);
        }
    }
    else {
        floatOptional t = object->intersect(refractRay);
        Vector3f rp = refractRay.getPoint(t.value);
        refractRay.refracted = true;
        if (trace) {
            printRayInfo(refractRay, object, 2, depth);
        }
        Vector3f color = calculateColor(refractRay, rp, object, depth + 1);
        return beersLawColor(color, object->color(), t.value);
    }
}

float Renderer::schlicksApproximation(const float ior, const Vector3f &n, const Vector3f &v) {
    float f0 = powf((ior - 1) / (ior + 1), 2);
    return f0 + (1 - f0) * powf(1 - n.dot(v), 5);
}

Vector3f Renderer::calculateColor(const Ray &r, const Vector3f &p, shared_ptr<Geometry> object, int depth) {
    Vector3f local = (this->*localColor)(r, object, p);

    if (depth <= maxBounces) {
        if (globalIlluminationOn) {
            if (depth < giBounces) {
                int samples = giSamples / powf(giRatio, depth);
                local += monteCarloAmbient(p, object, depth, samples);
            }
        }
        Finish_t finish = object->getFinish();

        float fresnelReflectance = 0;

        if (doFresnel && finish.filter > 0 && !r.refracted) {
            Vector3f n = object->normalAtPoint(p);
            Vector3f v = -r.direction();
            fresnelReflectance = schlicksApproximation(finish.ior, n, v);
        }

        Vector3f reflectColor;
        if ((finish.reflection > 0 || fresnelReflectance > 0) && !r.refracted) {
            reflectColor = reflect(r, p, object, depth);
        }
        else {
            reflectColor << 0, 0, 0;
        }

        Vector3f refractColor;
        if (finish.filter > 0) {
            floatOptional t;
            t = object->intersect(r);
            refractColor = refract(r, p, object, depth);
        }
        else {
            refractColor << 0, 0, 0;
        }

        float localContribution = (1 - finish.filter) * (1 - finish.reflection);
        float reflectContribution = (1 - finish.filter) * finish.reflection
            + finish.filter * fresnelReflectance;
        float refractContribution = finish.filter * (1 - fresnelReflectance);

        if (trace) {
            cout << "      Reflection: " << object->formatVector(reflectColor) << endl;
            cout << "      Refraction: " << object->formatVector(refractColor) << endl;
            cout << fixed << setprecision(4);
            cout << "   Contributions: " << localContribution << " Local, ";
            cout << reflectContribution << " Reflection, ";
            cout << refractContribution << " Transmission\n";
        }

        return localContribution * local +
            reflectContribution * reflectColor.cwiseProduct(object->color()) +
            refractContribution * refractColor;
    }
    else {
        return local;
    }
}

Eigen::Vector3f Renderer::blinnPhongColor(const Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p) {
    Vector3f pigment = object->color();

    Vector3f ka;
    if (globalIlluminationOn) {
        ka << 0, 0, 0;
    }
    else {
        ka = (object->getFinish().ambient * pigment);
    }
    Vector3f kd = object->getFinish().diffuse * pigment;
    Vector3f ks = object->getFinish().specular * pigment;
    float power = object->getFinish().roughness;
    power = (2 / (power * power) - 2);

    Vector3f color = ka;

    Vector3f n = object->normalAtPoint(p);
    // if we are look at the back of a plane/triangle, we want the opposite of the normal
    // or if we are inside of an object
    if (n.dot(r.direction()) > 0) {
        n = -n;
    }
    Vector3f v = -r.direction().normalized();

    for (int i = 0; i < scene->lights.size(); i++) {
        auto light = scene->lights.at(i);
        Vector3f l = (light.getLocation() - p).normalized();
        const float sampleWeight = light.sampleWeight();

        for (int i = 0; i < light.getRowSamples(); i++) {
            for (size_t j = 0; j < light.getColumnSamples(); j++) {
                if (!inShadow(p, light.samplePosition(i, j))) {
                    Vector3f h = (v + l).normalized();
                    Vector3f lc = light.getColor();
                    Vector3f diff = blinnPhongDiffuse(n, l, kd, lc);
                    Vector3f spec = blinnPhongSpecular(n, h, ks, power, lc);
                    color += diff * sampleWeight;
                    color += spec * sampleWeight;

                    if (trace) {
                        cout << "         Ambient: " << object->formatVector(ka) << endl;
                        cout << "         Diffuse: " << object->formatVector(diff) << endl;
                        cout << "        Specular: " << object->formatVector(spec) << endl;
                    }
                }
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

    Vector3f ka;
    if (globalIlluminationOn) {
        ka << 0, 0, 0;
    }
    else {
        ka = (object->getFinish().ambient * pigment);
    }
    Vector3f kd = object->getFinish().diffuse * pigment;
    Vector3f ks = object->getFinish().specular * pigment;
    float roughness = object->getFinish().roughness;

    Vector3f color = ka;

    Vector3f n = object->normalAtPoint(p);
    Vector3f v = -r.direction().normalized();

    for (int i = 0; i < scene->lights.size(); i++) {
        auto light = scene->lights.at(i);
        Vector3f l = (light.getLocation() - p).normalized();
        const float sampleWeight = light.sampleWeight();

        for (int i = 0; i < light.getRowSamples(); i++) {
            for (int j = 0; j < light.getColumnSamples(); j++) {
                if (!inShadow(p, light.samplePosition(i, j))) {
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

                    color += lc.cwiseProduct(((1 - s) * rd + s * rs)) * sampleWeight;
                }
            }
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
