#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "templo.h"
#include "missaoCasa.h"
#include "interacao.h"
#include "missaoRuinas.h"

// Etapas:
// 0 = Templo fechado
// 1 = Pergunta da Esfinge
// 2 = Imagem final do museu

static int etapaTemplo = 0;

// Imagens da ultima fase
static ALLEGRO_BITMAP *imagemPergunta = NULL;
static ALLEGRO_BITMAP *imagemMuseu = NULL;

// Verifica se encontrou as tres chaves
void verificar_chaves_templo(void)
{
    if (encontrou_chave_templo_casa() &&
        encontrou_chave_oasis() &&
        encontrou_terceira_chave())
    {
        printf("Todas as 3 chaves foram encontradas!\n");
        printf("O templo esta destrancado!\n");
    }
    else
    {
        printf("Ainda faltam chaves para abrir o templo.\n");
    }
}

// Retorna verdadeiro quando tem as tres chaves
bool templo_destrancado(void)
{
    return encontrou_chave_templo_casa() &&
           encontrou_chave_oasis() &&
           encontrou_terceira_chave();
}

// Abre a pergunta quando o jogador entra
void iniciar_templo(void)
{
    if (etapaTemplo != 0)
    {
        return;
    }

    if (!templo_destrancado())
    {
        return;
    }

    // Carrega as duas imagens
    imagemPergunta = al_load_bitmap(
        "assets/imagens/esfinge_enigma.png");

    imagemMuseu = al_load_bitmap(
        "assets/imagens/euridice_museu.png");

    if (imagemPergunta == NULL || imagemMuseu == NULL)
    {
        printf("Erro ao carregar as imagens do templo!\n");

        fechar_templo();
        return;
    }

    // Comeca mostrando a pergunta
    etapaTemplo = 1;

    printf("Enigma da Esfinge iniciado!\n");
    printf("Escolha 1, 2 ou 3.\n");
}

// Indica se o jogador esta na fase final
bool templo_ativo(void)
{
    return etapaTemplo != 0;
}

// Indica se chegou a imagem do museu
bool templo_finalizado(void)
{
    return etapaTemplo == 2;
}

// Recebe as respostas pelo teclado
void responder_enigma_esfinge(int tecla)
{
    // A pergunta precisa estar aberta
    if (etapaTemplo != 1)
    {
        return;
    }

    if (tecla == ALLEGRO_KEY_2)
    {
        // Resposta correta: ser humano
        printf("Resposta correta! Ser humano!\n");

        etapaTemplo = 2;
    }
    else if (tecla == ALLEGRO_KEY_1 ||
             tecla == ALLEGRO_KEY_3)
    {
        // Continua mostrando a pergunta
        printf("Resposta incorreta! Tente novamente.\n");
    }
}

// Desenha a imagem da etapa atual
void desenhar_templo(void)
{
    ALLEGRO_BITMAP *imagemAtual = NULL;

    if (etapaTemplo == 1)
    {
        imagemAtual = imagemPergunta;
    }
    else if (etapaTemplo == 2)
    {
        imagemAtual = imagemMuseu;
    }

    if (imagemAtual == NULL)
    {
        return;
    }

    al_draw_scaled_bitmap(
        imagemAtual,
        0, 0,
        al_get_bitmap_width(imagemAtual),
        al_get_bitmap_height(imagemAtual),
        0, 0,
        1300, 800,
        0);
}
// Libera as imagens e fecha a fase
void fechar_templo(void)
{
    if (imagemPergunta != NULL)
    {
        al_destroy_bitmap(imagemPergunta);
        imagemPergunta = NULL;
    }

    if (imagemMuseu != NULL)
    {
        al_destroy_bitmap(imagemMuseu);
        imagemMuseu = NULL;
    }

    etapaTemplo = 0;
}
