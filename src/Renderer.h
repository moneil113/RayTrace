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
class Geometry;

class Renderer {
private:
    Color_t *pixels;
    int width, height;
    int brdf;

    Scene *scene;

    Color_t colorFromVector(Eigen::Vector3f &v);
    Color_t calculateColor(Ray &r, float t, std::shared_ptr<Geometry> object);
    Color_t blinnPhongColor(Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p);
    Color_t cookTorranceColor(Ray &r, std::shared_ptr<Geometry> object, Eigen::Vector3f p);

    Eigen::Vector3f blinnPhongDiffuse(Eigen::Vector3f &n, Eigen::Vector3f &l,
        Eigen::Vector3f &kd, Eigen::Vector3f &lightColor);
    Eigen::Vector3f blinnPhongSpecular(Eigen::Vector3f &n, Eigen::Vector3f &h,
        Eigen::Vector3f &ks, float power, Eigen::Vector3f &lightColor);

public:
    Renderer(Scene *sc);

    void setBRDF(int type);
    void setImageSize(int width, int height);
    void renderScene(std::string output);
    void pixelColorTest(int x, int y);

};
#endif
