#ifndef _RETANGULO_H
#define _RETANGULO_H

#include <stdbool.h>

/*
  Provê operações para implementar um Retângulo.

  Um Retângulo é um elemento que pode representar diversas 
  estruturas dentro de um determinado contexto. Por exemplo, 
  se meu cenário de implementação é uma guerra, posso criar um 
  retângulo para representar uma estrutura de quartel general, um 
  hospital, um centro de comandos, etc.  
  O Retângulo deve ser criado por criaRetangulo. A seguir, operações
  para movimentá-lo e reportar seus parâmetros podem ser usadas 
  por esse contrato.
 */

typedef void* Retangulo;


/*
  Cria e retorna um TAD do tipo Retangulo. Recebe como parâmetro a
  posição central do retângulo (x,y), qual deve ser o sua largura (larg),
  sua altura (alt), seu identificador (i), qual deve ser a cor da sua 
  borda (corb) e qual deve ser a cor do seu preenchimento (corp). 
  Com isso, a função aloca um espaço na memória para 
  o TAD Retangulo, guarda em registro seus parâmetros e retorna um ponteiro 
  para o tipo Retangulo criado.   
 */
Retangulo criaRetangulo(double x, double y, double larg, double alt, int i, char *corb, char *corp);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna a altura
  desse retangulo.  
 */
double getAltura(Retangulo rect);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna a largurra
  desse retangulo.  
 */
double getLargura(Retangulo rect);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna a cor de 
  borda desse retangulo.  
 */
char *getCorbRect(Retangulo rect);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna a cor de 
  preenchimento desse retangulo.  
 */
char *getCorpRect(Retangulo rect);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna a posição X 
  desse retângulo. 
 */
double getPosicXRect(Retangulo rect);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna a posição Y
  desse retângulo. 
 */
double getPosicYRect(Retangulo rect);



/*
  Recebe por parâmetro o ponteiro para um Retangulo, a posição
  atual dele (coordenadas xAntigo, yAntigo) e qual deve ser 
  a distancia no eixo x (dx) e a distância no eixo y (dy) que
  esse retangulo deve percorrer para se movimentar.
  A função soma as coordenadas passadas: 
  xAntigo + dx
  yAntigo + dy 
  E atualiza no registro desse retangulo suas novas coordenadas. 
 */
void moveRetangulo(Retangulo rect, double xAntigo, double yAntigo, double dx, double dy);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Retangulo rect) e retorna o identificador  
  desse retangulo. 
 */
int getIdentificadorRetangulo(Retangulo rect); 





















#endif