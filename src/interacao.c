#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h> //permite escrever mensagem na tela
#include <allegro5/allegro_primitives.h>
#include "interacao.h"

// ponteiro que guarda a referencia da fonte utilizada para escrever
static ALLEGRO_FONT *fonteAviso = NULL;

// controla a abertura do aviso da casa
static bool mostrarAvisoCasa = false;

// Controla a abertura do aviso do mercado
static bool mostrarAvisoMercado = false;

// inicia a fonte que sera usada nas mensagens
bool iniciar_interacao(void)
{
    // cria fonte simples
    fonteAviso = al_create_builtin_font();

    // verifica se a fonte foi criada
    if (fonteAviso == NULL)
    {
        return false;
    }
    return true;
}

void interacao_tecla(int tecla, float jogadorX, float jogadorY)
{
    // Verifica se a tecla pressionada foi E
    if (tecla == ALLEGRO_KEY_E)
    {
        // Se a mensagem estiver aberta, fecha
        if (mostrarAvisoCasa)
        {
            mostrarAvisoCasa = false;
        }

        // Fecha o aviso do mercado
        else if (mostrarAvisoMercado)
        {
            mostrarAvisoMercado = false;
        }
        else
        {
            // Verifica se o jogador esta perto da porta da casa
            if (jogadorX >= 480 && jogadorX <= 660 &&
                jogadorY >= 1300 && jogadorY <= 1450)
            {
                mostrarAvisoCasa = true;
            }

            // Personagem proximo da tenda do mercado
            else if (jogadorX >= 1580 && jogadorX <= 1980 &&
                     jogadorY >= 760 && jogadorY <= 890)
            {
                mostrarAvisoMercado = true;
            }
        }
    }
}

void desenhar_interacao(void)
{
    // Verifica se o aviso da cada esta aberto
    if (mostrarAvisoCasa)
    {
        // Desenha o fundo da caixa de mensagem
        al_draw_filled_rectangle(
            200, 600, 1100, 750,
            al_map_rgb(60, 40, 25));

        // Desenha a borda dourada
        al_draw_rectangle(
            200, 600, 1100, 750,
            al_map_rgb(230, 185, 85),
            3);

        // Escreve o titulo
        al_draw_text(
            fonteAviso,
            al_map_rgb(255, 215, 120),
            650, 630,
            ALLEGRO_ALIGN_CENTER,
            "CASA EGIPCIA");

        // Escreve a mensagem
        al_draw_text(
            fonteAviso,
            al_map_rgb(255, 255, 255),
            650, 675,
            ALLEGRO_ALIGN_CENTER,
            "A porta esta trancada. Procure uma pista no mercado.");

        // Instrucao para fechar
        al_draw_text(
            fonteAviso,
            al_map_rgb(255, 255, 255),
            650, 715,
            ALLEGRO_ALIGN_CENTER,
            "Pressione E para fechar");
    }
}
void liberar_interacao(void)
{
    // Verifica se a fonte foi criada
    if (fonteAviso != NULL)
    {
        // Libera a fonte da memoria
        al_destroy_font(fonteAviso);

        // Indica que o ponteiro nao aponta mais para uma fonte
        fonteAviso = NULL;
    }
}
