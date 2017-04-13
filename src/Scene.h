#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Light.h"
#include "Geometry.h"

#include <vector>

class Scene {
private:
    Camera camera;
    std::vector<Light> lights;
    // Geometry is an abstract class, so we need pointers
    std::vector<std::shared_ptr<Geometry>> geometry;

public:
    Scene();

    void print();
    void setCamera(Camera newCam);
    void setImageSize(int width, int height);
    void addLight(Light l);
    void addGeometry(std::shared_ptr<Geometry> g);

    void pixelTest(int x, int y);
    void firstHitTest(int x, int y);
};

#endif
