PROJ_NAME=ted
ALUNO=LarissaMSFonseca
LIBS=-lm
OBJETOS=lista.o retangulo.o circulo.o main.o arq_svg.o linha.o texto.o fila.o balao.o forma.o bomba.o caca.o
# compilador
CC=gcc
# Flags

CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration

LDFLAGS=-O0

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME)  $(LDFLAGS) $(OBJETOS) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

#
# COLOCAR DEPENDENCIAS DE CADA MODULO
#
# Exemplo: suponha que o arquivo a.c possua os seguintes includes:
#
# #include "a.h"
# #include "b.h"
# #include "c.h"
#
# a.o: a.h b.h c.h a.c
#

lista.o: lista.h lista.c
retangulo.o: retangulo.h retangulo.c
circulo.o: circulo.h circulo.c
main.o: retangulo.h circulo.h lista.h arq_svg.h linha.h texto.h fila.h balao.h forma.h bomba.h caca.h main.c
arq_svg.o: arq_svg.h lista.h retangulo.h circulo.h linha.h arq_svg.c 
linha.o: linha.h linha.c
texto.o: texto.h texto.c
fila.o: fila.h fila.c
balao.o: balao.h lista.h fila.h texto.h forma.h balao.c
forma.o: forma.h arq_svg.h forma.c
bomba.o: bomba.h arq_svg.h lista.h circulo.h retangulo.h linha.h texto.h bomba.c
caca.o: caca.h caca.c


# EMPACOTAR PARA ENTREGA
#
# - se necessario: sudo apt install zip



pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega

	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-
	ME.txt .entrega
