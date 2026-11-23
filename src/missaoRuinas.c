
#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "missaoRuinas.h"
#include "missaoMercado.h"
#include "missaoCasa.h"
#include "interacao.h"

// Fonte utilizada na missao
static ALLEGRO_FONT *fonteRuinas = NULL;

// Estado da janela
static bool ruinasAberta = false;

// Guarda se a terceira chave foi encontrada
static bool terceiraChaveEncontrada = false;

// Controla mensagens de erro
static bool respostaErrada = false;

// INICIALIZACAO
bool iniciar_missao_ruinas(void)
{
    fonteRuinas = al_create_builtin_font();

    if (fonteRuinas == NULL)
    {
        printf("Erro ao criar fonte das ruinas!\n");
        return false;
    }

    return true;
}

// PROXIMIDADE DAS RUINAS
bool missao_ruinas_perto(float jogadorX, float jogadorY)
{
    return jogadorX >= 1600 && jogadorX <= 2050 &&
           jogadorY >= 1000 && jogadorY <= 1450;
}

// ESTADO DA MISSAO
bool missao_ruinas_aberta(void)
{
    return ruinasAberta;
}

// VERIFICA SE O JOGADOR JA PODE ACESSAR AS RUINAS
static bool ruinas_liberadas(void)
{
    return encontrou_chave_casa() &&
           encontrou_chave_templo_casa() &&
           encontrou_mapa_oasis() &&
           encontrou_chave_oasis();
}


bool encontrou_terceira_chave(void)
{
    return terceiraChaveEncontrada;
}

void fechar_missao_ruinas(void)
{
    ruinasAberta = false;
    respostaErrada = false;
}

// TECLAS E RESPOSTAS
void missao_ruinas_tecla(
    int tecla, float jogadorX, float jogadorY)
{
    // Se a janela estiver fechada
    if (!ruinasAberta)
    {
        if (tecla == ALLEGRO_KEY_E &&
            missao_ruinas_perto(jogadorX, jogadorY))
        {
            ruinasAberta = true;
            respostaErrada = false;

            printf("Investigando as ruinas!\n");
        }

        return;
    }

    // E fecha a janela
    if (tecla == ALLEGRO_KEY_E)
    {
        fechar_missao_ruinas();
        return;
    }

    // Se ainda faltam missoes, nao permite responder
    if (!ruinas_liberadas())
    {
        return;
    }

    // Missao ja concluida
    if (terceiraChaveEncontrada)
    {
        if (tecla == ALLEGRO_KEY_ENTER)
        {
            fechar_missao_ruinas();
        }
        return;
    }

    // Resposta correta
    if (tecla == ALLEGRO_KEY_1)
    {
        terceiraChaveEncontrada = true;
        respostaErrada = false;

        printf("TERCEIRA CHAVE ENCONTRADA!\n");
    }

    // Respostas incorretas
    else if (tecla == ALLEGRO_KEY_2 ||
             tecla == ALLEGRO_KEY_3)
    {
        respostaErrada = true;
    }
}

// DESENHAR A MISSAO

void desenhar_missao_ruinas(void)
{
    if (!ruinasAberta || fonteRuinas == NULL)
        return;

    // Fundo marrom, igual as outras interacoes
    al_draw_filled_rectangle(
        200, 540, 1100, 750,
        al_map_rgb(60, 40, 25)
    );

    // Borda dourada
    al_draw_rectangle(
        200, 540, 1100, 750,
        al_map_rgb(230, 185, 85),
        3
    );

    // Titulo
    al_draw_text(
        fonteRuinas,
        al_map_rgb(255, 215, 120),
        650, 560,
        ALLEGRO_ALIGN_CENTER,
        "O ENIGMA DAS RUINAS"
    );

    if (!ruinas_liberadas())
    {
        const char *mensagem;

        if (!encontrou_chave_casa())
        {
            mensagem = "Primeiro investigue o mercado egipcio.";
        }
        else if (!encontrou_chave_templo_casa() ||
                 !encontrou_mapa_oasis())
        {
            mensagem = "Resolva o enigma e encontre o mapa na casa.";
        }
        else
        {
            mensagem = "Encontre a segunda chave no oasis.";
        }

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 215, 120),
            650, 605,
            ALLEGRO_ALIGN_CENTER,
            "AS RUINAS ESTAO SELADAS!"
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 255, 255),
            650, 640,
            ALLEGRO_ALIGN_CENTER,
            mensagem
        );
    }
    else if (terceiraChaveEncontrada)
    {
        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 215, 120),
            650, 605,
            ALLEGRO_ALIGN_CENTER,
            "RESPOSTA CORRETA!"
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 255, 255),
            650, 640,
            ALLEGRO_ALIGN_CENTER,
            "Voce encontrou a terceira chave do templo!"
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 215, 120),
            650, 675,
            ALLEGRO_ALIGN_CENTER,
            "Pressione ENTER para continuar"
        );
    }
    else
    {
        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 255, 255),
            650, 590,
            ALLEGRO_ALIGN_CENTER,
            "Uma inscricao antiga protege a ultima chave."
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 215, 120),
            650, 615,
            ALLEGRO_ALIGN_CENTER,
            "Qual era uma das principais funções das pirâmides no Egito Antigo?"
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 255, 255),
            650, 645,
            ALLEGRO_ALIGN_CENTER,
            "1 - Servir como túmulos de faraós"
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 255, 255),
            650, 665,
            ALLEGRO_ALIGN_CENTER,
            "2 - Funcionar como escolas"
        );

        al_draw_text(
            fonteRuinas,
            al_map_rgb(255, 255, 255),
            650, 685,
            ALLEGRO_ALIGN_CENTER,
            "3 - Armazenar alimentos"
        );

        if (respostaErrada)
        {
            al_draw_text(
                fonteRuinas,
                al_map_rgb(255, 100, 100),
                650, 705,
                ALLEGRO_ALIGN_CENTER,
                "Resposta incorreta! Tente novamente."
            );
        }
    }

    // Instrucao para fechar
    al_draw_text(
        fonteRuinas,
        al_map_rgb(255, 255, 255),
        650, 730,
        ALLEGRO_ALIGN_CENTER,
        "Pressione E ou ESC para fechar"
    );
}

// LIBERAR MEMORIA
void liberar_missao_ruinas(void)
{
    if (fonteRuinas != NULL)
    {
        al_destroy_font(fonteRuinas);
        fonteRuinas = NULL;
    }
}
