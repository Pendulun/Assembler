#include "Montador.hpp"

Montador::Montador(std::ifstream& entrada, std::ofstream& saida){
    this->entrada = &entrada;
    this->saida = &saida;
    this->posAP=0;
    this->tamanhoPrograma=0;
    this->entryPoint=0;
    this->endCarregamento=0;
}

Montador::~Montador(){
    this->entrada = nullptr;
    this->saida = nullptr;    
}

void Montador::montar(){
    //std::cout<<"Entrou no montar!\n";
    this->escreveCabecalhoArquivoSaida();

    this->passo1();
    this->escreveInformacoesArquivoSaida();
}

void Montador::escreveCabecalhoArquivoSaida(){
    *this->saida<<"MV-EXE";
    *saida<<std::endl;
    *saida<<std::endl;
}

void Montador::escreveInformacoesArquivoSaida(){
    *this->saida<<std::to_string(this->tamanhoPrograma).append(" ");
    *this->saida<<std::to_string(this->endCarregamento).append(" ");
    *this->saida<<std::to_string(this->posAP).append(" ");
    *this->saida<<std::to_string(this->entryPoint).append(" ");
}

void Montador::escreveInstrucaoNoArquivoSaida(const std::string codigoOperacao, const std::list<std::string> operandos){
    *this->saida<<codigoOperacao<<" ";
    for(std::string operando : operandos){
        *this->saida<<operando.append(" ");
    }
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

    operador = instrucao.substr(inicio, fim+1);
    instrucao.erase(inicio, fim+1);

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
    instrucao.erase(inicio, fim+1);

    return operando;
}

int Montador::getRegistrador(std::string registrador){
    return std::stoi(registrador.substr(1,1));
}

void Montador::inserirNaTabelaDeSimbolos(std::string label){
    if(label.compare("") != 0){
        this->tabelaDeSimbolos.insert(std::pair<std::string, int>(label, this->LC));
    }
}

void Montador::passo1(){
    this->LC = 0;
    std::string instrucao;
    std::string label;
    std::string opcode;
    int operando1 = 0;
    //int operando2 = 0;

    while(!this->entrada->eof()){
        std::getline(*this->entrada, instrucao);        

        instrucao = removeComentario(instrucao);
        std::cout<<"A instrucao sem comentarios eh "<<instrucao<<std::endl;

        label = getLabel(instrucao);
        std::cout<<"O label eh "<<label<<std::endl;

        opcode = getOperacao(instrucao);
        std::cout<<"A operacao eh "<<opcode<<std::endl;
        std::cout<<"\n";

        //PSEUDO
        if(opcode.compare("WORD")==0){
            operando1 = std::stoi(getOperando(instrucao));
            this->tabelaDeSimbolos.insert(std::pair<std::string, int>(label, operando1));
        }
        else if(opcode.compare("END")==0){

        /*for(auto elem : this->tabelaDeSimbolos){
            std::cout<<elem.first<<" "<<elem.second<<"\n";
        }*/

            passo2();
            break;
        }
        else{

            //INSTRUCOES TABELADAS
            this->inserirNaTabelaDeSimbolos(label);

            if(opcode.compare("HALT")==0){
                //Parada
                this->LC++;
            } 
            else if(opcode.compare("LOAD")==0){
                //Carrega Registrador 
                //operando1 = getRegistrador(getOperando(instrucao));
                //operando2 = std::stoi(getOperando(instrucao));
                this->LC += 3;
            } 
            else if(opcode.compare("STORE")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("READ")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("WRITE")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("COPY")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("PUSH")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("POP")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("ADD")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("SUB")==0){
                this->LC += 3;
            }  
            else if(opcode.compare("MUL")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("DIV")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("MOD")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("AND")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("OR")==0){
                this->LC += 3;
            } 
            else if(opcode.compare("NOT")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("JUMP")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("JZ")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("JN")==0){
                this->LC += 2;
            } 
            else if(opcode.compare("CALL")==0){
                this->LC += 2;
            }  
            else if(opcode.compare("RET")==0){
                this->LC++;
            }
            else{
                std::cout<<"Instrucao nao encontrada.\n";
            }

        }
    } 
}
void Montador::passo2(){
    this->resetaLeituraArquivoEntrada();
    //Para cada instrução lida
    //Pega o opcode
    //Pega os valores dos operandos
    //Pede para imprimir no arquivo a instrução
}

void Montador::resetaLeituraArquivoEntrada(){
    this->entrada->clear();
    this->entrada->seekg(0);

}