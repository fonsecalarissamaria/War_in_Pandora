#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


 /**
   Provê operações para implementar uma Fila Circular.
   Uma fila é uma estrutura de dados que usa o método 
   de inserção: "primeiro a entrar, primeiro a sair". 
   A idéia fundamental da fila é que só é permitido 
   inserir um novo elemento no final da fila. Além disso,
   só é permitido retirar um elemento do início da mesma.
   Uma fila sem nenhum elemento é denominada fila vazia.
   
   O módulo provê operações básicas para fila circular, tais
   como criar, inserir elemento, verificar seu tamanho, se está 
   vazia, se está cheia, etc. 
*/


typedef void *Fila;

/** 
  Retorna uma fila vazia. A fila pode definir um limite maximo de 
  elementos armazenados (N). 
  N Não deve ser menor do que 0. 
*/
Fila criar_fila(int N);



/**  
  Insere o item elemento no final da Fila fila. O tamanho da
  fila é acrescido de 1 elemento. 
*/
void insere_elemento(Fila fila, Item elemento);



/** 
  Remove o primeiro elemento da Fila fila. 
  O tamanho da fila é diminuido de 1 elemento. 
*/
Item retira_elemento(Fila fila);


/** 
  Retorna 1 se a Fila fila estiver vazia
  ou 0 se não estiver vazia.  
*/ 
int is_empty(Fila fila);


/** 
  Retorna 1 se a Fila fila estiver cheia
  ou 0 se não estiver.  
*/  
int is_full(Fila fila);


/** Retorna o numero de elementos da fila. */ 
int count(Fila fila);

/** Retorna o incio da fila. */
int getInicio(Fila fila);


/** Retorna o fim da fila. */ 
int getFim(Fila fila);


/** Retorna Tamanho da fila. */
int getTamanho(Fila fila); 