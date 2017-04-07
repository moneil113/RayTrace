#include <iostream>
#include "Parser.h"

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "usage: RayTrace <input file>" << '\n';
        exit(-1);
    }
    Parser *p = new Parser(argv[1]);

    std::shared_ptr<Scene> sc = p->parse();
    sc->print();

    return 0;
}
