#include "Montador.hpp"

Montador::Montador(std::ifstream& entrada, std::ofstream& saida){
    this->entrada = &entrada;
    this->saida = &saida;
    this->posAP=0;
    this->tamanhoPrograma=0;
    this->entryPoint=0;
    this->definiuEntryPoint = false;
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
    std::cout<<"Fim Assembler\n";
}

void Montador::escreveCabecalhoArquivoSaida(){
    (*this->saida)<<"MV-EXE";
    (*this->saida)<<std::endl;
    (*this->saida)<<std::endl;
}

void Montador::imprimirTabelaDeSimbolos(){
    for (std::map<std::string, unsigned int>::iterator it=this->tabelaDeSimbolos.begin(); it!=this->tabelaDeSimbolos.end(); ++it){
        std::cout << it->first << " => " << it->second << '\n';
    }
    std::cout<<std::endl;
}


void Montador::passo1(){
    this->LC = 0;
    std::string instrucao;
    std::string label;
    std::string opcode;

    while(!this->entrada->eof()){
        opcode="";
        std::getline(*this->entrada, instrucao);
        this->imprimeNaTelaMensagem("LINHA LIDA: ", instrucao);

        instrucao = removeComentario(instrucao);
        this->imprimeNaTelaMensagem("A instrucao sem comentarios eh ", instrucao);

        instrucao = this->removeEspacosETabs(instrucao);
        this->imprimeNaTelaMensagem("LINHA SEM MUITOS ESPACOS E TABS", instrucao);

        if(instrucao.compare("") != 0){
            label = getLabel(instrucao);
            this->imprimeNaTelaMensagem("O label eh ", label);

            opcode = getOperacao(instrucao);
            this->imprimeNaTelaMensagem("A operacao eh ", opcode);

            this->imprimeNaTelaMensagem("Valor LC inicial: ",std::to_string(this->LC));

            this->inserirNaTabelaDeSimbolosSeLabelNaoVazio(label,this->LC);
            //PSEUDO
            if(opcode.compare("WORD")==0){
                this->LC++;
            }
            else if(opcode.compare("END")==0){
                this->defineInformacoesArquivoSaida();
                this->escreveInformacoesArquivoSaida();
                std::cout<<"TABELA DE SIMBOLOS PASSO 1\n";
                this->imprimirTabelaDeSimbolos();
                passo2();
                break;
            }
            else{

                //INSTRUCOES TABELADAS
                this->verificaEDefineEntryPoint();

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
            this->imprimeNaTelaMensagem("Valor LC final: ",std::to_string(this->LC));
        }else{
            //this->LC++;
            std::cout<<"LINHA EH VAZIA!\n";
        }
        std::cout<<std::endl;
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
    std::cout<<"ESCREVENDO: "<<codigoOperacao<<" ";
    for(std::string operando : operandos){
        std::cout<<operando<<" ";
    }
    std::cout<<"\n";
    *this->saida<<codigoOperacao<<" ";
    for(std::string operando : operandos){
        *this->saida<<operando.append(" ");
    }
}


std::string Montador::removeEspacosETabs(std::string instrucao){
    const std::string TAB = "\t";
    const std::string TWOSPACE = "  ";
    const std::string SPACE = " ";

    size_t pos = 0;

    while(true){
        pos = instrucao.find(TAB);
        if(pos != std::string::npos){
            instrucao.erase(pos, TAB.length());
            instrucao.insert(pos, SPACE);
        }else{
            break;
        }
    }

    while(true){
        pos = instrucao.find(TWOSPACE);
        if(pos != std::string::npos){
            instrucao.erase(pos, TWOSPACE.length());
            instrucao.insert(pos, SPACE);
        }else{
            break;
        }
    }

    return instrucao;
}

//Devo tratar também o caso de coisas opcionais como comentários
std::string Montador::removeComentario(std::string instrucao){
    std::string d_comentario = ";";
    std::string restante = instrucao;
    if(restante.compare("") == 0){
        std::cout<<"A LINHA EH VAZIA\n";
    }else if(restante.compare("\n") == 0){
        std::cout<<"A LINHA E SO UM BARRA N\n";
    }

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
    if(instrucao.compare("") != 0){
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
    }

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

void Montador::inserirNaTabelaDeSimbolosSeLabelNaoVazio(std::string label, unsigned int valor){
    if(label.compare("") != 0){
        std::string msg = "Inserindo ";
        msg+=label;
        msg+=" na TS com valor: ";
        this->imprimeNaTelaMensagem(msg, std::to_string(valor));
        this->tabelaDeSimbolos.insert(std::pair<std::string, unsigned int>(label, valor));
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
    this->imprimeNaTelaMensagem("PASSO 2: ", "");
    this->LC = 0;
    std::list<std::string> operandos;
    std::string valorOperacao="";
    //Para cada instrução lida
    while(!this->entrada->eof()){
        std::getline(*this->entrada, instrucao);

        instrucao = removeComentario(instrucao);
        //this->imprimeNaTelaMensagem("A instrucao sem comentarios eh ", instrucao);

        instrucao = this->removeEspacosETabs(instrucao);
        //this->imprimeNaTelaMensagem("LINHA SEM MUITOS ESPACOS E TABS ", instrucao);
        this->imprimeNaTelaMensagem("Instrucao Lida: ",instrucao);

        if(instrucao.compare("") != 0){
            operandos.clear();
            valorOperacao="";       
            
            label = getLabel(instrucao);
            opcode = getOperacao(instrucao);

            //PSEUDO
            this->imprimeNaTelaMensagem("label:", label);
            this->imprimeNaTelaMensagem("opcode:", opcode);

            this->imprimeNaTelaMensagem("Valor LC inicial: ",std::to_string(this->LC));

            if(opcode.compare("WORD")==0){
                this->LC++;
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
                    this->LC += 3;
                    operandos.push_back(this->getValorOperandoMemoriaVerificandoTabelaSimbolos(instrucao));
                    
                } 
                else if(opcode.compare("STORE")==0){
                    valorOperacao="2";
                    operandos.push_back(std::to_string(getRegistrador(getOperando(instrucao))));
                    this->LC += 3;
                    operandos.push_back(this->getValorOperandoMemoriaVerificandoTabelaSimbolos(instrucao));
                    
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
                    this->LC += 2;
                    operandos.push_back(this->getValorOperandoMemoriaVerificandoTabelaSimbolos(instrucao));
                    
                } 
                else if(opcode.compare("JZ")==0){
                    valorOperacao="17";
                    this->LC += 2;
                    operandos.push_back(this->getValorOperandoMemoriaVerificandoTabelaSimbolos(instrucao));
                } 
                else if(opcode.compare("JN")==0){
                    valorOperacao="18";
                    this->LC += 2;
                    operandos.push_back(this->getValorOperandoMemoriaVerificandoTabelaSimbolos(instrucao));
                } 
                else if(opcode.compare("CALL")==0){
                    valorOperacao="19";
                    this->LC += 2;
                    operandos.push_back(this->getValorOperandoMemoriaVerificandoTabelaSimbolos(instrucao));
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
            this->imprimeNaTelaMensagem("Valor LC final: ",std::to_string(this->LC));
            std::cout<<"\n\n";
        }else{
            //this->LC++;
        }
    }
}

void Montador::verificaEDefineEntryPoint(){
    if(!this->definiuEntryPoint){
        this->entryPoint = this->LC;
        this->definiuEntryPoint=true;
        std::cout<<"Definiu EntryPoint para: "<<this->entryPoint<<std::endl;
    }
}

std::string Montador::getValorOperandoMemoriaVerificandoTabelaSimbolos(std::string instrucao){
    std::string valorASerImpresso = "";
    std::string operandoMemoria = this->getOperando(instrucao);

    std::map<std::string, unsigned int>::iterator it;
    it = this->tabelaDeSimbolos.find(operandoMemoria);

    if (it != this->tabelaDeSimbolos.end()){
        std::cout<<"Operando de Memoria esta na tabela de simbolos!\n";
        //Essa variável é necessária porcausa de algum erro na hora de realizar a 
        //subtração de um unsigned int menor do que outro unsigned int causando underflow
        int enderecoRelativo = it->second - (this->LC);
        valorASerImpresso = std::to_string(enderecoRelativo);
        std::cout<<"Endereco operando: "<<it->second;
        std::cout<<" LC operando memoria: "<<this->LC<<std::endl;
        std::cout<<"Valor a ser impresso calculado: "<<valorASerImpresso<<std::endl;
    }else{
        valorASerImpresso = operandoMemoria;
    }

    return valorASerImpresso;
}
 

void Montador::resetaLeituraArquivoEntrada(){
    this->entrada->clear();
    this->entrada->seekg(0);

}