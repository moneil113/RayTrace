#include <iostream>
#include <fstream>

#include <Eigen/Dense>

#include "Scene.h"

class Parser {
private:
    std::ifstream in;

    void parseComment();
    // Parse objects
    void parseCamera();
    void parseLight_source(std::string line);
    void parseSphere(std::string line);
    void parsePlane(std::string line);
    void parseBox(std::string line);
    void parseCone(std::string line);
    void parseTriangle();
    // Convenience functions
    bool parseProperties(std::shared_ptr<Geometry> object, std::string line);
    Eigen::Vector3f parsePigment(std::string line);
    Eigen::Vector3f readVec3(std::string line);
    float readFloat(std::string line);

    std::shared_ptr<Scene> scene;

public:
    Parser (std::string);

    std::shared_ptr<Scene> parse();
};
