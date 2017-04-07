#include <stdio.h>
#include <iostream>
#include <fstream>

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
            parseCamera();
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
            std::cout << "not supported: " << line << '\n';
        }
    }

    return scene;
}

void Parser::parseCamera() {
    Camera cam;
    string line;
    bool done = false;

    while (getline(in, line) && !done) {
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
        else {
            cerr << "bad property: " << line << "\n";
        }
    }

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
    Sphere *s = new Sphere(); // geometry vector is abstract, so we need pointer
    s->center = readVec3(l);
    int start = l.find(">");
    s->radius = readFloat(l.substr(start));

    string line;
    bool done = false;

    while (getline(in, line) && !done) {
        done = parseProperties(s, line);
    }

    scene->addGeometry(s);
}

void Parser::parsePlane(std::string l) {
    Plane *p = new Plane();
    p->normal = readVec3(l);
    int start = l.find(">");
    p->distance = readFloat(l.substr(start));

    string line;
    bool done = false;

    while (getline(in, line) && !done) {
        done = parseProperties(p, line);
    }

    scene->addGeometry(p);
}

void Parser::parseBox(std::string l) {
    Box *b = new Box();
    b->corner1 = readVec3(l);
    int start = l.find(">");
    b->corner2 = readVec3(l.substr(start));

    string line;
    bool done = false;

    while (getline(in, line) && !done) {
        done = parseProperties(b, line);
    }

    scene->addGeometry(b);
}

void Parser::parseCone(std::string l) {
    Cone *c = new Cone();
    c->base = readVec3(l);
    int start = l.find(">");
    c->baseRadius = readFloat(l.substr(start));
    c->cap = readVec3(l.substr(start));
    start = l.rfind(">");
    c->capRadius = readFloat(l.substr(start));

    string line;
    bool done = false;

    while (getline(in, line) && !done) {
        done = parseProperties(c, line);
    }

    scene->addGeometry(c);
}

void Parser::parseTriangle() {
    Triangle *t = new Triangle();
    std::string line;
    bool done = false;

    getline(in, line);
    t->corner1 = readVec3(line);
    getline(in, line);
    t->corner2 = readVec3(line);
    getline(in, line);
    t->corner3 = readVec3(line);

    while (getline(in, line) && !done) {
        done = parseProperties(t, line);
    }

    scene->addGeometry(t);
}

bool Parser::parseProperties(Geometry *object, std::string line) {
    bool done = false;

    if (line.find("pigment") != string::npos) {
        object->pigment = parsePigment(line);
    }
    else if (line.find("finish") != string::npos) {
        int start = line.find("ambient");
        object->ambient = readFloat(line.substr(start));
        start = line.find("diffuse");
        object->diffuse = readFloat(line.substr(start));
    }
    else if (line.find("translate") != string::npos) {
        object->addTransform(TRANSFORM_TRANSLATE, readVec3(line));
    }
    else if (line.find("rotate") != string::npos) {
        object->addTransform(TRANSFORM_ROTATE, readVec3(line));
    }
    else if (line.find("scale") != string::npos) {
        object->addTransform(TRANSFORM_SCALE, readVec3(line));
    }
    else if (line.find("}") != string::npos) {
        done = true;
    }
    else {
        std::cerr << "bad property: " << line << '\n';
    }

    return done;
}

Eigen::Vector3f Parser::parsePigment(std::string line) {
    int start = line.find("color");
    return readVec3(line.substr(start));
}

Eigen::Vector3f Parser::readVec3(std::string line) {
    float x, y, z;
    int start = line.find("<");
    const char *str = line.substr(start).c_str();
    sscanf(str, "<%f, %f, %f>", &x, &y, &z);

    return Vector3f(x, y, z);
}

float Parser::readFloat(std::string line) {
    float x;
    sscanf(line.c_str(), "%*s%f", &x);

    return x;
}
