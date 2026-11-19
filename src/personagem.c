#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "personagem.h"

//Imagem do personagem // Ponteiro para armazenar a imagem do personagem
static ALLEGRO_BITMAP* frente = NULL;

//Posição inicial do personagem no mapa
static float x = 650;
static float y = 550;

//Tamanho do personagem no mapa
static float largura = 75;
static float altura = 50;

bool carregar_personagem_mapa(void){
    frente = al_load_bitmap("assets/imagens/personagemFrente.png"); // tenta carregar imagem do disco para memoria
    if (frente == NULL){
        printf("Erro em carregar o personagem");
        return false;
    }
    printf("Personagem Carregado");
    return true;
}
void desenha_personagem(void){
    if (frente != NULL)
    {
        al_draw_scaled_bitmap(
            frente,
            0, 0,
            al_get_bitmap_width(frente),
            al_get_bitmap_height(frente),
            x, y,
            largura, altura,
            0
        );
    }
}
void liberar_personagem_mapa(void){
    if (frente!=NULL)
    {
        al_destroy_bitmap(frente);
        frente = NULL;
    }
    
}