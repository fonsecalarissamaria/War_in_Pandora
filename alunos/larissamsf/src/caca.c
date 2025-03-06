#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caca.h"




typedef struct
{
    int i;  // identificador 
    int numeroDisparos;  
    int vetorIdentificadores[300];
    int indice; 
} StCaca;


typedef struct StNode Node;



Caca cria_caca(int id, Texto t)
{
    StCaca *caca = (StCaca *) malloc(sizeof(StCaca));
    caca->i = id; 
    caca->numeroDisparos = 0; 
    
    //printf("Caça de id: %d criado!\n",id);
    return caca; 
}

void aumenta_numero_disparo(Caca c)
{
    StCaca *caca = c; 
    caca->numeroDisparos += 1; 
}


int getNumeroDisparos(Caca c)
{
    StCaca *caca = c; 
    return caca->numeroDisparos;
}

int getIdentificadorCaca(Caca c)
{
    StCaca *caca = c; 
    return caca->i;
}

void guardaId(Caca c, int id)
{
    StCaca *caca = c; 
    int i = caca->indice;
    caca->vetorIdentificadores[i] = id;
    caca->indice += 1;  
}


int* retornaIds(Caca c)
{
    StCaca *caca = c; 
    int tamanho = caca->indice;
    int* ids = malloc(sizeof(int) * tamanho);
    for (int i = 0; i < tamanho; i++) 
    {
        ids[i] = caca->vetorIdentificadores[i];
    }
    return ids;
}

int getTamanhoVetor(Caca c)
{
    StCaca *caca = c; 
    return caca->indice;
}