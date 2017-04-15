#include <iostream>
#include "Parser.h"

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        std::cout << "usage: RayTrace <mode> <input file>" << '\n';
        exit(-1);
    }
    Parser *p = new Parser(argv[2]);
    std::shared_ptr<Scene> sc = p->parse();

    if (strstr(argv[1], "sceneinfo")) {
        sc->print();
    }
    else if (strstr(argv[1], "pixelray")) {
        if (argc != 7) {
            std::cout << "usage: RayTrace pixelray <input file> <width> <height> <x> <y>" << '\n';
            exit(-1);
        }
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));

        sc->pixelTest(atoi(argv[5]), atoi(argv[6]));
    }
    else if (strstr(argv[1], "firsthit")) {
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));

        sc->firstHitTest(atoi(argv[5]), atoi(argv[6]));
    }
    else if (strstr(argv[1], "render")) {
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));
        
        sc->render("output.png");
    }
    else {
        std::cout << "usage: RayTrace <mode> <input file>" << '\n';
        exit(-1);
    }

    return 0;
}
