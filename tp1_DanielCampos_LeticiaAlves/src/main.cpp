#include "Montador.hpp"

int main(int argc, char *argv[]) {

    // TODO: executar o montador...
    std::ifstream entrada = std::ifstream();

    if (argc > 1){
        entrada.open(argv[1]);

        if (entrada.is_open()){

            Montador montador;      
            montador.montar(entrada);

            entrada.close();
        }
        else{
        std::cout<<"Nao foi possivel abrir o arquivo.\n";
        }
    }
    else{
        std::cout<<"Arquivo nao informado.\n";
        exit(1);
    }
   
    return 0;
}
