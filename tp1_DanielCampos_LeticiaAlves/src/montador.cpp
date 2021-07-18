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
    instrucao.erase(inicio, fim);

    return operador;
}

std::string Montador::getOperando(std::string& instrucao){
    std::string operando = "";

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

    operando = instrucao.substr(inicio, fim);
    instrucao.erase(inicio, fim);

    return operando;
}

void Montador::passo1(){
    this->LC = 0;
    std::string instrucao;
    std::string label;
    std::string opcode;
    int operando1 = 0;
    int operando2 = 0;

    while(!this->entrada->eof()){
        std::getline(*this->entrada, instrucao);        

        instrucao = removeComentario(instrucao);
        label = getLabel(instrucao);
        opcode = getOperacao(instrucao);

        //PSEUDO
        if(opcode.compare("WORD")==0){
            operando1 = std::stoi(getOperando(instrucao));
            this->tabelaDeSimbolos.insert(std::pair<std::string, int>(label, operando1));
        }
        else if(opcode.compare("END")==0){
            passo2();
        }
        //INSTRUCOES TABELADAS
        else if(opcode.compare("HALT")==0){

        } 
        else if(opcode.compare("LOAD")==0){

        } 
        else if(opcode.compare("STORE")==0){

        } 
        else if(opcode.compare("READ")==0){

        } 
        else if(opcode.compare("WRITE")==0){

        } 
        else if(opcode.compare("COPY")==0){

        } 
        else if(opcode.compare("PUSH")==0){

        } 
        else if(opcode.compare("POP")==0){

        } 
        else if(opcode.compare("ADD")==0){

        } 
        else if(opcode.compare("SUB")==0){

        }  
        else if(opcode.compare("MUL")==0){

        } 
        else if(opcode.compare("DIV")==0){

        } 
        else if(opcode.compare("MOD")==0){

        } 
        else if(opcode.compare("AND")==0){

        } 
        else if(opcode.compare("OR")==0){

        } 
        else if(opcode.compare("NOT")==0){

        } 
        else if(opcode.compare("JUMP")==0){

        } 
        else if(opcode.compare("JZ")==0){

        } 
        else if(opcode.compare("JN")==0){

        } 
        else if(opcode.compare("CALL")==0){

        }  
        else if(opcode.compare("RET")==0){

        }    
    }   
}
void Montador::passo2(){

}