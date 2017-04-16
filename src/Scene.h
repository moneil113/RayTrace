#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Light.h"
#include "Geometry.h"
#include "Renderer.h"

#include <vector>
#include <memory>

class Scene {
friend class Renderer;
private:
    Camera camera;
    std::vector<Light> lights;
    // Geometry is an abstract class, so we need pointers
    std::vector<std::shared_ptr<Geometry>> geometry;
    Renderer renderer;

    std::shared_ptr<Geometry> firstHit(int x, int y, floatOptional &t);

public:
    Scene();

    void print();
    void setCamera(Camera newCam);
    void setImageSize(int width, int height);
    void addLight(Light l);
    void addGeometry(std::shared_ptr<Geometry> g);

    void render(std::string outputFile);

    void pixelTest(int x, int y);
    void firstHitTest(int x, int y);
};

#endif
