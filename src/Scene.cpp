#include <iostream>
#include "Scene.h"

Scene::Scene() {

}

void Scene::print() {
    std::cout << camera.to_string();

    std::cout << "\n-------------------------------------\n\n";

    for (size_t i = 0; i < lights.size(); i++) {
        std::cout << "[" << i << "]" << lights.at(i).to_string();
    }

    std::cout << "\n-------------------------------------\n\n";

    for (size_t i = 0; i < geometry.size(); i++) {
        std::cout << "[" << i << "] " << geometry.at(i)->to_string();
    }

}

void Scene::setCamera(Camera newCam) {
    camera = newCam;
}

void Scene::addLight(Light l) {
    lights.push_back(l);
}

void Scene::addGeometry(std::shared_ptr<Geometry> g) {
    geometry.push_back(g);
}
