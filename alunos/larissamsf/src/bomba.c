#include "bomba.h"

struct coordenadas
{
    double x;
    double y;
};

double distanciaEntrePontos(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


struct coordenadas getCoordenadasCentroTexto(double xInicial, double yInicial) 
{
    // Define o texto e a fonte
    char texto[] = "|-T-|";
    //char fonte[] = "Arial";
    int tamanhoFonte = 8;

    // Determina a largura do texto em pixels
    int larguraTexto = strlen(texto) * tamanhoFonte;

    // Calcula as coordenadas x e y do centro do texto
    double xCentroTexto = xInicial + (larguraTexto / 2.0);
    double yCentroTexto = yInicial + (tamanhoFonte / 2.0);

    // Retorna as coordenadas do centro do texto
    struct coordenadas centroTexto = {xCentroTexto, yCentroTexto};

    return centroTexto;
}


struct coordenadas calculaCoordenadas(double x, double y, double distancia, double angulo)
{
    //printf("//////Parametros: x: %lf y: %lf distancia: %lf angulo: %lf\n\n", x, y, distancia, angulo);
    const double PI = 3.14159265358979323846;
    const double angulo_inicial = 90.0; // Ângulo inicial de rotação do caça (em graus)
    
    // Calcula o vetor unitário apontando na direção do caça
    double angulo_total = (angulo_inicial + angulo) * (PI / 180.0);
    double vetorUnitarioX = cos(angulo_total);
    double vetorUnitarioY = sin(angulo_total);

    // Multiplica o vetor unitário pela distância desejada
    double novaCoordenadaX = x + vetorUnitarioX * distancia;
    double novaCoordenadaY = y + vetorUnitarioY * distancia;

    // Retorna as novas coordenadas
    struct coordenadas c = { novaCoordenadaX, novaCoordenadaY };
    
    return c;
}


bool linhaCruzAresta(double x1, double y1, double x2, double y2, double ax, double ay, double bx, double by) {
    // calcula a distância entre os pontos iniciais da linha e da aresta
    double d1 = distanciaEntrePontos(x1, y1, ax, ay);
    double d2 = distanciaEntrePontos(x2, y2, ax, ay);
    
    // calcula a distância entre os pontos finais da linha e da aresta
    double d3 = distanciaEntrePontos(x1, y1, bx, by);
    double d4 = distanciaEntrePontos(x2, y2, bx, by);
    
    
    
    // calcula a distância entre a linha e a aresta
    double det = (bx - ax)*(y2 - y1) - (x2 - x1)*(by - ay);
    double da = (y1 - ay)*(bx - ax) - (x1 - ax)*(by - ay);
    double db = (y1 - ay)*(x2 - x1) - (x1 - ax)*(y2 - y1);
    double ta = da / det;
    double tb = db / det;
    
    // se a distância entre a linha e a aresta está entre 0 e 1, considera que a linha cruza a aresta
    if (ta >= 0 && ta <= 1 && tb >= 0 && tb <= 1) {
        return true;
    }
    
    // se não encontrou cruzamento, retorna falso
    return false;
}