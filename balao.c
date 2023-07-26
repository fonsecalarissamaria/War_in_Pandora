#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "balao.h"

#define NUM_FILAS 10
#define NUM_LISTAS 16
#define NUM_ELEMENTOS 500


typedef struct 
{
    float Xmin;
    float Ymin;
    float Xmax;
    float Ymax;
} Coordenadas;


typedef struct
{
    int i;  // identificador do linha 
    Balao ponteir; 
    Texto enderecoBalao; 
    Fila queue[NUM_FILAS]; 
    Lista lists[NUM_FILAS][NUM_LISTAS]; // lista é um array bidimensional 
    int numeroFotoPorFila[NUM_FILAS]; // número da última foto adicionada em cada fila 
    Lista *ponteiroListaFoto; 

} StBalao;



typedef struct StNode Node;

Balao cria_balao(int id, Texto t)
{
    StBalao *balao = (StBalao *) malloc(sizeof(StBalao));
    balao->i = id;  
    balao->ponteir = balao;
    balao->enderecoBalao = t; 

    for(int i=0; i<NUM_FILAS; i++) 
    {
        balao->queue[i] = criar_fila(NUM_LISTAS);
    }

    Fila queue = balao->queue[5];

    /*for(int i=0; i<NUM_FILAS; i++) 
    {
        printf("Fila: %d Lista: %d Endereço list: %p\n", i, j, balao->lists[j]);   
    }*/

    
    //printf("Balão de id: %d e endereço: %p\n",id, balao->ponteir);
    return balao; 
}




Lista* insere_foto_na_fila(int idBalao, int fila, Balao ponteiro)
{
    //int indice = 1; 
    if (ponteiro == NULL) // Verifica se o balão existe 
    { 
        printf("Balão com id %d não encontrado!\n", idBalao);
        //return;
    }

    StBalao* balao = ponteiro;
    balao->i = idBalao; 

    Fila queue = balao->queue[fila];

    //printf("//////////Fila: %d, Lista: %d Endereço list: %p\n\n", fila, lista, balao->lists[lista]);

    // Verifica se a fila existe
    if (queue == NULL) 
    { 
        printf("Fila inválida!\n");
        //return;
    }else 
    {
        //printf("Fila ou lista válida!!!!\n");
    }

     Lista* l = createLst(NUM_ELEMENTOS); 
    insere_elemento(queue, l);

    int numeroDaFoto = 1;
    if (balao->numeroFotoPorFila[fila] > 0) {
        numeroDaFoto = balao->numeroFotoPorFila[fila] + 1;
    }
    balao->numeroFotoPorFila[fila] = numeroDaFoto;
    balao->lists[fila][numeroDaFoto - 1] = l;

    //printf("Endereço da lista %d da Fila: %d %p\n", numeroDaFoto, fila, l);

    return l;
}



Lista* copia_listas_fila(Balao ponteiro, int fila) 
{
    StBalao* balao = ponteiro;
    Lista* lista_completa = createLst(1000);
    Lista* lista_atual;
    

    for (int i = 0; i < balao->numeroFotoPorFila[fila]; i++) 
    {
        lista_atual = balao->lists[fila][i];
        Node* node_atual = getFirstLst(lista_atual);

        if (lista_atual != NULL) 
        {
            int num5 = lengthLst(lista_atual); 
            //printf("\nNúmero de elementos na lista atual: %d\n\n", num5);

            Node* pCurr = getFirstLst(lista_atual);
            while(pCurr != NULL)
            {
                Forma geometrica = getLst(lista_atual, pCurr);
                insertLst(lista_completa, geometrica);
                Node* pNext = getNextLst(lista_atual, pCurr);
                removeLst(lista_atual, pCurr);
                pCurr = pNext;
            }
        }
    }

    return lista_completa;
}


Lista* copia_listas_todas_filas(Balao ponteiro) 
{
    StBalao* balao = ponteiro;
    Lista* lista_completa = createLst(15000);
    Lista* lista_atual;

    for (int i = 0; i < 10; i++) //itera pelas 10 filas
    {
        
        for (int j = 0; j < balao->numeroFotoPorFila[i]; j++) //itera pelas 15 listas de cada fila
        {
            lista_atual = balao->lists[i][j];
            if (lista_atual != NULL) 
            {
                int num_elem = lengthLst(lista_atual); 
                //printf("\nNúmero de elementos na lista atual: %d\n\n", num_elem);

                Node* pCurr = getFirstLst(lista_atual);
                while(pCurr != NULL)
                {
                    Forma geometrica = getLst(lista_atual, pCurr);
                    insertLst(lista_completa, geometrica);
                    Node* pNext = getNextLst(lista_atual, pCurr);
                    removeLst(lista_atual, pCurr);
                    pCurr = pNext;
                }
            }
        }
    }

    return lista_completa;
}


int getNumeroFotos(Balao ponteiro, int fila) 
{
    StBalao* balao = ponteiro;

    if (balao == NULL) {
        printf("Balão inválido!\n");
        return -1;
    }
    if (fila < 0 || fila >= NUM_FILAS) {
        printf("Fila inválida!\n");
        return -1;
    }
    return balao->numeroFotoPorFila[fila];
}


/*void getCoordenadasLista(Balao ponteiro, int fila, int lista, float Xmin, float Ymin, float Xmax, float Ymax) 
{
    StBalao* balao = ponteiro;

    // Cria uma nova estrutura Coordenadas para armazenar as coordenadas
    Coordenadas* coord = (Coordenadas*) malloc(sizeof(Coordenadas));
    coord->Xmin = Xmin;
    coord->Ymin = Ymin;
    coord->Xmax = Xmax;
    coord->Ymax = Ymax;

    // Cria um novo nó para a lista de fotografias do balão
    Node* novo_no = coord;

    // Insere o novo nó na lista de fotografias do balão
    Lista* l = &(balao->lists[fila][lista]);
    Posic proximo = getNextLst(l, novo_no);
    proximo = balao->ponteiroListaFoto;
    balao->ponteiroListaFoto = novo_no;
}*/






Lista* envia_fotos_base(int idBalao, Balao ponteiro, int fila, char *sufixo)
{
    if (ponteiro == NULL) // Verifica se o balão existe 
    { 
        printf("Balão não encontrado!\n");
        return NULL;
    } else
    { 
        //printf("balãaãããão encontradooo!");
    }

    if (fila < 0 || fila >= NUM_FILAS) // Verifica se o índice da fila é válido
    {
        printf("Índice de fila inválido!\n");
        return NULL;
    }

    StBalao* balao = ponteiro;
    balao->i = idBalao;
    

    Fila queue = balao->queue[fila];
    Lista* lista_completa = createLst(1000);

    if (lista_completa == NULL) // Verifica se a lista foi criada com sucesso
    {
        printf("Erro ao criar lista!\n");
        return NULL;
    }

    int indice = balao->numeroFotoPorFila[fila]; 
    
    for(int i=0; i<indice; i++) // Ajuste do loop para começar em 0
    {
        Lista lst = balao->lists[i];
        if(lst != NULL)
        {
            //printf("Achei a lista %d da fila %d!!!! O endereço dela é: %p\n",i, fila, balao->lists[i]);
            int num5 = lengthLst(lst); 
            //printf("\nNúmero de elementos na lista: %d\n\n", num5);

            for(Node *pCurr=getFirstLst(lst); pCurr != NULL; pCurr=getNextLst(lst, pCurr))
            {
                Item item = getLst(lst, pCurr);
                insertLst(lista_completa, item);
            }
        }
        else if (i == 0) // Verifica se a lista de índice 0 existe
        {
            printf("A lista %d da fila %d não existe!\n", i, fila);
        }
        else
        {
            printf("Não foi possível encontrar a lista %d na fila %d\n", i, fila);
        }
    }

    //verificando quantos elementos tem na lista
    //int num = lengthLst(lista_completa); 
    //printf("\nNúmero de elementos na lista completa: %d\n\n", num);
    return lista_completa;
}

