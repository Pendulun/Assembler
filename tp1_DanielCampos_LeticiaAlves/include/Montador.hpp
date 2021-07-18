#ifndef MONTADOR_H
#define MONTADOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

// TODO: structs, classes, e outras definições.
class Montador{
    private:
        std::ifstream* entrada;
        std::ofstream* saida;
        unsigned int LC;
        std::map<std::string, int> tabelaDeSimbolos;

        std::string removeComentario(std::string instrucao);
        std::string getLabel(std::string& instrucao);
        std::string getOperacao(std::string& instrucao);
        std::string getOperando(std::string& instrucao);

        void passo1();
        void passo2();
    public:
        Montador(std::ifstream& entrada, std::ofstream& saida);
        ~Montador();

        void montar();
};


#endif
