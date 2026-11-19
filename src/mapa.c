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
void desenhar_mapa(int largura, int altura){
    if (mapa != NULL)
    {
        al_draw_scaled_bitmap(
            mapa,
            0, 0,
            al_get_bitmap_width(mapa),
            al_get_bitmap_height(mapa),
            0, 0,
            largura, altura,
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