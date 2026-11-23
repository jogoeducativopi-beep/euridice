#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "missaoCasa.h"

// Ponteiro que guarda a imagem do interior da casa
static ALLEGRO_BITMAP *imagemCasa = NULL;

// Controla se a janela da casa esta aberta
static bool casaAberta = false;

// Fonte das mensagens da janela da casa
static ALLEGRO_FONT *fonteCasa = NULL;

// Controla se o enigma do vaso aparece
static bool mostrarEnigmaVaso = false;

// Guarda se encontrou a primeira chave do templo
static bool encontrouChaveTemploCasa = false;

// 0 = sem resposta, 1 = correta, 2 = incorreta
static int respostaVaso = 0;

// carrega imagem da casa
bool iniciar_casa(void)
{
    imagemCasa = al_load_bitmap("assets/imagens/interiorCasa.png");
    fonteCasa = al_create_builtin_font();
    if (imagemCasa == NULL)
    {
        printf("Erro ao carregar interior da casa \n");
        return false;
    }
    if (fonteCasa == NULL)
    {
        printf("Erro ao criar fonte da casa!\n");
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
    mostrarEnigmaVaso = false;
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

    // Mostra o enigma quando o jogador clica no vaso
    if (mostrarEnigmaVaso && fonteCasa != NULL)
    {
        // Fundo marrom
        al_draw_filled_rectangle(
            200, 550, 1100, 750,
            al_map_rgb(60, 40, 25));

        // Borda dourada
        al_draw_rectangle(
            200, 550, 1100, 750,
            al_map_rgb(230, 185, 85), 3);

        // Titulo
        al_draw_text(
            fonteCasa, al_map_rgb(255, 215, 120),
            650, 575, ALLEGRO_ALIGN_CENTER,
            "ENIGMA DO VASO");

        // Pergunta
        al_draw_text(
            fonteCasa, al_map_rgb(255, 255, 255),
            650, 610, ALLEGRO_ALIGN_CENTER,
            "O que os egipcios costumavam guardar em vasos de ceramica?");

        // Alternativas
        al_draw_text(
            fonteCasa, al_map_rgb(255, 255, 255),
            650, 645, ALLEGRO_ALIGN_CENTER,
            "1 - Agua, graos e oleo");

        al_draw_text(
            fonteCasa, al_map_rgb(255, 255, 255),
            650, 670, ALLEGRO_ALIGN_CENTER,
            "2 - Aparelhos eletronicos");

        al_draw_text(
            fonteCasa, al_map_rgb(255, 255, 255),
            650, 695, ALLEGRO_ALIGN_CENTER,
            "3 - Livros impressos");

        if (respostaVaso == 1)
        {
            al_draw_text(
                fonteCasa, al_map_rgb(255, 215, 120),
                650, 725, ALLEGRO_ALIGN_CENTER,
                "Correto! Primeira chave encontrada! ENTER para fechar.");
        }
        else if (respostaVaso == 2)
        {
            al_draw_text(
                fonteCasa, al_map_rgb(255, 100, 100),
                650, 725, ALLEGRO_ALIGN_CENTER,
                "Resposta incorreta! Tente novamente.");
        }
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
    // Liberar a fonte quando encerrado
    if (fonteCasa != NULL)
    {
        al_destroy_font(fonteCasa);
        fonteCasa = NULL;
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
        mostrarEnigmaVaso = true;
    }

    // Verifica se clicou no mapa do tesouro
    else if (mouseX >= 800 && mouseX <= 1120 &&
             mouseY >= 310 && mouseY <= 450)
    {
        printf("Clicou no mapa do tesouro!\n");
    }

    //Impede clicar nos objetos enquanto o enigma estiver aberto
    if (mostrarEnigmaVaso)
    {
        return;
    }
}

void responder_enigma_casa(int tecla)
{
    // Verifica se o enigma esta aberto
    if (!casaAberta || !mostrarEnigmaVaso)
    {
        return;
    }

    //Depois de acertar, Enter fecha a pergunta
    if (respostaVaso == 1)
    {
        if (tecla == ALLEGRO_KEY_ENTER)
        {
            mostrarEnigmaVaso = false;
        }
        return;
    }

    //Resposta correta
    if (tecla == ALLEGRO_KEY_1)
    {
        respostaVaso = 1;
        encontrouChaveTemploCasa = true;
    }
    //Respostas incorretas
    else if (tecla == ALLEGRO_KEY_2 ||
             tecla == ALLEGRO_KEY_3)
    {
        respostaVaso = 2;
    }
}

