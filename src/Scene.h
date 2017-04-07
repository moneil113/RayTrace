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
    std::vector<Geometry *> geometry;

public:
    Scene();

    void print();
    void setCamera(Camera newCam);
    void addLight(Light l);
    void addGeometry(Geometry *g);
};

#endif
