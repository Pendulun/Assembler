#include "Montador.hpp"

Montador::Montador(std::ifstream& entrada, std::ofstream& saida){
    this->entrada = &entrada;
    this->saida = &saida;
    this->posAP=0;
    this->tamanhoPrograma=0;
    this->entryPoint=0;
    this->endCarregamento=0;
    this->LC=0;
}

Montador::~Montador(){
    this->entrada = nullptr;
    this->saida = nullptr;    
}

void Montador::montar(){
    this->escreveCabecalhoArquivoSaida();
    this->passo1();
}

void Montador::escreveCabecalhoArquivoSaida(){
    *this->saida<<"MV-EXE";
    *saida<<std::endl;
    *saida<<std::endl;
}

void Montador::passo1(){
    this->LC = 0;
    std::string instrucao;
    std::string label;
    std::string opcode;

    while(!this->entrada->eof()){
        opcode="";
        std::getline(*this->entrada, instrucao);        

        instrucao = removeComentario(instrucao);
        this->imprimeNaTelaMensagem("A instrucao sem comentarios eh ", instrucao);

        label = getLabel(instrucao);
        this->imprimeNaTelaMensagem("O label eh ", label);

        opcode = getOperacao(instrucao);
        this->imprimeNaTelaMensagem("A operacao eh ", opcode);
        std::cout<<"\n";

        //PSEUDO
        if(opcode.compare("WORD")==0){
            this->inserirNaTabelaDeSimbolosSeLabelNaoVazio(label,this->LC);
            this->LC++;
        }
        else if(opcode.compare("END")==0){

        /*for(auto elem : this->tabelaDeSimbolos){
            std::cout<<elem.first<<" "<<elem.second<<"\n";
        }*/
            this->defineInformacoesArquivoSaida();
            this->escreveInformacoesArquivoSaida();
            passo2();
            break;
        }
        else{

            //INSTRUCOES TABELADAS
            this->inserirNaTabelaDeSimbolosSeLabelNaoVazio(label, this->LC);

            if(opcode.compare("HALT")==0){
                //Parada
                this->LC++;
            } 
            else if(opcode.compare("LOAD")==0){
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

void Montador::defineInformacoesArquivoSaida(){
    this->tamanhoPrograma = this->LC;
    this->endCarregamento=0;
    this->posAP = this->endCarregamento + this->tamanhoPrograma + 1000;

}

void Montador::escreveInformacoesArquivoSaida(){
    *this->saida<<std::to_string(this->tamanhoPrograma).append(" ");
    *this->saida<<std::to_string(this->endCarregamento).append(" ");
    *this->saida<<std::to_string(this->posAP).append(" ");
    *this->saida<<std::to_string(this->entryPoint);
    *this->saida<<std::endl;
    *this->saida<<std::endl;
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
        else if (instrucao[i+1] == ' '){
            fim = i;
            break;    
        }
    }

    operador = instrucao.substr(inicio, fim+1-inicio);
    instrucao.erase(inicio, fim+1-inicio);

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

    operando = instrucao.substr(inicio, fim+1-inicio);
    instrucao.erase(inicio, fim+1-inicio);

    return operando;
}

int Montador::getRegistrador(std::string registrador){
    return std::stoi(registrador.substr(1,1));
}

void Montador::inserirNaTabelaDeSimbolosSeLabelNaoVazio(std::string label, int valor){
    if(label.compare("") != 0){
        this->tabelaDeSimbolos.insert(std::pair<std::string, int>(label, valor));
    }
}

void Montador::imprimeNaTelaMensagem(const std::string mensagem, const std::string instrucao){
    std::cout<<mensagem<<"."<<instrucao<<"."<<std::endl;

}

void Montador::passo2(){
    this->resetaLeituraArquivoEntrada();
    std::string instrucao;
    std::string label;
    std::string opcode;
    int operando1 = 0;
    this->imprimeNaTelaMensagem("PASSO 2: ", "");
    std::list<std::string> operandos;
    std::string valorOperacao="";
    //Para cada instrução lida
    while(!this->entrada->eof()){
        std::getline(*this->entrada, instrucao);
        operandos.clear();
        valorOperacao="";        

        instrucao = removeComentario(instrucao);
        this->imprimeNaTelaMensagem("A instrucao sem comentarios eh ", instrucao);
        
        label = getLabel(instrucao);
        opcode = getOperacao(instrucao);

        //PSEUDO
        std::cout<<"opcode: ."<<opcode<<"."<<std::endl;
        if(opcode.compare("WORD")==0){
            valorOperacao=getOperando(instrucao);
            std::cout<<"Valor WORD: "<<valorOperacao<<std::endl;
        }
        else if(opcode.compare("END")==0){
            break;
        }
        else{

            //INSTRUCOES TABELADAS
            if(opcode.compare("HALT")==0){
                valorOperacao="0";
                //Parada
                this->LC++;
            } 
            else if(opcode.compare("LOAD")==0){ 
                valorOperacao="1";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(getOperando(instrucao));
                this->LC += 3;
            } 
            else if(opcode.compare("STORE")==0){
                valorOperacao="2";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(getOperando(instrucao));
                this->LC += 3;
            } 
            else if(opcode.compare("READ")==0){
                valorOperacao="3";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 2;
            } 
            else if(opcode.compare("WRITE")==0){
                valorOperacao="4";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 2;
            } 
            else if(opcode.compare("COPY")==0){
                valorOperacao="5";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(getOperando(instrucao));
                this->LC += 3;
            } 
            else if(opcode.compare("PUSH")==0){
                valorOperacao="6";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 2;
            } 
            else if(opcode.compare("POP")==0){
                valorOperacao="7";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 2;
            } 
            else if(opcode.compare("ADD")==0){
                valorOperacao="8";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            } 
            else if(opcode.compare("SUB")==0){
                valorOperacao="9";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            }  
            else if(opcode.compare("MUL")==0){
                valorOperacao="10";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            } 
            else if(opcode.compare("DIV")==0){
                valorOperacao="11";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            } 
            else if(opcode.compare("MOD")==0){
                valorOperacao="12";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            } 
            else if(opcode.compare("AND")==0){
                valorOperacao="13";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            } 
            else if(opcode.compare("OR")==0){
                valorOperacao="14";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 3;
            } 
            else if(opcode.compare("NOT")==0){
                valorOperacao="15";
                operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                this->LC += 2;
            } 
            else if(opcode.compare("JUMP")==0){
                valorOperacao="16";
                operandos.push_back(getOperando(instrucao));
                this->LC += 2;
            } 
            else if(opcode.compare("JZ")==0){
                valorOperacao="17";
                operandos.push_back(getOperando(instrucao));
                this->LC += 2;
            } 
            else if(opcode.compare("JN")==0){
                valorOperacao="18";
                operandos.push_back(getOperando(instrucao));
                this->LC += 2;
            } 
            else if(opcode.compare("CALL")==0){
                valorOperacao="19";
                operandos.push_back(getOperando(instrucao));
                this->LC += 2;
            }  
            else if(opcode.compare("RET")==0){
                valorOperacao="20";
                this->LC++;
            }
            else{
                std::cout<<"Instrucao nao encontrada.\n";
            }

        }

        this->escreveInstrucaoNoArquivoSaida(valorOperacao, operandos);
    }
    
}

void Montador::resetaLeituraArquivoEntrada(){
    this->entrada->clear();
    this->entrada->seekg(0);

}