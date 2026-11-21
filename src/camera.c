#include "camera.h"
#include "mapa.h"

// Posicao da camera dentro do mapa (00) canto superior esquerdo do mapa
static float cameraX = 0;
static float cameraY = 0;

//atualiza a camera seguindo o personagem
void atualizar_camera(float jogadorX, float jogadorY,float larguraTela, float alturaTela)
{
    // Centraliza a camera no personagem
    cameraX = jogadorX - larguraTela / 2;
    cameraY = jogadorY - alturaTela / 2;
    //divide a tela por 2 e depois subtrai da posição do jogador, fazendo assim ele continuar no centro

    // Limites maximos da camera 0 a 1004
    float limiteX = LARGURA_MUNDO - larguraTela;
    float limiteY = ALTURA_MUNDO - alturaTela;

    if (limiteX < 0)
        limiteX = 0;

    if (limiteY < 0)
        limiteY = 0;

    // Impede mostrar fora do mapa
    if (cameraX < 0)
        cameraX = 0;

    if (cameraY < 0)
        cameraY = 0;

    if (cameraX > limiteX)
        cameraX = limiteX;

    if (cameraY > limiteY)
        cameraY = limiteY;
}

// Permite consultar a posicao da camera
float posicaoCameraX(void)
{
    return cameraX;
}

float posicaoCameraY(void)
{
    return cameraY;
}
//devolver o valor da camera porque a variavel é static entao outros arquivos nao acessam