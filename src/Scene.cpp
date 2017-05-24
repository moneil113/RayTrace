#include <iostream>
#include "Scene.h"
#include "Ray.h"

#define EPSILON 0.001f

using namespace std;

Scene::Scene() :
    renderer(this)
{
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
        cout << "Object[" << i << "]:\n";
        cout << geometry.at(i)->to_string();
    }

}

void Scene::setCamera(Camera newCam) {
    camera = newCam;
}

void Scene::setImageSize(int width, int height) {
    camera.setImageSize(width, height);
    renderer.setImageSize(width, height);
}

void Scene::setBRDF(int type) {
    renderer.setBRDF(type);
}

void Scene::setSuperSamples(int n) {
    renderer.setSuperSamples(n);
}

void Scene::setFresnel() {
    renderer.setFresnel();
}

void Scene::addLight(Light l) {
    lights.push_back(l);
}

void Scene::addGeometry(std::shared_ptr<Geometry> g) {
    geometry.push_back(g);
}

std::shared_ptr<Geometry> Scene::firstHit(Ray r, floatOptional &t) {
    t = {false, INFINITY};
    floatOptional temp;
    shared_ptr<Geometry> objectHit;

    for (int i = 0; i < geometry.size(); i++) {
        temp = geometry[i]->intersect(r);

        if (temp.valid && temp.value > EPSILON && temp.value < t.value) {
            t = temp;
            objectHit = geometry[i];
        }
    }

    if (t.valid) {
        return objectHit;
    }
    else {
        return NULL;
    }
}

void Scene::render(std::string outputFile) {
    renderer.renderScene(outputFile);
}

void Scene::pixelTest(int x, int y) {
    cout << "Pixel: [" << x << ", " << y << "] ";
    cout << "Ray: " << camera.rayToPixel(x, y).to_string() << '\n';
}

void Scene::firstHitTest(int x, int y) {
    pixelTest(x, y);

    floatOptional t;

    Ray r = camera.rayToPixel(x, y);

    std::shared_ptr<Geometry> objectHit = firstHit(r, t);

    if (t.valid) {
        cout << setprecision(4);
        cout << "T = " << t.value << "\n";
        cout << "Object Type: " << objectHit->type() << '\n';
        Eigen::Vector3f color = objectHit->color();
        cout << "Color: ";
        cout << color.x() << " " << color.y() << " " << color.z() << "\n";
    }
    else {
        cout << "No Hit" << '\n';
    }
}

void Scene::pixelColorTest(int x, int y) {
    pixelTest(x, y);

    floatOptional t;

    Ray r = camera.rayToPixel(x, y);

    std::shared_ptr<Geometry> objectHit = firstHit(r, t);

    if (t.valid) {
        cout << setprecision(4);
        cout << "T = " << t.value << "\n";
        cout << "Object Type: " << objectHit->type() << '\n';
        renderer.pixelColorTest(x, y);
    }
    else {
        cout << "No Hit" << '\n';
    }
}

void Scene::printRaysTest(int x, int y) {
    renderer.printRaysTest(x, y);
}
