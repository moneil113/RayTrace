#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <Eigen/Dense>

#include <iostream>

struct Color_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color_t operator+=(const Color_t &rhs)  {
        r = r  + rhs.r > 255 ? 255 : r + rhs.r;
        g = g  + rhs.g > 255 ? 255 : g + rhs.g;
        b = b  + rhs.b > 255 ? 255 : b + rhs.b;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream &os, const Color_t &rhs)
    {
        return os << "(" << (int) rhs.r << ", " << (int) rhs.g << ", " << (int) rhs.b << ")";
    }
};

class Scene;
class Ray;
class Light;
class Geometry;

class Renderer {
private:
    Color_t *pixels;
    int width, height;
    int brdf;
    Eigen::Vector3f (Renderer::*localColor)(const Ray &r,
        std::shared_ptr<Geometry> object, Eigen::Vector3f p) = NULL;

    const int maxBounces = 6;
    const float epsilon = 0.001f;
    bool trace = false;
    int superSamples = 1;
    bool doFresnel = false;
    bool globalIlluminationOn = false;
    int giSamples = 128;
    int giBounces = 2;
    int giRatio = 4;

    Scene *scene;

    bool inShadow(const Eigen::Vector3f &point, const Eigen::Vector3f &lightPos);

    Color_t colorFromVector(const Eigen::Vector3f &v);
    Eigen::Vector3f calculateColor(const Ray &r, const Eigen::Vector3f &p,
        std::shared_ptr<Geometry> object, int depth);

    Eigen::Vector3f monteCarloAmbient(const Eigen::Vector3f &p,
        const std::shared_ptr<Geometry> object, const int depth, const int samples);
    Ray monteCarloRay(const Eigen::Matrix4f &rotation, const Eigen::Vector3f &n,
        const Eigen::Vector3f &p);
    Eigen::Vector3f monteCarloSample(const float u, const float v);

    Eigen::Vector3f beersLawColor(const Eigen::Vector3f &refractColor,
        const Eigen::Vector3f &objectColor, float distance);
    float schlicksApproximation(const float ior, const Eigen::Vector3f &n,
        const Eigen::Vector3f &v);

    Eigen::Vector3f reflect(const Ray &r, const Eigen::Vector3f &p,
        const std::shared_ptr<Geometry> object, int depth);
    Eigen::Vector3f refract(const Ray &r, const Eigen::Vector3f &p,
        const std::shared_ptr<Geometry> object, int depth);

    Eigen::Vector3f blinnPhongColor(const Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p);
    Eigen::Vector3f cookTorranceColor(const Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p);

    Eigen::Vector3f blinnPhongDiffuse(Eigen::Vector3f &n, Eigen::Vector3f &l,
        Eigen::Vector3f &kd, Eigen::Vector3f &lightColor);
    Eigen::Vector3f blinnPhongSpecular(Eigen::Vector3f &n, Eigen::Vector3f &h,
        Eigen::Vector3f &ks, float power, Eigen::Vector3f &lightColor);

    float D_ggx(Eigen::Vector3f &m, Eigen::Vector3f &n, float alpha);
    float G_ggx(Eigen::Vector3f &l, Eigen::Vector3f &v, Eigen::Vector3f &n, float alpha);
    float G_ggxPart(Eigen::Vector3f &x, Eigen::Vector3f &m, Eigen::Vector3f &n, float alpha);
    float fresnel(float ior, Eigen::Vector3f &v, Eigen::Vector3f &h);

    Eigen::Vector3f pixelColor(int x, int y);
    Eigen::Vector3f averagePixelColor(int x, int y);

    void printRayInfo(const Ray &r, std::shared_ptr<Geometry> object, int type, int depth);

public:
    Renderer(Scene *sc);

    void setBRDF(int type);
    void setImageSize(int width, int height);
    void setSuperSamples(int n);
    void setFresnel();
    void useGlobalIllumination();
    void setGISamples(int n);
    void setGIBounces(int n);
    void setGIRatio(int n);

    void renderScene(std::string output);
    void pixelColorTest(int x, int y);
    void printRaysTest(int x, int y);

};
#endif
