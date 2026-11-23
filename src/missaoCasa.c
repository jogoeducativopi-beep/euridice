#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "missaoCasa.h"

// Ponteiro que guarda a imagem do interior da casa
static ALLEGRO_BITMAP *imagemCasa = NULL;

// Controla se a janela da casa esta aberta
static bool casaAberta = false;

// carrega imagem da casa
bool iniciar_casa(void)
{
    imagemCasa = al_load_bitmap("assets/imagens/interiorCasa.png");

    if (imagemCasa == NULL)
    {
        printf("Erro ao carregar interior da casa \n");
        return false;
    }
    printf("Interior da casa foi carregado \n");
    return true;
}

void abrir_casa(void)
{
    casaAberta = true;
}

void fechar_casa(void)
{
    casaAberta = false;
}

bool casa_aberta(void)
{
    return casaAberta;
}

void desenhar_casa(void)
{
    // Verifica se a casa esta aberta e a imagem foi carregada
    if (casaAberta && imagemCasa != NULL)
    {
        // Desenha a imagem ocupando toda a janela
        al_draw_scaled_bitmap(
            imagemCasa,
            0, 0,
            al_get_bitmap_width(imagemCasa),
            al_get_bitmap_height(imagemCasa),
            0, 0,
            1300, 800,
            0);
    }
}

void liberar_casa(void)
{
    // Verifica se a imagem foi carregada
    if (imagemCasa != NULL)
    {
        // Libera a imagem da memoria
        al_destroy_bitmap(imagemCasa);

        // O ponteiro nao aponta mais para a imagem
        imagemCasa = NULL;
    }
}

void clique_objetos_casa(int mouseX, int mouseY)
{
    // Verifica se estamos dentro da casa
    if (!casaAberta)
    {
        return;
    }

    // Verifica se clicou no vaso grande
    if (mouseX >= 230 && mouseX <= 365 &&
        mouseY >= 180 && mouseY <= 330)
    {
        printf("Clicou no vaso!\n");
    }

    // Verifica se clicou no mapa do tesouro
    else if (mouseX >= 800 && mouseX <= 1120 &&
             mouseY >= 310 && mouseY <= 450)
    {
        printf("Clicou no mapa do tesouro!\n");
    }
}
