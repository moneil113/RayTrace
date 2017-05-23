#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>

#include "Parser.h"
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Cone.h"
#include "Triangle.h"

using namespace std;
using namespace Eigen;

Parser::Parser(std::string file) {
    in.open(file);
    if (!in.is_open()) {
        cerr << "could not open " << file << '\n';
        exit(-1);
    }

    scene = make_shared<Scene>();
}

std::shared_ptr<Scene> Parser::parse() {
    string line;

    while (getline(in, line)) {
        if (line.length() == 0 || line.find("//") != string::npos) {
            // do nothing
        }
        else if (line.find("camera") != string::npos) {
            parseCamera(line);
        }
        else if (line.find("light_source") != string::npos) {
            parseLight_source(line);
        }
        else if (line.find("sphere") != string::npos) {
            parseSphere(line);
        }
        else if (line.find("plane") != string::npos) {
            parsePlane(line);
        }
        else if (line.find("box") != string::npos) {
            parseBox(line);
        }
        else if (line.find("cone") != string::npos) {
            parseCone(line);
        }
        else if (line.find("triangle") != string::npos) {
            parseTriangle();
        }
        else {
            cout << "not supported: " << line << '\n';
        }
    }

    return scene;
}

void Parser::parseCamera(std::string line) {
    Camera cam;
    // string line;
    bool done = false;

    do {
        if (line.find("location") != string::npos) {
            cam.location = readVec3(line);
        }
        else if (line.find("up") != string::npos) {
            cam.up = readVec3(line);
        }
        else if (line.find("right") != string::npos) {
            cam.right = readVec3(line);
        }
        else if (line.find("look_at") != string::npos) {
            cam.look_at = readVec3(line);
        }
        else if (line.find("}") != string::npos) {
            done = true;
        }
        else if (line.find("camera") != string::npos) {
            // do nothing
        }
        else {
            cerr << "bad property: " << line << "\n";
        }
    } while (getline(in, line) && !done);

    scene->setCamera(cam);
}

void Parser::parseLight_source(std::string line) {
    Light l;
    l.location = readVec3(line);
    int color = line.find("color");
    l.color = readVec3(line.substr(color));

    scene->addLight(l);
}

void Parser::parseSphere(std::string l) {
    shared_ptr<Sphere> s = make_shared<Sphere>();
    s->center = readVec3(l);
    int start = l.find(">");
    s->radius = readFloat(l.substr(start));

    parseProperties(s);

    scene->addGeometry(s);
}

void Parser::parsePlane(std::string l) {
    shared_ptr<Plane> p = make_shared<Plane>();
    p->normal = readVec3(l);
    int start = l.find(">");
    p->distance = readFloat(l.substr(start));

    parseProperties(p);

    scene->addGeometry(p);
}

void Parser::parseBox(std::string l) {
    shared_ptr<Box> b = make_shared<Box>();
    b->corner1 = readVec3(l);
    int start = l.find(">");
    b->corner2 = readVec3(l.substr(start));

    parseProperties(b);

    scene->addGeometry(b);
}

void Parser::parseCone(std::string l) {
    shared_ptr<Cone> c = make_shared<Cone>();
    c->base = readVec3(l);
    int start = l.find(">");
    c->baseRadius = readFloat(l.substr(start));
    c->cap = readVec3(l.substr(start));
    start = l.rfind(">");
    c->capRadius = readFloat(l.substr(start));

    parseProperties(c);

    scene->addGeometry(c);
}

void Parser::parseTriangle() {
    shared_ptr<Triangle> t = make_shared<Triangle>();
    string line;

    getline(in, line);
    t->v1 = readVec3(line);
    getline(in, line);
    t->v2 = readVec3(line);
    getline(in, line);
    t->v3 = readVec3(line);

    parseProperties(t);

    scene->addGeometry(t);
}

void Parser::parseProperties(std::shared_ptr<Geometry> object) {
    bool done = false;
    object->finish = {};
    string line;

    while (getline(in, line) && !done) {
        if (line.find("pigment") != string::npos) {
            Vector4f pigment = parsePigment(line);
            object->pigment = pigment.head<3>();
            object->finish.filter = pigment[3];
        }
        else if (line.find("finish") != string::npos) {
            parseFinish(object, line);
        }
        else if (line.find("translate") != string::npos) {
            object->translate(readVec3(line));
        }
        else if (line.find("rotate") != string::npos) {
            object->rotate(readVec3(line));
        }
        else if (line.find("scale") != string::npos) {
            object->scale(readVec3(line));
        }
        else if (line.find("}") != string::npos) {
            done = true;
        }
        else {
            cerr << "bad property: " << line << '\n';
        }
        if (line.find("}}") != string::npos) {
            done = true;
        }
    }

    object->finalizeTransform();
}

Eigen::Vector4f Parser::parsePigment(std::string line) {
    int start = line.find("rgbf");
    if (start != string::npos) {
        return readVec4(line.substr(start));
    }
    start = line.find("rgb");
    Vector4f ret;
    ret << readVec3(line.substr(start)), 0;
    return ret;
}

void Parser::parseFinish(std::shared_ptr<Geometry> object, std::string line) {
    int start = line.find("ambient");
    object->finish.ambient = readFloat(line.substr(start));

    start = line.find("diffuse");
    object->finish.diffuse = readFloat(line.substr(start));

    start = line.find("specular");
    if (start != string::npos) {
        object->finish.specular = readFloat(line.substr(start));
    }

    start = line.find("roughness");
    if (start != string::npos) {
        object->finish.roughness = readFloat(line.substr(start));
    }

    start = line.find("metallic");
    if (start != string::npos) {
        object->finish.metallic = readFloat(line.substr(start));
    }

    start = line.find("ior");
    if (start != string::npos) {
        object->finish.ior = readFloat(line.substr(start));
    }

    start = line.find("reflection");
    if (start != string::npos) {
        object->finish.reflection = readFloat(line.substr(start));
    }
}

Eigen::Vector4f Parser::readVec4(std::string line) {
    float x, y, z, f;
    int start = line.find("<");
    string sub = line.substr(start);
    const char *str = sub.c_str();
    sscanf(str, "<%f, %f, %f, %f>", &x, &y, &z, &f);

    return Vector4f(x, y, z, f);
}

Eigen::Vector3f Parser::readVec3(std::string line) {
    float x, y, z;
    int start = line.find("<");
    string sub = line.substr(start);
    const char *str = sub.c_str();
    sscanf(str, "<%f, %f, %f>", &x, &y, &z);

    return Vector3f(x, y, z);
}

float Parser::readFloat(std::string line) {
    float x;
    int start = line.find(",");

    if (start != string::npos) {
        string sub = line;
        sub = line.substr(start + 1);
        sscanf(sub.c_str(), " %f", &x);
    }
    else {
        sscanf(line.c_str(), "%*s%f", &x);
    }

    return x;
}
