#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <Eigen/Dense>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color_t;

class Scene;

class Renderer {
private:
    Color_t *pixels;

    Color_t colorFromVector(Eigen::Vector3f &v);
    int width, height;

    Scene *scene;

public:
    Renderer(Scene *sc);

    void setImageSize(int width, int height);
    void renderScene(std::string output);

};
#endif
