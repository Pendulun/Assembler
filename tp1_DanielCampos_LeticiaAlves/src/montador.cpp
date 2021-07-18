#include "Montador.hpp"

// TODO: implementação do montador
Montador::Montador(){}

Montador::~Montador(){}

void Montador::montar(std::ifstream& entrada, std::ofstream& saida){
    std::cout<<"Entrou no montar!\n";
    saida<<"MV-EXE"<<std::endl;
}
