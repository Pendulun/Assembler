###############################################################################
########################### Parametros de Projeto #############################
###############################################################################

# TODO: altere a extensão dos arquivos (c ou cpp)
EXT = cpp
# TODO: altere a lista de forma que contenha todos os arquivos .cpp
#       do seu projeto (sem a extensão), que deve estar no diretório 
#       src/
SRCS_MONTADOR = $(patsubst %,%.$(EXT),main montador)
OBJS_MONTADOR = $(patsubst %,build/%.o,$(basename $(SRCS_MONTADOR)))

###############################################################################
########################### Parametros de Ambiente ############################
###############################################################################

CC = gcc
CXX = g++
CCFLAGS = -Wall
CXXFLAGS = -Wall -g
CPPFLAGS = -Iinclude/

###############################################################################
################################ Executáveis ##################################
###############################################################################

all: bin/montador

ifeq ($(EXT), c)
bin/montador: $(OBJS_MONTADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CC) $(CCFLAGS) $(OBJS_MONTADOR) -o bin/montador
else
bin/montador: $(OBJS_MONTADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CXX) $(CXXFLAGS) $(OBJS_MONTADOR) -o bin/montador
endif

###############################################################################
################################## Objetos ####################################
###############################################################################

ifeq ($(EXT), c)
build/%.o: src/%.c
	@echo "- Compilando objeto \"$@\""
	@$(CC) $(CPPFLAGS) $(CCFLAGS) -c $< -o $@
else
build/%.o: src/%.cpp
	@echo "- Compilando objeto \"$@\""
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
endif

###############################################################################
################################# Auxiliares ##################################
###############################################################################

clean:
	rm -f bin/* $(OBJS_MONTADOR)
