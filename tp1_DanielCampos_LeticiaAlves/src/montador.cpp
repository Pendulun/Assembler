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

//Devo tratar também o caso de coisas opcionais como comentários
std::string Montador::removeComentario(std::string instrucao){
    std::string d_comentario = ";";
    std::string restante = instrucao;

    size_t pos = 0;

    pos = instrucao.find(d_comentario);

    while (pos != std::string::npos) {
        restante = instrucao.substr(0, pos);
        //std::cout<<nao_comentario<<std::endl;
        instrucao.erase(pos, instrucao.length()-pos);

        pos = instrucao.find(d_comentario);
    }

    return restante;
}

std::string Montador::getLabel(std::string& instrucao){
    std::string d_label = ":";
    std::string label = "";

    size_t pos = 0;

    pos = instrucao.find(d_label);

    while (pos != std::string::npos) {
        label = instrucao.substr(0, pos);
        //std::cout<<nao_comentario<<std::endl;
        instrucao.erase(0, pos+1);

        pos = instrucao.find(d_label);
    }

    return label;
}

//Supondo que haja pelo menos 1 espaço entre cada coisa
std::string Montador::getOperacao(std::string& instrucao){
    std::string operador = "";

    int inicio = -1;
    int fim = -1;

    for (unsigned int i=0; i<instrucao.length(); i++){
        if (instrucao[i] != ' '){
            inicio = i;

            break;    
        }
    }

    for (unsigned int i=inicio; i<instrucao.length(); i++){
        if (i == instrucao.length()-1){
            fim = i;
            break;
        }
        if (instrucao[i+1] == ' '){
            fim = i;
            break;    
        }
    }

    operador = instrucao.substr(inicio, fim);

    return operador;
}

void Montador::passo1(){
    this->LC = 0;
    std::string instrucao;
    std::string label;
    std::string opcode;

    while(!this->entrada->eof()){
        std::getline(*this->entrada, instrucao);        

        instrucao = removeComentario(instrucao);
        label = getLabel(instrucao);
        opcode = getOperacao(instrucao);

    }   
}
void Montador::passo2(){

}