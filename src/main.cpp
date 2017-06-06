#include <iostream>
#include <memory>
#include "Util/Parser.h"

void parseOptionalArgs(std::shared_ptr<Scene> sc, const char *argv[], int start, int len) {
    for (int i = start; i < len; i++) {
        if (strstr(argv[i], "-altbrdf")) {
            sc->setBRDF(1);
        }
        else if (strstr(argv[i], "-ss=")) {
            int n;
            sscanf(argv[i], "-ss=%d", &n);
            sc->setSuperSamples(n);
        }
        else if (strstr(argv[i], "-fresnel")) {
            sc->setFresnel();
        }
        else if (strstr(argv[i], "-sds")) {
            sc->useSDS();
        }
        else if (strstr(argv[i], "-gi")) {
            sc->useGlobalIllumination();
        }
        else {
            std::cout << "Unknown optional argument: " << argv[i] << '\n';
        }
    }
}

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
        if (argc < 7) {
            std::cout << "usage: raytrace pixelray <input file> <width> <height> <x> <y>" << '\n';
            exit(-1);
        }
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));

        sc->pixelTest(atoi(argv[5]), atoi(argv[6]));
    }
    else if (strstr(argv[1], "firsthit")) {
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));
        parseOptionalArgs(sc, argv, 7, argc);

        sc->firstHitTest(atoi(argv[5]), atoi(argv[6]));
    }
    else if (strstr(argv[1], "render")) {
        if (argc < 5) {
            std::cout << "usage: raytrace render <input file> <width> <height> [-altbrdf] [-ss=N] [-fresnel] [-sds] [-gi]" << '\n';
            exit(-1);
        }
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));
        parseOptionalArgs(sc, argv, 5, argc);

        sc->render("output.png");
    }
    else if (strstr(argv[1], "pixelcolor")) {
        if (argc < 7) {
            std::cout << "usage: raytrace pixelcolor <input file> <width> <height> <x> <y> [-altbrdf] [-ss=N] [-fresnel] [-sds] [-gi]" << '\n';
            exit(-1);
        }
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));
        parseOptionalArgs(sc, argv, 7, argc);

        sc->pixelColorTest(atoi(argv[5]), atoi(argv[6]));
    }
    else if (strstr(argv[1], "printrays")) {
        if (argc < 7) {
            std::cout << "usage: raytrace printrays <input file> <width> <height> <x> <y> [-altbrdf] [-ss=N] [-fresnel] [-sds] [-gi]" << '\n';
            exit(-1);
        }
        sc->setImageSize(atoi(argv[3]), atoi(argv[4]));
        parseOptionalArgs(sc, argv, 7, argc);

        sc->printRaysTest(atoi(argv[5]), atoi(argv[6]));
    }
    else {
        std::cout << "usage: RayTrace <mode> <input file>" << '\n';
        exit(-1);
    }

    return 0;
}
