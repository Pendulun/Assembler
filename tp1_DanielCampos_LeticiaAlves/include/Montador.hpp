#ifndef MONTADOR_H
#define MONTADOR_H

#include <iostream>
#include <fstream>

// TODO: structs, classes, e outras definições.
class Montador{
    private:
        /* data */
    public:
        Montador();
        ~Montador();

        void montar(std::ifstream& entrada);
};


#endif
