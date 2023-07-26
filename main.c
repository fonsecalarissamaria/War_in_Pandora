#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "arq_svg.h"
#include "retangulo.h"
#include "circulo.h"
#include "linha.h"
#include "texto.h" 
#include "fila.h"
#include "balao.h"
#include "math.h"
#include "forma.h"
#include "caca.h"
#include "bomba.h"

/*
        Aluna: Larissa Maria dos Santos Fonseca 
        Matrícula: 202200560118
*/



typedef struct StNode Node;

struct coordenadas 
{
    double x;
    double y;
};

union vetor 
{
    Balao b;
    Caca c;
};



//Protótipos
void le_comando(int argc, char *argv[], char **entrada, char **saida, char **arquivoGeo, char **arquivoQry, 
                 char **sufixo_completo, char **caminho_completoGeo, char **caminho_completoQry);
void abrir_arquivo(char* path, FILE** file);
char* strdup(const char* s);
void escreverFormasSvg(Lista* r, Lista* c, Lista* l, Lista* t, ArqSvg Desenho);
void desenharFotosSvg(Lista* formas, ArqSvg desenho);
bool checkRetangulo(Item item, void *extra);
void imprime_endereco_elemento(void *objeto, void *auxiliar);






void processaGeo(FILE *geo_file, Lista circulosDaListaBD, Lista retangulosDaListaBD, 
                            Lista linhasDaListaBD, Lista textosDaListaBD, Lista listaBD, 
                            Balao* vetorBalao[], Caca* vetorCacas[], char *FontFamily, 
                            char *FontWeigth, double size)
{
    
    int num; 
    char line[150];
    int id, q=1;
    double x1, y1, x2, y2, dx, raio, largura, altura, profundidade, graus, distancia;
    char corb[10], corp[10], ancora[1], txt[500], sufixo[20], capacidade[5];
    Balao balloon;
    Forma f;
    union vetor v[1000];

    while (fgets(line, 150, geo_file) != NULL) 
    {
        if (line[0] == 'c') 
        {
            Circulo c;
            sscanf(line, "c %d %lf %lf %lf %s %s", &id, &x1, &y1, &raio, corb, corp);
            //printf("Id: %d X1: %lf Y1: %lf Raio: %lf Corb: %s Corp: %s\n", id, x1, y1, raio, corb, corp);
            
            //Criando o circulo e adicionando a lista Banco de Dados 
            c = criaCirculo(x1,y1,raio,id,corb,corp);
            Posic p = insertLst(circulosDaListaBD, c); 
            Item I = getLst(circulosDaListaBD, p);
            f = guarda_formato(c, "c"); 
            insertLst(listaBD, f); 

        }else if (line[0] == 'r') 
        {
            Retangulo r;
            sscanf(line, "r %d %lf %lf %lf %lf %s %s", &id, &x1, &y1, &largura, &altura, corb, corp);
            //printf("RETANGULO: %d %lf %lf %lf %lf %s %s\n", id, x1, y1, largura, altura, corb, corp);
            
            //Criando o retangulo e adicionando na lista Banco de Dados 
            r = criaRetangulo(x1,y1,largura,altura,id,corb,corp);
            Posic p2 = insertLst(retangulosDaListaBD, r);
            Item i2 = getLst(retangulosDaListaBD, p2); 
            f = guarda_formato(r, "r"); 
            insertLst(listaBD, f);

        }else if (line[0] == 'l') 
        {
            Linha l;
            sscanf(line, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, corp);
            //printf("********Antes Id: %d %lf %lf %lf %lf %s\n", id, x1, y1, x2, y2, corp);

            //Criando a linha e adicionando na lista Banco de Dados 
            l = criaLinha(x1,y1,x2,y2,id,corp);
            Posic p3 = insertLst(linhasDaListaBD, l);
            Item i3 = getLst(linhasDaListaBD, p3);             
            f = guarda_formato(l, "l");
            insertLst(listaBD, f);
            
        } else if ((line[0] == 't') && (line[1] == 's'))
        {
            sscanf(line, "ts %s %s %lf", FontFamily, FontWeigth, &size);
            //printf("Estilo dos TEXTOS: %s %s %lf\n", FontFamily, FontWeigth, size);

        }else if (line[0] == 't' && (line[1] == ' ')) 
        {
            
            Texto t;
            sscanf(line, "t %d %lf %lf %s %s %s %s", &id, &x1, &y1, corb, corp, ancora, txt);
            //printf("Id: %d X: %lf Y: %lf Corb: %s Corp: %s Ancora: %s Texto: %s \n", id, x1, y1, corb, corp, ancora, txt);
            
            //Criando o texto e adicionando na lista Banco de Dados 
            t = criaTexto(x1,y1,id,ancora,txt,corb,corp,size,FontFamily,FontWeigth);
            Posic p4 = insertLst(textosDaListaBD, t);
            Item i4 = getLst(textosDaListaBD, p4); 
            f = guarda_formato(t, "t");
            insertLst(listaBD, f);

            if (strcmp(txt, "v_O_v") == 0) //Criando as filas de fotos para cada balão 
            {
                vetorBalao[id] = cria_balao(id, t); 
                //printf("balloon %d: %p\n",id, vetorBalao[id]);
            }
            if (strcmp(txt, "|-T-|") == 0) //Criando os caças  
            {
                vetorCacas[id] = cria_caca(id, t); 
                //printf("caça %d: %p\n",id, vetorCacas[id]);
            }
        }
    }
}


void processaQry(FILE *qry_file, Lista circulosDaListaBD, Lista retangulosDaListaBD, 
                Lista linhasDaListaBD, Lista textosDaListaBD, Lista listaBD,  FILE *arquivoTXT, 
                       char *saida, char *sufixo_completo, Balao* vetorBalao[], Caca* vetorCacas[], 
                       char *FontFamily, char *FontWeigth, double size, Lista formasAux)
{

    //Criando variáveis que vão me auxiliar no programa 
    int num;
    char line[100];
    int id, q=1;
    double x1, y1, x2, y2, dx, raio, largura, altura, profundidade, graus, distancia;
    char corb[10], corp[10], ancora[1], txt[500], sufixo[20], capacidade[5];
    Balao balloon;
    Forma f; 
    

   
    //Chamando as funções do .qry 
    if (qry_file != NULL)
    { 
        while (fgets(line, 100, qry_file) != NULL) 
        {     
            //-----> MOVIMENTA OS OBJETOS 
            if ((line[0] == 'm') && (line[1] == 'v')) 
            {
                sscanf(line, "mv %d %lf %lf", &id, &x1, &y1);
                //printf("id: %d novoX: %lf novoY: %lf\n", id, x1, y1);
                int idR, idC, idL, idT; 

                //percorrendo a lista de retângulos e comparando os identificadores 
                Posic p = getFirstLst(retangulosDaListaBD); 
                while (p != NULL)  
                { 
                    Retangulo r = getLst(retangulosDaListaBD, p);  
                    idR = getIdentificadorRetangulo(r); 
                    if(id == idR)
                    {
                        double xRect = getPosicXRect(r);
                        double yRect = getPosicYRect(r);
                        moveRetangulo(r, xRect, yRect, x1, y1);
                        fprintf(arquivoTXT, "Moveu o Retângulo de identificador: %d Posição original: %lf,%lf Posição final: %lf,%lf\n", idR, xRect, yRect, (xRect+x1), (yRect+y1));
                        break; 
                    }else
                    {
                        p = getNextLst(retangulosDaListaBD, p);  // Obtendo o próximo indicador de item;
                    }
                }

                //percorrendo a lista de circulos e comparando os identificadores 
                Posic p2 = getFirstLst(circulosDaListaBD);  
                while (p2 != NULL) 
                { 
                    Circulo c = getLst(circulosDaListaBD, p2);  
                    idC = getIdentificadorCirculo(c); 
                    if(id == idC)
                    {
                        double xCirc = getPosicXCircle(c);
                        double yCirc = getPosicYCircle(c);
                        moveCirculo(c, xCirc, yCirc, x1, y1);
                        fprintf(arquivoTXT, "Moveu o Circulo de identificador: %d Posição original: %lf,%lf Posição final: %lf,%lf\n", idC, xCirc, yCirc, (x1+xCirc), (y1+yCirc));
                        break;
                    }else
                    {
                        p2 = getNextLst(circulosDaListaBD, p2);  
                    }
                }

                //percorrendo a lista de linhas e comparando os identificadores 
                Posic p3 = getFirstLst(linhasDaListaBD);  
                while (p3 != NULL) 
                { 
                    Linha l = getLst(linhasDaListaBD, p3);  
                    idL = getIdentificadorLine(l);
                    if(id == idL)
                    { 
                        double x1Line = getPosicX1Line(l);
                        double y1Line = getPosicY1Line(l);
                        double x2Line = getPosicX2Line(l);
                        double y2Line = getPosicY2Line(l);
                        //printf("id: %d X1: %lf X2: %lf Y1: %lf Y2: %lf\n",id, x1Line, x2Line, y1Line, y2Line);
                        moveLinha(l, x1Line, y1Line, x2Line, y2Line, x1, y1);
                        fprintf(arquivoTXT, "Moveu a Linha de identificador: %d Posição original da primeira ancora: %lf,%lf Posição final da primeira ancora: %lf,%lf\n", idL, x1Line, y1Line, (x1Line+x1), (y1Line+y1));
                        break;
                    }else
                    {
                        p3 = getNextLst(linhasDaListaBD, p3); 
                    }
                }

                //percorrendo a lista de textos e comparando os identificadores 
                Posic p4 = getFirstLst(textosDaListaBD);  
                while (p4 != NULL) 
                { 
                    Texto t = getLst(textosDaListaBD, p4);  
                    idT = getIdentificadorText(t); 
                    if(id == idT)
                    {
                        double xTexto = getPosicXText(t);
                        double yTexto = getPosicYText(t);
                        moveTexto(t, xTexto, yTexto, x1, y1);
                        fprintf(arquivoTXT, "Moveu o Texto de identificador: %d Posição original: %lf,%lf Posição final: %lf,%lf\n", idT, xTexto, yTexto, (xTexto+x1), (yTexto+y1));
                        break;
                    }else
                    {
                        p4 = getNextLst(textosDaListaBD, p4);  
                    } 
                }
            }
    
            
            //-----> GIRA OS TEXTOS 
            if ((line[0] == 'g'))
            {
                sscanf(line, "g %d %lf", &id, &graus);
                //printf("id: %d graus: %lf", id, graus);
                int idT; 

                //percorrendo a lista de textos e comparando os identificadores 
                Posic p4 = getFirstLst(textosDaListaBD);  
                while (p4 != NULL) 
                { 
                    
                    Texto t = getLst(textosDaListaBD, p4);  
                    idT = getIdentificadorText(t);  
                    double x = getPosicXText(t);
                    double y = getPosicYText(t); 
                    double anguloOriginal = getAnguloText(t); 
                    //printf("identificador do Texto: %d\n", idT); 
                    
                    if(id == idT)
                    {
                        giraTexto(t, graus); 
                        fprintf(arquivoTXT, "Girou o Texto de identificador: %d Ângulo original: %lf Ângulo final: %lf\n", idT, anguloOriginal, (anguloOriginal+graus));
                        //printf("GIRA: Coordenadas do Texto: (%lf, %lf)\n", getPosicXText(t), getPosicYText(t)); 
                        break;
                    }else
                    {
                        p4 = getNextLst(textosDaListaBD, p4);  
                    } 
                }
            } 
    
    
    
    
            
            //----> FOCALIZA FOTO 
            if ((line[0] == 'f') && (line[1] == 'f'))   
            {
                sscanf(line, "ff %d %lf %lf %lf", &id, &raio, &profundidade, &altura);
                //printf("id: %d raio: %lf profundidade: %lf e altura: %lf", id, raio, profundidade, altura);
            }
            
    

            
    
    
            
            //-----> TIRA FOTO
            if ((line[0] == 't') && (line[1] == 'f'))  
            {
                int h = 0, idBalao, idC, idL, idT, idR, numFila; 

                //Lendo o .qry e armazenando as info sobre o balão e em qual a fila (dentre as filas) que vai guardar a foto
                sscanf(line, "tf %d %d", &idBalao, &numFila);
                //printf("\nid Balão: %d numero da Fila: %d\n\n", idBalao, numFila);
                
                                   

                //Percorrendo a lista a procura do identificador do balão
                Posic p5 = getFirstLst(textosDaListaBD);  
                while (p5 != NULL) 
                { 
                    Texto textinho = getLst(linhasDaListaBD, p5);  
                    idT = getIdentificadorText(textinho); 
                    Forma f2;

                    if(idT == idBalao)
                    {
                        //Procurando o balão que quero tirar fotos 
                        int xBalao = getPosicXText(textinho); 
                        int yBalao = getPosicYText(textinho); 
                        Item ponteir = getLst(textosDaListaBD, p5);
                        //printf("X do balão: %d Y do balão: %d\n", xBalao, yBalao);
                        
                        Lista* l; //ponteiro para a lista que vai armazenar as formas geométricas da foto  
                        l = insere_foto_na_fila(idBalao, numFila, vetorBalao[idBalao]); 

                        //Definindo as limitações da foto 
                        double Xmin, Xmax, Ymin, Ymax;
                        Xmin = (xBalao - raio);
                        Xmax = (xBalao + raio); 
                        Ymin = yBalao + profundidade;
                        Ymax = yBalao + profundidade + altura;   
                        //printf("\nArea da Foto Xmin: %lf Xmax: %lf Ymin: %lf Ymax: %lf\n", Xmin, Xmax, Ymin, Ymax); 


                        //Desenhando o traço da foto no SVG 
                        Retangulo foto = criaRetangulo(Xmin, Ymin, (2*raio), altura, 100000, "red", "none"); 
                        Forma fotoPontilhada = guarda_formato(foto, "r"); 
                        insertLst(formasAux, fotoPontilhada);                                                                                                      
                        
                        fprintf(arquivoTXT, "Balão de id %d tirou fotos e inseriu na fila %d.\n            Identificadores e Formas:\n", idBalao, numFila);

                        //Procurando na lista de Retângulos se há algum deles na região da foto 
                        for(Node *pCurr=getFirstLst(retangulosDaListaBD); pCurr != NULL; pCurr=getNextLst(retangulosDaListaBD, pCurr))
                        {   
                            Retangulo rect = getLst(retangulosDaListaBD, pCurr); 
                            double XmaxRect = getPosicXRect(rect)+getLargura(rect); 
                            double YmaxRect = getPosicYRect(rect) + getAltura(rect); 
                            int idR = getIdentificadorRetangulo(rect); 
                            
                            // Verifica se há interseção
                            bool dentro = getPosicXRect(rect) >= Xmin && XmaxRect <= Xmax && getPosicYRect(rect) >= Ymin && YmaxRect <= Ymax;
                            bool parcialmenteDentro = (getPosicXRect(rect) >= Xmin && getPosicXRect(rect) <= Xmax &&
                            getPosicYRect(rect) >= Ymin && getPosicYRect(rect) <= Ymax) ||
                            (getPosicXRect(rect) + getLargura(rect) >= Xmin && getPosicXRect(rect) + getLargura(rect) <= Xmax &&
                            getPosicYRect(rect) + getAltura(rect) >= Ymin && getPosicYRect(rect) + getAltura(rect) <= Ymax);
                            
                            // Imprime o resultado e insere na lista do balão 
                            if (dentro || parcialmenteDentro) 
                            {
                                f2 = guarda_formato(rect, "r"); 
                                insertLst(l, f2); 
                                fprintf(arquivoTXT, "            Retangulo de identificador: %d Posição Relativa à borda: %lf,%lf\n", idR, (getPosicXRect(rect)+Xmin), (getPosicYRect(rect)+Ymin));
                                //printf("O retângulo %d está completamente dentro do retângulo de referência.\n", getIdentificadorRetangulo(rect));
                            }
                        }
    
    

                        //Procurando na lista de Circulos se há algum deles na região da foto
                        for(Node *pCurr=getFirstLst(circulosDaListaBD); pCurr != NULL; pCurr=getNextLst(circulosDaListaBD, pCurr))
                        {   
                            Circulo circle = getLst(circulosDaListaBD, pCurr); 
                            int idC = getIdentificadorCirculo(circle); 
                            double xCirculo = getPosicXCircle(circle);
                            double yCirculo = getPosicYCircle(circle);
                            double raioCirculo = getRaio(circle); 
                            double diagonal = distanciaEntrePontos(Xmin, Ymin, Xmax, Ymax);
                            double distanciaCentros = distanciaEntrePontos(xCirculo, yCirculo, (Xmin + Xmax)/2, (Ymin + Ymax)/2);

                            bool dentro = (xCirculo >= Xmin + raioCirculo &&
                                            xCirculo <= Xmax - raioCirculo &&
                                            yCirculo >= Ymin + raioCirculo &&
                                            yCirculo <= Ymax - raioCirculo);
                            
                            bool parcialmenteDentro = (distanciaCentros <= raioCirculo + diagonal/2);

                            // Imprime o resultado e insere na lista do balão 
                            if (dentro || parcialmenteDentro) 
                            {
                                f2 = guarda_formato(circle, "c"); 
                                insertLst(l, f2); 
                                fprintf(arquivoTXT, "            Circulo de identificador: %d Posição Relativa à borda: %lf,%lf\n", idC, (getPosicXCircle(circle)+Xmin), (getPosicYCircle(circle)+Ymin));
                                //printf("O circulo %d está completamente ou parcialmente dentro do retângulo de referência.\n", getIdentificadorCirculo(circle));
                            }
                        }
    
    

                        //Procurando na lista de Linhas se há alguma delas na região da foto
                        int u = 0; 
                        for(Node *pCurr=getFirstLst(linhasDaListaBD); pCurr != NULL; pCurr=getNextLst(linhasDaListaBD, pCurr))
                        {   
                            Linha line = getLst(linhasDaListaBD, pCurr); 
                            int idL = getIdentificadorLine(line);
                            double x1 = getPosicX1Line(line);
                            double y1 = getPosicY1Line(line);
                            double x2 = getPosicX2Line(line);
                            double y2 = getPosicY2Line(line);
                            bool dentro1 = (x1 >= Xmin && x1 <= Xmax && y1 >= Ymin && y1 <= Ymax);
                            bool dentro2 = (x2 >= Xmin && x2 <= Xmax && y2 >= Ymin && y2 <= Ymax);
                            
                            // Imprime o resultado e insere na lista do balão
                            if (dentro1 || dentro2) 
                            {
                                f2 = guarda_formato(line, "l"); 
                                insertLst(l, f2); 
                                fprintf(arquivoTXT, "            Linha de identificador: %d Posição Relativa à borda: %lf,%lf\n", idL, (getPosicX1Line(line)+Xmin), (getPosicY1Line(line)+Ymin));
                                //printf("a Linha %d está completamente ou parcialmente dentro do retângulo de referência.\n", getIdentificadorLine(line));
                            }
                        }
    
    

                        //Procurando na lista de Textos se há algum deles na região da foto
                        for(Node *pCurr=getFirstLst(textosDaListaBD); pCurr != NULL; pCurr=getNextLst(textosDaListaBD, pCurr))
                        {   
                            Texto tex = getLst(textosDaListaBD, pCurr); 
                            int idT = getIdentificadorText(tex);
                            bool dentro = (getPosicXText(tex) >= Xmin && getPosicXText(tex) <= Xmax && getPosicYText(tex) >= Ymin && getPosicYText(tex) <= Ymax);

                            // Imprime o resultado e insere na lista do balão
                            if (dentro) 
                            {
                                f2 = guarda_formato(tex, "t"); 
                                insertLst(l, f2); 
                                fprintf(arquivoTXT, "            Texto de identificador: %d Posição Relativa à borda: %lf,%lf\n", idT, (getPosicXText(tex)+Xmin), (getPosicYText(tex)+Ymin));
                                //printf("o Texto %d está dentro do retângulo de referência.\n", getIdentificadorText(tex));
                            }
                        }                         
                        break;
                    }else 
                        p5 = getNextLst(textosDaListaBD, p5);  // Obtendo o próximo indicador de item;
                }   
            }

            
            //-----> DOWNLOAD DAS FOTOS
            if ((line[0] == 'd') && (line[1] == 'f'))  
            {
                int idBalao, idC, idL, idT, idR, numFila, pontos = 0, numFoto = 0;
                char formato[5];

                //Lendo o .qry e armazenando as informações 
                sscanf(line, "df %d %d %s", &idBalao, &numFila, sufixo);
                //printf("\nid Balão que quero fazer Download : %d numero da Fila: %d sufixo: %s\n\n", idBalao, numFila, sufixo);
                
                //Procurando o balão de identificador idBalao 
                for(Node *pCurr=getFirstLst(textosDaListaBD); pCurr != NULL; pCurr=getNextLst(textosDaListaBD, pCurr))
                {   
                    Texto tex = getLst(textosDaListaBD, pCurr); 
                    idT = getIdentificadorText(tex);
                     

                    if(idBalao == idT)
                    { 
                        //Procurando o balão que quero fazer download 
                        Balao endereço = vetorBalao[idBalao]; 
                        //printf("Endereço do balão %d: %p\n", idBalao, endereço);
                        Lista *lista_base; 
                        lista_base = copia_listas_fila(endereço, numFila); 
                        
                        //Conferindo os endereços das formas 
                        //fold(lista_base, imprime_endereco_elemento, NULL);
                                        //----->OBS: Função fold comentada porque ficaria muita coisa escrita no terminal 

                        //Criando o arquivo SVG para gerar as fotos udo download 
                        char nome_arquivoSVG[100];
                        strcpy(nome_arquivoSVG, sufixo_completo);
                        strcat(nome_arquivoSVG, "-");
                        strcat(nome_arquivoSVG, sufixo);
                        strcat(nome_arquivoSVG, ".svg");
                        char caminho_arquivoSVG[200];
                        sprintf(caminho_arquivoSVG, "%s/%s", saida, nome_arquivoSVG);
                        ArqSvg fotosBase = abreEscritaSvg(caminho_arquivoSVG);
                        desenharFotosSvg(lista_base, fotosBase); 
                        
                        //--->Pontuando as fotos 
                        for(Node *pCurr=getFirstLst(lista_base); pCurr != NULL; pCurr=getNextLst(lista_base, pCurr))
                        {   
                            Forma forma = getLst(lista_base, pCurr);
                            strcpy(formato, getFormato(forma));
                            double pi = 3.14159265359;

                            // identifica o tipo da forma e chama a função de escrita correspondente
                            if (strcmp(formato, "r") == 0) 
                            {
                                Retangulo rect = getObjeto(forma);
                                double pontosR = ((getAltura(rect)*getLargura(rect))/4);
                                //printf("Id: %d Cor do preenchimento: %s\n", getIdentificadorRetangulo(rect), getCorpRect(rect)); 
                                
                                if(strcmp(getCorbRect(rect), "#80080") == 0)
                                {
                                    pontosR += 10;
                                }else if(strcmp(getCorbRect(rect), "#AA0088") == 0)
                                {
                                    pontosR += 15;
                                }

                                if(strcmp(getCorpRect(rect), "#008033") == 0)
                                {
                                    pontosR += 20;
                                }else if(strcmp(getCorpRect(rect), "#FFCC00") == 0)
                                {
                                    pontosR += 30;
                                }

                                pontos += pontosR;
                            }

                            if (strcmp(formato, "c") == 0) 
                            {
                                Circulo circle = getObjeto(forma);
                                double raioCirc = getRaio(circle); 
                                double pontosC = ((raioCirc*raioCirc*pi)/2); 
                                //printf("Id: %d Cor do preenchimento: %s\n", getIdentificadorCirculo(circle), getCorbCircle(circle)); 
                                
                                if(strcmp(getCorbCircle(circle), "#FFFFFF") == 0 && strcmp(getCorpCircle(circle), "#FFFF00") == 0)
                                {
                                    pontosC = pontosC * 8;
                                }else if(strcmp(getCorbCircle(circle), "#D45500") == 0 && strcmp(getCorpCircle(circle), "#FF7F2A") == 0)
                                {
                                    pontosC = pontosC * 2;
                                }else if(strcmp(getCorbCircle(circle), "#AA0000") == 0 && strcmp(getCorpCircle(circle), "#DE8787") == 0)
                                {
                                    pontosC = pontosC * 4;
                                }else if(strcmp(getCorbCircle(circle), "#FFFFFF") == 0 && strcmp(getCorpCircle(circle), "#B3B3B3") == 0)
                                {
                                    pontosC += 0;
                                }

                                pontos += pontosC;
                            }

                            if (strcmp(formato, "l") == 0) 
                            {
                                Linha line = getObjeto(forma);
                                double x1Linha =getPosicX1Line(line);
                                double y1Linha =getPosicY1Line(line);
                                double x2Linha =getPosicX2Line(line);
                                double y2Linha =getPosicY2Line(line);
                                double pontosL = distanciaEntrePontos(x1Linha, y1Linha, x2Linha, y2Linha);
                                //printf("Id: %d Comprimento da linha: %lf\n", getIdentificadorLine(line), pontosL); 
                                
                                if(strcmp(getCorLine(line), "#FFFF00") == 0)
                                {
                                    pontosL = pontosL * 3;
                                }else if(strcmp(getCorLine(line), "#DDFF55") == 0)
                                {
                                    pontosL = pontosL * 2;
                                }else if(strcmp(getCorLine(line), "#0000FF") == 0)
                                {
                                    pontosL = pontosL * 4;
                                }

                                pontos += pontosL;
                            }


                            if (strcmp(formato, "t") == 0) 
                            {
                                Texto text = getObjeto(forma);
                                char* texto = getTextoText(text); 
                                                                
                                if(strcmp(texto, "|-T-|") == 0)
                                {
                                    pontos = 100;
                                }else 
                                {
                                    double pontos = strlen(texto);
                                    //printf("Id: %d Pontos por quantidade de carecteres: %lf\n", getIdentificadorText(text), pontosT); 
                                }
                            }
                        }
                        fprintf(arquivoTXT, "DOWNLOAD: Pontuação da foto %d da fila %d = %d\n", numFoto, numFila, pontos);
                        numFoto++;                       
                    }
                }
            }



            //-----> DISPARO 
            if ((line[0] == 'd') && (line[1] == ' '))  
            {
                int idCaca, j, idC, idL, idT, idR;
                double raioBomba;

                //Lendo o .qry e armazenando as informações 
                sscanf(line, "d %d %s %lf %d %lf", &idCaca, capacidade, &distancia, &j, &dx);
                //printf("\nid Caça que vai atirar: %d Capacidade da Bomba: %s Distancia: %lf id clone: %d Dx: %lf\n\n", idCaca, capacidade, distancia, j, dx);

                //Contabilizando o número de disparos de um determinado caça 
                if(vetorCacas[idCaca] == NULL)
                {
                    printf("Caça de id: %d não foi definido no .geo\n", idCaca);
                }else 
                {
                    Caca endereçoC = vetorCacas[idCaca];
                    aumenta_numero_disparo(endereçoC); 
                }
                
                //Reporando no TXT
                fprintf(arquivoTXT, "Caça de identificador: %d atirou uma bomba de capacidade: %s Elementos atingidos:\n", idCaca, capacidade);

                //Definindo a capacidade da bomba 
                if(strcmp(capacidade, "A") == 0)
                {
                    raioBomba = 10;
                }else if(strcmp(capacidade, "B") == 0)
                {
                    raioBomba = 15;
                }else if(strcmp(capacidade, "C") == 0)
                {
                    raioBomba = 30;
                }
            
                //Procurando o caça que vai atirar 
                for(Node *pCurr=getFirstLst(textosDaListaBD); pCurr != NULL; pCurr=getNextLst(textosDaListaBD, pCurr))
                {   
                    
                    Texto tex = getLst(textosDaListaBD, pCurr); 
                    idT = getIdentificadorText(tex);

                    if(idCaca == idT)
                    { 
                        //printf("Achei o caça que vai atirar, é o %d!!\n", idT);
                        Caca enderecoCaca = vetorCacas[idCaca];
                        double anguloCaca = getAnguloText(tex);
                        double xCaca = getPosicXText(tex); 
                        double yCaca = getPosicYText(tex); 
                        
                        //Calculando as coordenadas que a bomba vai cair 
                        struct coordenadas c = calculaCoordenadas(xCaca, yCaca, distancia, anguloCaca);
                        //printf("Xcaça: %lf YCaça: %lf Angulo: %lf\n", xCaca, yCaca, anguloCaca);
                        //printf("Coordenadas calculadas: (%lf, %lf) Raio da Bomba: %lf Distancia: %lf\n", c.x, c.y, raioBomba, distancia);
                        
                        
                        
                        //Percorrendo a lista de retangulos e verificando se tem algum na área da bomba  
                        for(Node *pCurr=getFirstLst(retangulosDaListaBD); pCurr != NULL; pCurr=getNextLst(retangulosDaListaBD, pCurr))
                        {   
                            Retangulo rect = getLst(retangulosDaListaBD, pCurr); 
                            int idR = getIdentificadorRetangulo(rect); 
                            char formato[5]; 
                            double XmaxRect = getPosicXRect(rect)+getLargura(rect); 
                            double YmaxRect = getPosicYRect(rect) + getAltura(rect); 
                            double XminRect = getPosicXRect(rect);
                            double YminRect = getPosicYRect(rect); 
                            
                            //printf("RETÂNGULO: %d Xminimo: %lf Yminimo: %lf Xmaximo: %lf Ymaximo: %lf\n", getIdentificadorRetangulo(rect), getPosicXRect(rect), getPosicYRect(rect), XmaxRect, YmaxRect);

                            double Xm1 = (XminRect + XmaxRect) / 2.0;
                            double Ym1 = (YminRect + YmaxRect) / 2.0;
                            double Xm2 = (XminRect + XmaxRect) / 2.0;
                            double Ym2 = YminRect;
                            double Xm3 = XminRect;
                            double Ym3 = (YminRect + YmaxRect) / 2.0;
                            double Xm4 = XmaxRect;
                            double Ym4 = (YminRect + YmaxRect) / 2.0;

                            double d1 = distanciaEntrePontos(c.x, c.y, Xm1, Ym1);
                            double d2 = distanciaEntrePontos(c.x, c.y, Xm2, Ym2);
                            double d3 = distanciaEntrePontos(c.x, c.y, Xm3, Ym3);
                            double d4 = distanciaEntrePontos(c.x, c.y, Xm4, Ym4);

                            bool dentro = (d1 <= raioBomba && d2 <= raioBomba && d3 <= raioBomba && d4 <= raioBomba);
                            bool parcialmente_dentro = !dentro && (d1 <= raioBomba || d2 <= raioBomba || d3 <= raioBomba || d4 <= raioBomba);

                            if (dentro || parcialmente_dentro) 
                            {

                                //printf("O retangulo %d está na área que a bomba vai destruir\n", getIdentificadorRetangulo(rect));
                                guardaId(enderecoCaca, idR);
                                
                                fprintf(arquivoTXT, "       Retângulo de id %d e posição %lf,%lf foi atingido pelo caça\n", idR, XminRect, YminRect);

                                //Desenhando o X no SVG 
                                Texto X = criaTexto(XminRect, YminRect, 20001, "m", "X", "red", "red", 25, FontFamily, FontWeigth); 
                                Forma fotoPontilhada = guarda_formato(X, "t"); 
                                insertLst(formasAux, fotoPontilhada); 

                                for (Node *pCurr2 = getFirstLst(listaBD); pCurr2 != NULL; pCurr2 = getNextLst(listaBD, pCurr2)) 
                                {
                                    Forma forma = getLst(listaBD, pCurr2);
                                    strcpy(formato, getFormato(forma)); 
                                    if (strcmp(formato, "r") == 0) 
                                    {
                                        Retangulo rect2 = getObjeto(forma);
                                        int idR2 = getIdentificadorRetangulo(rect2);
                                        if(idR == idR2)
                                            //printf("É o mesmo retangulo!\n");
                                            removeLst(listaBD, pCurr2);
                                            break;
                                    }
                                }
                            }
                        }



                        //Percorrendo a lista de circulos e verificando se tem algum na área da bomba  
                        for(Node *pCurr=getFirstLst(circulosDaListaBD); pCurr != NULL; pCurr=getNextLst(circulosDaListaBD, pCurr))
                        {   
                            Circulo circle = getLst(circulosDaListaBD, pCurr);
                            int idC = getIdentificadorCirculo(circle);
                            char formato[5];
                            double Xcirc = getPosicXCircle(circle);
                            double Ycirc = getPosicYCircle(circle);
                            double d = distanciaEntrePontos(c.x, c.y, Xcirc, Ycirc); 
                            double r = getRaio(circle);
                             
                            //verificarndo se o círculo está dentro da área da bomba
                            bool dentro = (d <= raioBomba - r);
                            bool parcialmente_dentro = (d < raioBomba + r);
                            

                            //Elimina da lista geral 
                            if (dentro || parcialmente_dentro) 
                            {
                                //printf("O círculo %d está na área que a bomba vai destruir\n", getIdentificadorCirculo(circle));
                                guardaId(enderecoCaca, idC);
                                fprintf(arquivoTXT, "       Circulo de id %d e posição %lf,%lf foi atingido pelo caça\n", idC, Xcirc, Ycirc);

                                //Desenhando o X no SVG 
                                Texto X = criaTexto(Xcirc, Ycirc, 20001, "m", "X", "red", "red", 25, FontFamily, FontWeigth); 
                                Forma fotoPontilhada = guarda_formato(X, "t"); 
                                insertLst(formasAux, fotoPontilhada);

                                for (Node *pCurr2 = getFirstLst(listaBD); pCurr2 != NULL; pCurr2 = getNextLst(listaBD, pCurr2)) 
                                {
                                    Forma forma = getLst(listaBD, pCurr2);
                                    strcpy(formato, getFormato(forma)); 
                                    if (strcmp(formato, "c") == 0) 
                                    {
                                        Circulo circle2 = getObjeto(forma);
                                        int idC2 = getIdentificadorCirculo(circle2);
                                        if(idC == idC2)
                                        {
                                            //printf("É o mesmo circulo!\n");
                                            removeLst(listaBD, pCurr2);
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        

                        //Percorrendo a lista de linhas e verificando se tem alguma na área da bomba  
                        for(Node *pCurr=getFirstLst(linhasDaListaBD); pCurr != NULL; pCurr=getNextLst(linhasDaListaBD, pCurr))
                        {   
                            Linha line = getLst(linhasDaListaBD, pCurr); 
                            int idL1 = getIdentificadorLine(line);
                            char formato[5];
                            double x1 = getPosicX1Line(line);
                            double y1 = getPosicY1Line(line);
                            double x2 = getPosicX2Line(line);
                            double y2 = getPosicY2Line(line);


                            double distancia1 = distanciaEntrePontos(c.x, c.y, x1, y1);
                            double distancia2 = distanciaEntrePontos(c.x, c.y, x2, y2);

                            bool dentro = (distancia1 <= raioBomba && distancia2 <= raioBomba); 
                            bool parcialmente_dentro = (distancia1 < raioBomba || distancia2 < raioBomba);
                            
                            //Elimina da lista geral 
                            if (dentro || parcialmente_dentro) 
                            {
                                //printf("A linha %d está na área que a bomba vai destruir\n", getIdentificadorLine(line));
                                guardaId(enderecoCaca, idL1);
                                fprintf(arquivoTXT, "       Linha de id %d e posição %lf,%lf foi atingido pelo caça\n", idL1, x1, y1);

                                //Desenhando o X no SVG 
                                Texto X = criaTexto(x1, y1, 20001, "m", "X", "red", "red", 25, FontFamily, FontWeigth); 
                                Forma fotoPontilhada = guarda_formato(X, "t"); 
                                insertLst(formasAux, fotoPontilhada);

                                for (Node *pCurr2 = getFirstLst(listaBD); pCurr2 != NULL; pCurr2 = getNextLst(listaBD, pCurr2)) 
                                {
                                    Forma forma = getLst(listaBD, pCurr2);
                                    strcpy(formato, getFormato(forma)); 
                                    if (strcmp(formato, "l") == 0) 
                                    {
                                        Linha line2 = getObjeto(forma);
                                        int idL2 = getIdentificadorLine(line2);
                                        if(idL1 == idL2)
                                        {
                                            //printf("Achei a linha que quero explodir!\n");
                                            removeLst(listaBD, pCurr2);
                                            break;
                                        }
                                    }
                                }
                            }
                        }




                        //Percorrendo a lista de textos e verificando se tem algum na área da bomba
                        for(Node *pCurr=getFirstLst(textosDaListaBD); pCurr != NULL; pCurr=getNextLst(textosDaListaBD, pCurr))
                        {   
                            Texto tex = getLst(textosDaListaBD, pCurr); 
                            int idT1 = getIdentificadorText(tex);
                            double xTexto = getPosicXText(tex);
                            double yTexto = getPosicYText(tex); 
                            char formato[5];
                            double distancia = distanciaEntrePontos(getPosicXText(tex), getPosicYText(tex), c.x, c.y);
                            bool dentro = (distancia <= raioBomba);
                            
                            //Elimina da lista geral 
                            if (dentro) 
                            {
                                //printf("O texto %d: %s está na área que a bomba vai destruir\n", getIdentificadorText(tex), getTextoText(tex));
                                guardaId(enderecoCaca, idT1);
                                fprintf(arquivoTXT, "       Texto de id %d e posição %lf,%lf foi atingido pelo caça\n", idT1, xTexto, yTexto);

                                //Desenhando o X no SVG 
                                Texto X = criaTexto(xTexto, yTexto, 20001, "m", "X", "red", "red", 25, FontFamily, FontWeigth); 
                                Forma fotoPontilhada = guarda_formato(X, "t"); 
                                insertLst(formasAux, fotoPontilhada);
                                
                                for (Node *pCurr2 = getFirstLst(listaBD); pCurr2 != NULL; pCurr2 = getNextLst(listaBD, pCurr2)) 
                                {
                                    Forma forma = getLst(listaBD, pCurr2);
                                    strcpy(formato, getFormato(forma)); 
                                    if (strcmp(formato, "t") == 0) 
                                    {
                                        Texto tex2 = getObjeto(forma);
                                        int idT2 = getIdentificadorText(tex2);
                                         
                                        if(idT1 == idT2)
                                        {

                                            //-->Criando os clones 
                                            char* texto = getTextoText(tex2);
                                            if(strcmp(texto, "v_O_v") == 0)
                                            {
                                                //printf("O balão %d foi atingido!!!\n", getIdentificadorText(tex2));
                                                Balao endereço = vetorBalao[idT2];
                                                Lista *lista_clones; 
                                                lista_clones = copia_listas_todas_filas(endereço);
                                                char formato2[5];


                                                //--->USANDO A FUNÇÃO FILTER E O ITERADOR
                                                Lista *retangulos = filter(lista_clones, checkRetangulo, NULL); //Verificando na lista de clones se é um retangulo
                                                Iterador it = getIterator(retangulos, false); //Iterador it vai "percorrer" a lista de retangulos 

                                                while (!isIteratorEmpty(retangulos, it)) 
                                                {
                                                    Retangulo rect = getObjeto(getIteratorCurrent(retangulos, it));
                                                    double novoXRet = getPosicXRect(rect)+dx;
                                                    char *novaCorB = getCorpRect(rect); 
                                                    char *novaCorP = getCorbRect(rect); 
                                                    Retangulo Clone = criaRetangulo(novoXRet, getPosicYRect(rect), getLargura(rect), getAltura(rect), j, novaCorB, novaCorP); 
                                                    Forma f2 = guarda_formato(Clone, "r"); 
                                                    insertLst(listaBD, f2); 
                                                    j++;
                                                    Forma forma = (Forma) getIteratorNext(retangulos, it);
                                                    //printf("O clone é um retângulo!\n");
                                                }


                                                //Percorrendo a lista de clones e verificando se é circulo, texto ou linha 
                                                for(Node *pCurr=getFirstLst(lista_clones); pCurr != NULL; pCurr=getNextLst(lista_clones, pCurr))
                                                {   
                                                    Forma forma = getLst(lista_clones, pCurr);
                                                    strcpy(formato2, getFormato(forma));


                                                    if (strcmp(formato2, "c") == 0) 
                                                    {
                                                        Circulo circle = getObjeto(forma);
                                                        double novoXCirc = getPosicXCircle(circle)+dx;
                                                        char *novaCorBCircle = getCorpCircle(circle); 
                                                        char *novaCorPCircle = getCorbCircle(circle); 
                                                        Circulo Clone = criaCirculo(novoXCirc, getPosicYCircle(circle), getRaio(circle), j, novaCorBCircle, novaCorPCircle); 
                                                        fprintf(arquivoTXT, "Clone de um Circulo de id: %d foi criado. Atributos:\n       Posição: %lf,%lf\n       Cor Borda: %s\n       Cor Preenchimento: %s\n", j, novoXCirc, getPosicYCircle(circle), novaCorBCircle, novaCorPCircle);
                                                        Forma f2 = guarda_formato(Clone, "c"); 
                                                        insertLst(listaBD, f2);
                                                        j++;
                                                    }


                                                    if (strcmp(formato2, "l") == 0) 
                                                    {
                                                        Linha line = getObjeto(forma);
                                                        double novoX1Line = getPosicX1Line(line)+dx;
                                                        double novoX2Line = getPosicX2Line(line)+dx; 
                                                        char *novaCorLine = getCorLine(line); 
                                                        Linha Clone = criaLinha(novoX1Line, getPosicY1Line(line), novoX2Line, getPosicY2Line(line), j, novaCorLine); 
                                                        fprintf(arquivoTXT, "Clone de uma Linha de id: %d foi criado. Atributos:\n       Posição: %lf,%lf\n       Cor: %s\n", j, novoX1Line, getPosicY1Line(line), novaCorLine);
                                                        Forma f2 = guarda_formato(Clone, "l"); 
                                                        insertLst(listaBD, f2);
                                                        j++;
                                                    }

                                                    if (strcmp(formato2, "t") == 0) 
                                                    {
                                                        Texto text = getObjeto(forma);
                                                        double novoXTexto = getPosicXText(text)+dx; 
                                                        char *novaCorBTexto = getCorpText(text); 
                                                        char *novaCorPTexto = getCorbText(text); 
                                                        char *textoDoBalao = getTextoText(text); 
                                                        printf("id do texto J: %d FontFamily dele: %s\n", j, getFontFamily(text));
                                                        Texto Clone = criaTexto(novoXTexto, getPosicYText(text), j, getAncoraText(text), getTextoText(text), novaCorBTexto, novaCorPTexto, getSizeText(text), getFontFamily(text), getFontWeigth(text)); 
                                                        fprintf(arquivoTXT, "Clone de um Texto de id: %d foi criado. Atributos:\n       Posição: %lf,%lf\n       Cor Borda: %s\n       Cor Preenchimento: %s\n", j, novoXTexto, getPosicYText(text), novaCorBTexto, novaCorPTexto);
                                                        if (strcmp(textoDoBalao, "v_O_v") == 0) 
                                                        {
                                                            //printf("Achei um balão\n");
                                                            vetorBalao[j] = cria_balao(j, text); 
                                                        }
                                                        if (strcmp(textoDoBalao, "|-T-|") == 0) 
                                                        {
                                                            //printf("Achei um caça\n");
                                                            vetorCacas[j] = cria_caca(j, text); 
                                                        }
                                                        Forma f2 = guarda_formato(Clone, "t"); 
                                                        insertLst(listaBD, f2);
                                                        j++;
                                                    }
                                                }
                                                removeIterator(it);
                                            }
                                            removeLst(listaBD, pCurr2);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //---> REPORTA DADOS DO BALÃO 
            if ((line[0] == 'b') && (line[1] == '?'))
            {
                char formato[5]; 

                for(Node *pCurr=getFirstLst(listaBD); pCurr != NULL; pCurr=getNextLst(listaBD, pCurr))
                {   
                    Forma forma = getLst(listaBD, pCurr);
                    strcpy(formato, getFormato(forma));

                    if (strcmp(formato, "t") == 0) //Percorrendo a lista geral procurando os textos dela    
                    {
                        Texto text = getObjeto(forma);
                        char* texto = getTextoText(text);

                        if(strcmp(texto, "v_O_v") == 0) //Se o texto for um balão, reporta seus dados
                        {
                            int idT = getIdentificadorText(text);
                            Balao endereço = vetorBalao[idT];

                            //Reporta todos os dados do balão no TXT
                            fprintf(arquivoTXT, "Balão de identificador: %d\n", getIdentificadorText(text));
                            fprintf(arquivoTXT, "   Atributos do Balão:\n       Posição: %lf,%lf\n       Cor da borda: %s\n       Cor do preenchimento: %s\n", getPosicXText(text), getPosicYText(text), getCorbText(text), getCorpText(text));
                            for(int numeroFila=0; numeroFila<10; numeroFila++)
                            {
                                fprintf(arquivoTXT, "       Número de fotos na fila %d: %d\n", numeroFila, getNumeroFotos(endereço, numeroFila));
                            }
                            fprintf(arquivoTXT, "       Rotação corrente do balão: %lf graus\n", getAnguloText(text));
                        }
                    }
                }
            }

            //---> REPORTA DADOS DO CAÇA
            if ((line[0] == 'c') && (line[1] == '?'))
            {
                char formato[5]; 

                for(Node *pCurr=getFirstLst(listaBD); pCurr != NULL; pCurr=getNextLst(listaBD, pCurr))
                {   
                    Forma forma = getLst(listaBD, pCurr);
                    strcpy(formato, getFormato(forma));

                    if (strcmp(formato, "t") == 0) //Percorrendo a lista geral procurando os textos dela    
                    {
                        Texto text = getObjeto(forma);
                        char* texto = getTextoText(text);
                        //printf("TEXTO do texto: %s\n\n", texto);

                        if(strcmp(texto, "|-T-|") == 0) //Se o texto for um caça, reporte seus dados 
                        {
                            int idTC = getIdentificadorText(text);
                            Caca enderecoC = vetorCacas[idTC];
                            int tam = getTamanhoVetor(enderecoC);
                            int* ident = retornaIds(enderecoC);
                            
                            //Reporta todos os dados do caça no TXT
                            fprintf(arquivoTXT, "Caça de identificador: %d\n", getIdentificadorText(text)); 
                            fprintf(arquivoTXT, "   Atributos do Caça:\n       Posição: %lf,%lf\n       Cor da borda: %s\n       Cor do preenchimento: %s\n", getPosicXText(text), getPosicYText(text), getCorbText(text), getCorpText(text));
                            fprintf(arquivoTXT, "       Número de disparos já efetuados: %d\n", getNumeroDisparos(enderecoC));
                            fprintf(arquivoTXT, "       Rotação corrente do caça: %lf graus\n", getAnguloText(text));
                            fprintf(arquivoTXT, "       Identificadores dos elementos atingidos pelo caça: ");
                            for(int m=0; m<tam; m++)
                            {
                                fprintf(arquivoTXT, "%d, ", ident[m]);
                            }
                            fprintf(arquivoTXT, "\n");
                        }
                    }
                }
            }
        }
    }
}



int main(int argc, char *argv[]) 
{
    //Criando variáveis que vão me auxiliar no programa 
    Balao* vetorPonteirosBalao[5000];
    Caca* vetorPonteirosCacas[5000]; 
    char *entrada = NULL;
    char *saida = NULL;
    char *sufixo_completo = NULL;
    char *caminho_completoGeo = NULL;
    char *caminho_completoQry = NULL;
    char *arquivoGeo, *arquivoQry;
    char FontFamily[10]; 
    char FontWeigth[10];
    double size;
    Lista formasAuxiliares = createLst(300);

    //Chamando a função que guarda caminhos para os arquivos que serão lidos 
    le_comando(argc, argv, &entrada, &saida, &arquivoGeo, &arquivoQry, &sufixo_completo, 
                                                        &caminho_completoGeo, &caminho_completoQry);
    
   
    //Criando os ponteiros para abrir os arquivos para leitura 
    char* geo_path = NULL;
    char* qry_path = NULL;
    FILE* geo_file = NULL;
    FILE* qry_file = NULL;

    
    //Abrindo os arquivos .geo e .qry para leitura 
    if (caminho_completoGeo != NULL) 
       abrir_arquivo(caminho_completoGeo, &geo_file);

    if (caminho_completoQry != NULL)
        abrir_arquivo(caminho_completoQry, &qry_file); 
    


    //Criando as listas que vão armazenar os diferentes TAD's do arquivo .geo  
    Lista *listaBancoDados = createLst(1000);
    Lista *r = createLst(500);
    Lista *c = createLst(500);
    Lista *l = createLst(500);
    Lista *t = createLst(500); 



    //Criando o arquivo de texto para ser gerado com o SVG Final
    char nome_arquivoTXT[100]; 
    sprintf(nome_arquivoTXT, "%s.txt", sufixo_completo);
    //printf("Nome do arquivo TXT: %s\n", nome_arquivoTXT);
    char caminho_arquivoTXT[200];
    sprintf(caminho_arquivoTXT, "%s/%s", saida, nome_arquivoTXT);
    //printf("Caminho para gerar o TXT: %s\n", caminho_arquivoTXT);
    FILE *arquivoTXT;
    arquivoTXT = fopen(caminho_arquivoTXT, "w");

    //Colocando uma mensagem no .txt em casos de não haver .qry 
    if (qry_file == NULL)
    {
        fprintf(arquivoTXT, "         \nNão há arquivo .qry para reportar movimentações.\n");
    }
    

    //Processando o .geo e .qry 
    processaGeo(geo_file, c, r, l, t, listaBancoDados, vetorPonteirosBalao, vetorPonteirosCacas, FontFamily, FontWeigth, size);
    processaQry(qry_file, c, r, l, t, listaBancoDados, arquivoTXT, saida, sufixo_completo, vetorPonteirosBalao, vetorPonteirosCacas, FontFamily, FontWeigth, size, formasAuxiliares);

    

    //Criando e Escrevendo no arquivo SVG para testes
    char nome_arquivoSVG[100]; // tamanho máximo da string resultante
    sprintf(nome_arquivoSVG, "%s.svg", sufixo_completo);
    //printf("Nome do arquivo SVG: %s\n", nome_arquivoSVG);
    char caminho_arquivoSVG[200];
    sprintf(caminho_arquivoSVG, "%s/%s", saida, nome_arquivoSVG);
    ArqSvg desenho = abreEscritaSvg(caminho_arquivoSVG);
    desenharFotosSvg(listaBancoDados, desenho); 

    //Inserindo as formas auxiliares (marca da foto e marca de X) na lista Banco de Dados
    map(formasAuxiliares, insertLst, listaBancoDados); 
     
    //Escrevendo no arquivo SVG Final
    desenho = abreEscritaSvg(caminho_arquivoSVG);
    desenharFotosSvg(listaBancoDados, desenho);
    
     
    //Escrevendo no arquivo SVG Final
    desenho = abreEscritaSvg(caminho_arquivoSVG);
    desenharFotosSvg(listaBancoDados, desenho);     

    
    //Fechando os arquivos que foram abertos 
    if (qry_file != NULL) 
    {
        fclose(qry_file);
    }else 
        printf("ponteiro para qry NULO\n");

    if (geo_file != NULL) 
        fclose(arquivoTXT);

    if (geo_path != NULL) 
    {
        free(geo_path);
    }
    if (qry_path != NULL) 
    {
        free(qry_path);
    }

    //Liberando a memoria 
    killLst(listaBancoDados);
    killLst(r);
    killLst(l);
    killLst(c);
    killLst(t);
    killLst(formasAuxiliares);

    return 0;
}








                                //FUNÇÕES PARA USAR NO main.c 



/** Escreve textos no arquivo SVG, buscando elemento por elemento 
 * de cada tipo de lista. Exemplo: Escreve todos os retângulos da lista Retangulos,
 * depois escreve todos as linhas da lista de linhas, e assim por diante. */
void escreverFormasSvg(Lista* r, Lista* c, Lista* l, Lista* t, ArqSvg Desenho) 
{
    Node* pCurr;

    // escrever retângulos
    for (pCurr = getFirstLst(r); pCurr != NULL; pCurr = getNextLst(r, pCurr)) 
    {
        Retangulo rect = getLst(r, pCurr);
        escreveRetanguloSvg(rect, Desenho);
    }

    // escrever círculos
    for (pCurr = getFirstLst(c); pCurr != NULL; pCurr = getNextLst(c, pCurr)) 
    {
        Circulo circle = getLst(c, pCurr);
        escreveCirculoSvg(circle, Desenho);
    }

    // escrever linhas
    for (pCurr = getFirstLst(l); pCurr != NULL; pCurr = getNextLst(l, pCurr)) 
    {
        Linha line = getLst(l, pCurr);
        escreveLinhaSvg(line, Desenho);
    }

    // escrever textos
    for (pCurr = getFirstLst(t); pCurr != NULL; pCurr = getNextLst(t, pCurr)) 
    {
        Texto text = getLst(t, pCurr);
        escreveTextoSvg(text, Desenho);
    }

    fechaSvg(Desenho);
}


//Função para escrever no SVG passando apenas pela lista geral 
/** Escreve textos no arquivo SVG, buscando elemento por elemento 
 * de uma única lista geral (banco de dados). Essa lista é passada
 * no parâmetro Lista* formas. A função identifica o tipo de cada
 * item e escreve seu respectivo código no SVG. */
void desenharFotosSvg(Lista* formas, ArqSvg desenho) 
{
    Node* pCurr;

    char formato[5]; 

    // percorre a lista de formas
    for (pCurr = getFirstLst(formas); pCurr != NULL; pCurr = getNextLst(formas, pCurr)) 
    {
        Forma forma = getLst(formas, pCurr);
        strcpy(formato, getFormato(forma)); 
        //printf("********Formato: %s\n", formato);
        
        // identifica o tipo da forma e chama a função de escrita correspondente
        if (strcmp(formato, "r") == 0) 
        {
                Retangulo rect = getObjeto(forma);
                //printf("Retangulo: %d\n", getIdentificadorRetangulo(rect));
                escreveRetanguloSvg(rect, desenho);
        }else if(strcmp(formato, "c") == 0)
        {
            Circulo circle = getObjeto(forma);
            //printf("Circulo: %d\n", getIdentificadorCirculo(circle));
            escreveCirculoSvg(circle, desenho);
        }else if(strcmp(formato, "l") == 0)
        {
            Linha line = getObjeto(forma);
            //printf("Linha: %d\n", getIdentificadorLine(line));
            escreveLinhaSvg(line, desenho);
        }else if(strcmp(formato, "t") == 0)
        {
            Texto text = getObjeto(forma);
            //printf("Texto de id: %d Texto: %s\n", getIdentificadorText(text), getTextoText(text));
            escreveTextoSvg(text, desenho); 
        }else 
        {
            printf("Formato não encontrado!\n");
        }
    }

    fechaSvg(desenho);
}



/** Abre o arquivo de texto para ser lido.
 * A função recebe o caminho de onde está esse arquivo por 
 * meio do parâmetro char* path e abre o arquivo, no qual 
 * o caminho leva. */
void abrir_arquivo(char* path, FILE** file) 
{
    *file = fopen(path, "r");
    if (*file == NULL) 
    {
        printf("Erro ao abrir o arquivo %s.\n", path);
        return;
    }else
    {
        printf("Arquivo %s aberto com sucesso.\n", path);
    }
}


 
/** Função auxiliar para gerar o nome dos 
 * arquivos finais na função le_comando() */
char* strdup(const char* s) 
{
    // Calcula o tamanho da string
    size_t len = strlen(s) + 1;
    
    // Aloca memória suficiente para a cópia
    char* new_str = malloc(len);
    
    // Copia a string original para a nova área de memória
    if (new_str != NULL) {
        memcpy(new_str, s, len);
    }
    
    // Retorna o ponteiro para a nova cópia da string
    return new_str;
}


/** Função lê a linha de comando no terminal, guarda nas variáveis passadas por
 * parâmetro os seguintes conteúdos: entrada: caminho para arquivos de entrada -e
 * saida: caminho para arquivos de saída -o arquivoGeo: caminho recebido após -f
 * arquivoQry: caminho recebido após -q sufixo_completo: nome base dos arquivos de saída SVG e TXT 
 * caminho_completoGeo: caminho absoluto para achar arquivo .geo
 * caminho_completoQry: caminho absoluto para achar arquivo .qry*/
void le_comando(int argc, char *argv[], char **entrada, char **saida, char **arquivoGeo, 
                 char **arquivoQry, char **sufixo_completo, char **caminho_completoGeo, 
                 char **caminho_completoQry) 
{
    char *sufixo_geo = NULL;
    char *sufixo_qry = NULL;
    bool quiet; 
    char *caminho_completo = NULL;
    char *base_geo = NULL;
    char *base_qry = NULL;


    // Loop para extrair as informações dos argumentos de linha de comando
    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-e") == 0 && i+1 < argc) 
        {
            *entrada = argv[i+1];
            //printf("Achei o -e: %s\n", *entrada); 
        }
        else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) 
        {
            *saida = argv[i+1];
            //printf("Achei o -o: %s\n", *saida);
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) 
        {
            *arquivoGeo = argv[i+1];
            base_geo = argv[i+1];
            //printf("Achei o -f!!! Arquivo -f: %s\n", *arquivoGeo);

            //Concatenando -e com -f para dar o caminho completo do arquivo .geo 
            int tamanho2 = strlen(*entrada)+strlen(*arquivoGeo)+2; //tamanho das duas strings mais '/' e '\0' 
            *caminho_completoGeo = malloc(tamanho2*(sizeof(char)));
            sprintf(*caminho_completoGeo, "%s/%s", *entrada, *arquivoGeo);
        
            char *extensao = strchr(base_geo, '.'); //encontra a primeira ocorrência de '.'
            if (extensao != NULL) 
            {
                *extensao = '\0'; //se houver '.', substitui-o por '\0' para truncar a string
            }
            //printf("Base Geo Depois: %s\n", base_geo);

        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            *arquivoQry = argv[i+1]; 
            base_qry = argv[i+1];
            //printf("Achei o -q: %s\n", *arquivoQry);

            //printf("Arquivo Qry Antes de trabalhar sufixo: %s\n", *arquivoQry);
            int tamanho3 = strlen(*entrada)+strlen(*arquivoQry)+2; //tamanho das duas strings mais '/' e '\0' 
            *caminho_completoQry = malloc(tamanho3*(sizeof(char)));
            sprintf(*caminho_completoQry, "%s/%s", *entrada, *arquivoQry);
            //printf("Caminho completo Qry: %s\n", *caminho_completoQry);


            char *last_sep = strrchr(base_qry, '/'); //encontra a última ocorrência de '/'
            if (last_sep != NULL) 
            {
                base_qry = last_sep + 1; //se houver '/', atualiza dir_name para apontar para a string depois de '/'
            }
            char *extensao = strchr(base_qry, '.'); //encontra a primeira ocorrência de '.'
            if (extensao != NULL) 
            {
                *extensao = '\0'; //se houver '.', substitui-o por '\0' para truncar a string
            }
            //printf("Base Qry Depois: %s\n", base_qry);
            quiet = true;
        }
    }

    //Fazendo tratamento dos NOMES dos arquivos de saída 
    if(quiet)
    {
        int tamanho = strlen(base_geo)+strlen(base_qry)+2; //tamanho das duas strings mais '-' e '\0' 
        *sufixo_completo = malloc(tamanho*sizeof(char)); 
        sprintf(*sufixo_completo, "%s-%s", base_geo, base_qry);
    }else
    {
        *sufixo_completo = strdup(base_geo);
    }
    printf("Sufixo Completo: %s\n", *sufixo_completo);     
}



/** Função auxiliar para usar a função FILTER e o Iterador
 *  Verifica se o item é um Retângulo 
*/
bool checkRetangulo(Item item, void *extra) 
{
    Forma forma = (Forma) item;
    char formato[5];
    strcpy(formato, getFormato(forma));
    return strcmp(formato, "r") == 0;
}



/** Função auxiliar para usar a função FOLD
 *  Escreve o endereço do item passado por parâmetro 
*/
void imprime_endereco_elemento(void *objeto, void *auxiliar) 
{
    printf("%p ", *(Item *)objeto); 
}