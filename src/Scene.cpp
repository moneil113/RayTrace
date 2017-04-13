#include <iostream>
#include "Scene.h"
#include "Ray.h"

using namespace std;

Scene::Scene() {

}

void Scene::print() {
    cout << camera.to_string();

    cout << "\n---\n\n";
    cout << lights.size() << " light(s)\n";

    for (size_t i = 0; i < lights.size(); i++) {
        cout << "\n";
        cout << "Light[" << i << "]:\n";
        cout << lights.at(i).to_string();
    }

    cout << "\n---\n\n";
    cout << geometry.size() << " object(s)\n";

    for (size_t i = 0; i < geometry.size(); i++) {
        cout << "\n";
        cout << "Object: [" << i << "]:\n";
        cout << geometry.at(i)->to_string();
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
    cout << "Pixel: [" << x << " " << y << "] ";
    cout << "Ray: " << camera.rayToPixel(x, y).to_string() << '\n';
}

void Scene::firstHitTest(int x, int y) {
    pixelTest(x, y);

    floatOptional t = {false, INFINITY}, temp;
    shared_ptr<Geometry> objectHit;

    for (int i = 0; i < geometry.size(); i++) {
        Ray r = camera.rayToPixel(x, y);
        temp = geometry[i]->intersect(r);

        if (temp.valid && temp.value < t.value) {
            t = temp;
            objectHit = geometry[i];
        }
    }

    if (t.valid) {
        cout << "T = " << t.value << "\n";
        cout << "Object Type: " << objectHit->type() << '\n';
        Eigen::Vector3f color = objectHit->color() * 255;
        printf("Color: (%d, %d, %d)\n", (int) color.x(), (int) color.y(), (int) color.z());
    }
    else {
        cout << "No Hit" << '\n';
    }
}
