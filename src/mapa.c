#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // permite carregar imagens
#include "mapa.h" //conecta a chamada das declarações do .h

// Ponteiro para armazenar a imagem do mapa
static ALLEGRO_BITMAP *mapa = NULL;
//null por que a imagem nao foi carregada
bool carregar_mapa(void){
    mapa = al_load_bitmap("assets/imagens/mapa.egito.png"); // tenta carregar imagem do disco para memoria

    //verefica o carregamento da imagem
    if(mapa == NULL){
        printf("Erro ao Carregar o mapa");
        return false;
    }
    printf("Mapa Carregado");
    return true;
}
void desenhar_mapa(float cameraX, float cameraY)
{
    if (mapa != NULL)
    {
        al_draw_scaled_bitmap(
            mapa,
            0, 0,
            al_get_bitmap_width(mapa),
            al_get_bitmap_height(mapa),

            // Posicao do mapa na tela, quando personagem anda para direira o mapa se move para esquerda
            -cameraX, -cameraY,

            // Tamanho do mundo
            LARGURA_MUNDO, ALTURA_MUNDO,
            0
        );
    }
}
void liberar_mapa(void){
    if (mapa!=NULL)
    {
        al_destroy_bitmap(mapa);
        mapa = NULL;
    }
    
}