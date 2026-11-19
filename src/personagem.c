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

//Vetor que guarda o estado das teclas
static bool teclas[ALLEGRO_KEY_MAX] = {false}; // static vetor restrito aquele arquivo; key max posições que representam as teclas; bool true tecla pressionada, false solta

bool carregar_personagem_mapa(void){
    frente = al_load_bitmap("assets/imagens/personagemFrente.png"); // tenta carregar imagem do disco para memoria
    if (frente == NULL){
        printf("Erro em carregar o personagem");
        return false;
    }
    printf("Personagem Carregado");
    return true;
}
//Guarda quando uma tecla é pressionada ou solta
void personagem_tecla(int tecla, bool pressionada){
    if (tecla>=0 && tecla< ALLEGRO_KEY_MAX)
    {
        teclas[tecla] = pressionada;
    }
}
// Atualizar a posição do personagem
void atualizar_posicao_personagem(int largura_mapa, int altura_mapa){
    float velocidade = 3;
    //movimento horizontal
    if (teclas[ALLEGRO_KEY_RIGHT]) //direita
    {
        x += velocidade;
    }
    if (teclas[ALLEGRO_KEY_LEFT]) //esquerda
    {
        x-=velocidade;
    }

    //Movimento vertical
    if (teclas[ALLEGRO_KEY_UP])
    {
        y -= velocidade;
    }
    if(teclas[ALLEGRO_KEY_DOWN])
    {
        y+=velocidade;
    }

    // Limites horizontais
    if (x < 0)
    {
        x = 0;
    }

    if (x + largura > largura_mapa)
    {
        x = largura_mapa - largura;
    }

    // Limites verticais
    if (y < 0)
    {
        y = 0;
    }

    if (y + altura > altura_mapa)
    {
        y = altura_mapa - altura;
    }
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