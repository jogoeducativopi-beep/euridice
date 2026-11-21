#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#ifndef COLISAO_H
#define COLISAO_H
#include <stdbool.h>

//estrutura das informações do retangulo
typedef struct
{
    float x; //posição horizontal
    float y; //poseção vertical
    float width; //largura
    float height; //altura
}Retangulo;

//verifica se estão colidindo
bool colisao(Retangulo jogador, Retangulo obstaculo);

//verifica se o personagem pode ocupar aquela posição no mapa
bool posicao_bloqueada(float x, float y, float largura, float altura);

// Desenha as areas de colisao para depuracao
void desenhar_colisoes(float cameraX, float cameraY);

#endif
