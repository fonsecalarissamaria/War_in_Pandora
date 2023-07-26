#ifndef _CACA_H
#define _CACA_H
#include "lista.h"
#include "fila.h"
#include "texto.h"
#include "forma.h"

#include <stdbool.h>


typedef void *Caca;


/*
  Provê operações para implementar um Caça.
  Um caça pode disparar bombas para atingir 
  e destruir elementos. Com isso, é necessário 
  ter um controle do número de tiros que ele deu 
  e dos identificadores dos elementos que foram 
  atingidos. 
  O caça deve ser criada por cria_caca. A seguir, operações
  como inserir +1 na contagem de tiros pode ser chamada pela
  função aumenta_numero_disparo.
 */


/*
  Cria e retorna um TAD do tipo Caca (Caça). Recebe como parâmetro o identificador (id) 
  do Caça a ser criado e o ponteiro t para o tipo Texto, como por exemplo, se o caça
  for definido pelo texto "|-T-|". Com isso, a função aloca um espaço na memória para 
  o TAD Caca, guarda em registro seu id e o endereço de t e já inicializa o 
  número de disparos com zero.  
 */
Caca cria_caca(int id, Texto t);

/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Caca c) e adiciona +1 no número de disparos 
  do Caça c.    
 */
void aumenta_numero_disparo(Caca c);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Caca c) e reporta o número de disparos 
  que já foram dados por esse caça. 
 */
int getNumeroDisparos(Caca c);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Caca c) e reporta o identificador 
  desse caça. 
 */
int getIdentificadorCaca(Caca c);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Caca c) e armazena o identificador 
  do elemento que foi atingido pelo caça. 
 */
void guardaId(Caca c, int id);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Caca c) e retorna todos os identificadores 
  de todos os elementos que esse caça atingiu. 
 */
int* retornaIds(Caca c);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Caca c) e retorna o tamanho do vetor 
  de identificadores de elementos atingidos, ou seja, quantos 
  elementos o caça atingiu.  
 */
int getTamanhoVetor(Caca c);

#endif