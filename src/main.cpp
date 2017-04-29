#include <iostream>
#include <memory>
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
        if (argc != 7 && argc != 8) {
            std::cout << "usage: raytrace pixelray <input file> <width> <height> <x> <y> [-altbrdf]" << '\n';
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
        if (argc < 5) {
            std::cout << "usage: raytrace render <input file> <width> <height> [-altbrdf]" << '\n';
            exit(-1);
        }
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));
        if (argc == 6 && strstr(argv[5], "-altbrdf")) {
            sc->setBRDF(1);
        }
        else {
            sc->setBRDF(0);
        }

        sc->render("output.png");
    }
    else if (strstr(argv[1], "pixelcolor")) {
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));

        sc->pixelColorTest(atoi(argv[5]), atoi(argv[6]));
    }
    else {
        std::cout << "usage: RayTrace <mode> <input file>" << '\n';
        exit(-1);
    }

    return 0;
}
