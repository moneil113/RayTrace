#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <Eigen/Dense>

struct Color_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color_t operator+=(const Color_t &rhs)  {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
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

public:
    Renderer(Scene *sc);

    void setBRDF(int type);
    void setImageSize(int width, int height);
    void renderScene(std::string output);

};
#endif
