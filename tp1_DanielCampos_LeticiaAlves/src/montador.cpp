#include "Montador.hpp"

// TODO: implementação do montador
Montador::Montador(std::ifstream& entrada, std::ofstream& saida){
    this->entrada = &entrada;
    this->saida = &saida;
}

Montador::~Montador(){
    this->entrada = nullptr;
    this->saida = nullptr;    
}

void Montador::montar(){
    std::cout<<"Entrou no montar!\n";
    *saida<<"MV-EXE"<<std::endl;

    this->passo1();
}

void Montador::passo1(){
    this->LC = 0;
    std::string instrucao;

    while(!this->entrada->eof()){
        std::getline(*this->entrada, instrucao);

        std::string d_comentario = ";";

        size_t pos = 0;
        std::string nao_comentario;

        pos = instrucao.find(d_comentario);

        while (pos != std::string::npos) {
            nao_comentario = instrucao.substr(0, pos);
            std::cout<<nao_comentario<<std::endl;
            instrucao.erase(pos, instrucao.length()-pos);

            pos = instrucao.find(d_comentario);
        }
    }   
}
void Montador::passo2(){

}