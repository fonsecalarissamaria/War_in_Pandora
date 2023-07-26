#ifndef _BOMBA_H_
#define _BOMBA_H_
#include <stdbool.h>
#include <string.h>
#include "math.h"
#include "arq_svg.h"
#include "lista.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "stdio.h"

/*
  Provê operações para implementar uma Bomba.
  Uma bomba pode ser disparada por um Caça. A bomba pode 
  atingir diversos elementos, como por exemplo, um balão.
  Cada bomba pode ter uma capacidade de alcance definida por 
  A, B ou C, sendo A < B < C. 
 */



/*
  Calcula a distancia entre o par de coordenadas (x1,y1) até o par de
  coordenadas (x2, y2) usando a distancia Euclidiana, biblioteca "math.h"
  e retorna um double com o resultado dessa distância.  
 */
double distanciaEntrePontos(double x1, double y1, double x2, double y2);


/*
  Calcula a coordenada central de um Caça para auxiliar
  no cálculo da coordenada de onde a Bomba vai cair, 
  Leva em consideração os parâmetros passados na chamada 
  da função, são eles:
  *xInicial: Posição X do caça 
  *yInicial: Posição Y do caça
  Retorna a posição central do caça 
  Essa função é necessária pois a coordenada (x,y) inicial do 
  caça pode estar no canto do texto e não no meio dele.  
 */
struct coordenadas getCoordenadasCentroTexto(double xInicial, double yInicial);



/*
  Calcula a coordenada de onde a Bomba vai cair, levando em consideração os
  parâmetros passados na chamada da função, são eles:
  *posição (x,y): Posição central do caça
  *distancia: Distancia que a bomba deve cair 
  *angulo: ângulo corrente do caça (em graus)
  Retorna um struct de coordenadas com o (X,Y) de onde a bomba cairá. 
 */
struct coordenadas calculaCoordenadas(double x, double y, double distancia, double angulo);


#endif