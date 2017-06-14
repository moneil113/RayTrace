#include <iostream>
#include <fstream>

#include <Eigen/Dense>

#include "../Scene/Scene.h"

class Parser {
private:
    std::ifstream in;

    // Parse scene objects
    void parseCamera(std::string line);
    void parseLight_source(std::string line);
    void parseAreaLight(std::string line, Light &l);

    // Parse geometry objects
    void parseSphere(std::string line);
    void parsePlane(std::string line);
    void parseBox(std::string line);
    void parseCone(std::string line);
    void parseTriangle(std::string line);
    // Convenience functions
    void parseProperties(std::shared_ptr<Geometry> object);
    Eigen::Vector4f parsePigment(std::string line);
    void parseFinish(std::shared_ptr<Geometry> object, std::string line);
    Eigen::Vector4f readVec4(std::string line);
    Eigen::Vector3f readVec3(std::string line);
    float readFloat(std::string line);

    std::shared_ptr<Scene> scene;

public:
    Parser (std::string);

    std::shared_ptr<Scene> parse();
};
