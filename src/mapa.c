#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // permite carregar imagens
#include "mapa.h"                   //conecta a chamada das declarações do .h
#include "missaoMercado.h"

// Ponteiro para armazenar a imagem do mapa
static ALLEGRO_BITMAP *mapa = NULL;
// null por que a imagem nao foi carregada

static ALLEGRO_BITMAP *mapaCasaAberta = NULL;

bool carregar_mapa(void)
{
    mapa = al_load_bitmap("assets/imagens/mapa.egito.png"); // tenta carregar imagem do disco para memoria

    mapaCasaAberta = al_load_bitmap("assets/imagens/casa_aberto.png");

    // verefica o carregamento das imagens
    if (mapa == NULL || mapaCasaAberta == NULL)
    {
        printf("Erro ao Carregar o mapa");
        return false;
    }
    printf("Mapa Carregado");
    return true;
}
void desenhar_mapa(float cameraX, float cameraY)
{
    // Comeca utilizando o mapa com as portas fechadas
    ALLEGRO_BITMAP *mapaAtual = mapa;

    // Se o jogador montou a chave, muda para mapa porta aberta
    if (encontrou_chave_casa())
    {
        mapaAtual = mapaCasaAberta;
    }

    if (mapa != NULL)
    {
        al_draw_scaled_bitmap(
            mapaAtual,
            0, 0,
            al_get_bitmap_width(mapaAtual),
            al_get_bitmap_height(mapaAtual),

            // Posicao do mapa na tela, quando personagem anda para direira o mapa se move para esquerda
            -cameraX, -cameraY,

            // Tamanho do mundo
            LARGURA_MUNDO, ALTURA_MUNDO,
            0);
    }
}
void liberar_mapa(void)
{
    if (mapa != NULL)
    {
        al_destroy_bitmap(mapa);
        mapa = NULL;
    }
    if (mapaCasaAberta != NULL)
    {
        al_destroy_bitmap(mapaCasaAberta);
        mapaCasaAberta = NULL;
    }
}