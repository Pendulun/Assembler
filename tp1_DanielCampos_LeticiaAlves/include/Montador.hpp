#ifndef MONTADOR_H
#define MONTADOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

// TODO: structs, classes, e outras definições.
class Montador{
    private:
        std::ifstream* entrada;
        std::ofstream* saida;
        
        unsigned int LC;
        unsigned int tamanhoPrograma;
        unsigned int endCarregamento;
        unsigned int posAP;
        unsigned int entryPoint;
        
        std::map<std::string, int> tabelaDeSimbolos;

        std::string removeComentario(std::string instrucao);
        std::string getLabel(std::string& instrucao);
        std::string getOperacao(std::string& instrucao);
        std::string getOperando(std::string& instrucao);
        void inserirNaTabelaDeSimbolosSeLabelNaoVazio(std::string label, int valor);

        int getRegistrador(std::string registrador);

        void imprimeNaTelaMensagem(const std::string,const std::string);

        void passo1();
        void passo2();
        void resetaLeituraArquivoEntrada();

        void defineInformacoesArquivoSaida();
        void escreveCabecalhoArquivoSaida();
        void escreveInformacoesArquivoSaida();
        void escreveInstrucaoNoArquivoSaida(const std::string, const  std::list<std::string>);

    public:
        Montador(std::ifstream& entrada, std::ofstream& saida);
        ~Montador();

        void montar();
};


#endif
