#ifndef _BALAO_H
#define _BALAO_H
#include "lista.h"
#include "fila.h"
#include "texto.h"
#include "forma.h"

#include <stdbool.h>


typedef void *Balao;


/*
  Provê operações para implementar um Balão.

  Um Balão pode tirar fotos, armazená-las e fazer Dowload dessas 
  fotos. 
  O Balão deve ser criado por cria_balao. A seguir, operações
  como inserir foto no balão, copiar as listas de um balão em uma
  única lista, enviar fotos do balão para a base (download), copiar 
  todas as fotos de todas as filas de um balão, retornar o número de fotos 
  que tem em cada fila do balão, podem ser usados por esse contrato.
 */


/*
  Cria e retorna um TAD do tipo Balao. Recebe como parâmetro o identificador (id) 
  do Balao a ser criado e o ponteiro t para o tipo Texto, como por exemplo, se o balão
  for definido pelo texto "v_O_v". Com isso, a função aloca um espaço na memória para 
  o TAD Balao, guarda em registro seu id e o endereço de t, e cria 10 filas para 
  o Balao que foi criado. 
 */
Balao cria_balao(int id, Texto t);

/*
  Verifica se o ponteiro que aponta para um Balao (passado por parâmetro) é válido, ou seja,
  se ele existe. Caso seja NULL devolve uma mensagem de erro.
  Se existir, cria um ponteiro auxiliar para esse endereço de registros (struct) e acessa
  suas filas, criando uma lista (que será retornada pela função) e inserindo ela na fila 
  passada por parâmetro (int fila). Além disso, recebe o id do Balão e adiciona-o aos registros. 
 */
Lista* insere_foto_na_fila(int idBalao, int fila, Balao ponteiro);


/*
  Recebe por parâmetro o ponteiro para um Balao e o número 
  da fila que quer retornar as listas. Assim, a função cria 
  um ponteiro auxiliar para esse endereço de registros (struct) 
  e acessa a fila especificada na chamada da função. Então, 
  cria uma lista completa que receberá todos os elementos 
  de todas as listas existentes dentro dessa determinada fila 
  e devolve essa lista completa para ser utilizada. 
 */
Lista* copia_listas_fila(Balao ponteiro, int fila);


/*
  Recebe por parâmetro o ponteiro para um Balao, cria um ponteiro 
  auxiliar para esse endereço de registros (struct) e acessa
  a fila especificada na chamada da função. Então, cria uma 
  lista completa que receberá todos os elementos de todas as 
  listas existentes dentro do balão e devolve essa lista 
  completa para ser utilizada. 
 */
Lista* copia_listas_todas_filas(Balao ponteiro);


/*
  Recebe por parâmetro o ponteiro para um Balao, e o número 
  da fila que quer reportar o número de listas (ou fotos) que
  existem nessa fila. Cria um ponteiro auxiliar para esse endereço 
  de registros (struct) e acessa a fila especificada na chamada da função. 
  Então, retorna o número de listas existentes nessa fila. 
  Se o endereço para o Balao for NULL, retorna "Balão inválido"
  Se o número da fila passado for menor que zero ou maior que o limite máximo,
  retorna "Fila inválida".
 */
int getNumeroFotos(Balao ponteiro, int fila);

#endif