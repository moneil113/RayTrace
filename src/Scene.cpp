#include <iostream>
#include "Scene.h"
#include "Ray.h"

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

void Scene::setImageSize(int width, int height) {
    camera.setImageSize(width, height);
}

void Scene::addLight(Light l) {
    lights.push_back(l);
}

void Scene::addGeometry(std::shared_ptr<Geometry> g) {
    geometry.push_back(g);
}

void Scene::pixelTest(int x, int y) {
    std::cout << "in Scene::pixelTest()" << '\n';
    std::cout << "Pixel: [" << x << " " << y << "] ";
    std::cout << "Ray: " << camera.rayToPixel(x, y).to_string() << '\n';
}
