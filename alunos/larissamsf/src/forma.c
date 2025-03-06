#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forma.h"



typedef struct  
{
    void* obj;  
    char tipo[5];
}StForma;



Forma guarda_formato(void* objeto, char* formato)
{

    //printf("********ANTEEEEEEEEEES: Endereço da forma: %p forma: %s\n\n", posicao, formato);
    StForma *form = (StForma *) malloc(sizeof(StForma));
    form->obj = objeto;
    strcpy(form->tipo, formato); 
    //printf("********DEEPOOOOOOOOOOIS Endereço da forma: %p Tipo: %s\n\n", form->endereco, form->tipo);
    return form; 
}

char* getFormato(Forma forma)
{
    StForma *form = (StForma*)forma;
    return form->tipo; 
}

void* getObjeto(Forma forma)
{
    StForma *form = (StForma*)forma;
    return form->obj; 
}



int identificar_forma(Forma forma) 
{
    StForma *form = (StForma*)forma;

    if (strcmp(form->tipo, "c") == 0)
    {
        return 1;
    }else if (strcmp(form->tipo, "r") == 0)
    {   
        return 2;
    }else if (strcmp(form->tipo, "l") == 0)
    {
        return 3;
    }else if (strcmp(form->tipo, "t") == 0)
    {
        return 4;
    }else 
    {
        printf("Formato não encontrado!\n");
    }
}