#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "missaoMercado.h"

// ponteiro que guarda a imagem da ativação do mercado
static ALLEGRO_BITMAP *imagemMercado = NULL;
// verifica se tela da imagem do mercado esta aberta
static bool mercadoAberto = false;

bool inicia_mercado(void)
{
    // carrega imagem do mercado
    imagemMercado = al_load_bitmap("assets/imagens/mercado.png");
    // verfica se a imagem foi carregada
    if (imagemMercado == NULL)
    {
        printf("Erro ao carregar imagem do mercado");
        return false;
    }
    printf("Mercado carregado");
    return true;
}

// abre a imagem da tela do mercado
void abrir_mercado(void)
{
    mercadoAberto = true;
}

/// fecha a imagem da tela do mercado
void fechar_mercado(void)
{
    mercadoAberto = false;
}

// verifica se a tela da imagem esta aberta
bool mercado_aberto(void)
{
    return mercadoAberto;
}

void desenhar_mercado(void)
{
    // verifica se a imagem foi carregada e o mercado aberto
    if (mercadoAberto && imagemMercado != NULL) //tel do mercado aberta e imagem mercado doferente de null
    {
        // Desenha a imagem na janela
        al_draw_scaled_bitmap(
            imagemMercado,
            0, 0,
            al_get_bitmap_width(imagemMercado),
            al_get_bitmap_height(imagemMercado),
            0, 0,
            1300, 800,
            0);
    }
}

void liberar_mercado(void)
{
    //verifica se a imagem existe na memoria antes de destruila
    if(imagemMercado != NULL)
    {
        //libera a memoria utilizada pela imagem
        al_destroy_bitmap(imagemMercado);
    }
    //o ponteiro não aponta mais para a imagem
    imagemMercado = NULL;
}