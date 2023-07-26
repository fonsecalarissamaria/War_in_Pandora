#include "fila.h"
#include "lista.h"
#include <stdio.h> 
#include <stdlib.h> 


struct FilaInt
{
    int inicio;
    int fim;
    int tamanho; 
    Item *stack; 
}; 



Fila criar_fila(int N)
{
    struct FilaInt *p;  
    p = malloc(sizeof(struct FilaInt)); 

    p->stack = malloc(sizeof(Item)*N); 
    p->tamanho = N; 
    p->inicio = 0;
    p->fim = -1; 

    return p; 
}

 
void insere_elemento(Fila fila, Item elemento)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    int indice = (p->fim + 1) % (p->tamanho);
    p->stack[indice] = elemento;  
    p->fim = indice; 


}


Item retira_elemento(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;

    Item valor = p->stack[p->inicio]; 

    if(p->inicio == p->fim)
    {
        p->fim = -1;
        p->inicio = 0; 
    }else 
    {
         p->inicio = (p->inicio + 1) % (p->tamanho);
    }
   
    return valor; 
}




int is_empty(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    
    if(p->fim == -1)
    {
        return 1;  //Sim está vazia 
    }else 
    {
        return 0; 
    }
}


 
int is_full(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    /*int aux; 
    aux = count(fila); 

    if (aux == p->tamanho)
    {
        return 1; 
    }else
    {
        return 0;
    }*/
    if(p->fim == ((p->inicio - 1) % p->tamanho))
    {
        return 1; 
    }else 
    {
        return 0; 
    }
}



int count(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    int valor;

    if(p->fim > p->inicio)
    {
        valor = p->fim - p->inicio; 
    }else
    {
        valor = (p->fim - p->inicio) % p->tamanho;
    }
    return ((valor % p->tamanho) + 1);
}



 
int getInicio(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    return p->inicio; 
}


int getFim(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    return p->fim;
}


int getTamanho(Fila fila)
{
    struct FilaInt *p = (struct FilaInt *) fila;
    return p->tamanho; 
}

