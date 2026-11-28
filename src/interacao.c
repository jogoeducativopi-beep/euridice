#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h> //permite escrever mensagem na tela
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "interacao.h"
#include "missaoMercado.h"
#include "missaoCasa.h"
#include "missaoRuinas.h"
#include "templo.h"

// ponteiro que guarda a referencia da fonte utilizada para escrever
static ALLEGRO_FONT *fonteAviso = NULL;

// controla a abertura do aviso da casa
static bool mostrarAvisoCasa = false;

// Controla a abertura do aviso do mercado
static bool mostrarAvisoMercado = false;

// Controla abertura do aviso do lago
static bool mostrarAvisoLago = false;

// Controla abertura do aviso do templo
static bool mostrarAvisoTemplo = false;

// Guarda se o jogador encontrou a segunda chave do templo
static bool chaveOasisEncontrada = false;

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
    // Nao executa interacoes do mapa enquanto estiver na casa
    if (casa_aberta())
    {
        return;
    }

    // Se o enigma estiver aberto, recebe as respostas
    if (missao_ruinas_aberta())
    {
        missao_ruinas_tecla(tecla, jogadorX, jogadorY);
        return;
    }

    // Permite investigar as ruinas ao apertar E
    if (tecla == ALLEGRO_KEY_E &&
        missao_ruinas_perto(jogadorX, jogadorY) &&
        !interacao_aberta() &&
        !mercado_aberto())
    {
        missao_ruinas_tecla(tecla, jogadorX, jogadorY);
        return;
    }

    // Enter abre a casa somente se estiver destrancada
    if (tecla == ALLEGRO_KEY_ENTER &&
        mostrarAvisoCasa &&
        encontrou_chave_casa())
    {
        mostrarAvisoCasa = false;
        abrir_casa();
        return;
    }

    // Enter inicia a cena da Esfinge se tiver as tres chaves
    if (tecla == ALLEGRO_KEY_ENTER &&
        mostrarAvisoTemplo &&
        templo_destrancado())
    {
        mostrarAvisoTemplo = false;

        printf("Entrando no templo!\n");

        reproduzir_video_esfinge();
        return;
    }

    // se a mensagem estiver aberta o enter entra no mercado
    if (tecla == ALLEGRO_KEY_ENTER && mostrarAvisoMercado)
    {
        mostrarAvisoMercado = false; // fecha o aviso
        abrir_mercado();             // abre a imagem
        return;
    }
    // Se nao apertou E, nao continua a interacao
    if (tecla != ALLEGRO_KEY_E)
    {
        return; // se for diferente de E, sai da função
    }

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
        else if (mostrarAvisoLago)
        {
            mostrarAvisoLago = false;
        }
        else if (mostrarAvisoTemplo)
        {
            mostrarAvisoTemplo = false;
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

            // Personagem proximo do oasis
            else if (jogadorX >= 490 && jogadorX <= 740 &&
                     jogadorY >= 850 && jogadorY <= 955)
            {
                mostrarAvisoLago = true;
                // So encontra a segunda chave se
                // concluiu a casa e encontrou o mapa
                if (encontrou_chave_templo_casa() &&
                    encontrou_mapa_oasis())
                {
                    chaveOasisEncontrada = true;
                }
            }
            // Personagem proximo ao templo
            else if (jogadorX >= 1080 && jogadorX <= 1235 &&
                     jogadorY >= 350 && jogadorY <= 470)
            {
                mostrarAvisoTemplo = true;
                verificar_chaves_templo();
            }
        }
    }
}

bool interacao_aberta(void)
{
    return mostrarAvisoCasa || mostrarAvisoMercado || mostrarAvisoLago || mostrarAvisoTemplo || missao_ruinas_aberta();
}

// Esse comando verifica se o personagem esta perto de um algum local
bool jogador_perto_interacao(float jogadorX, float jogadorY)
{

    if (missao_ruinas_perto(jogadorX, jogadorY))
    {
        return true;
    }

    // Casa egipcia
    if (jogadorX >= 480 && jogadorX <= 660 &&
        jogadorY >= 1300 && jogadorY <= 1450)
    {
        return true;
    }

    // Mercado egipcio
    if (jogadorX >= 1580 && jogadorX <= 1980 &&
        jogadorY >= 760 && jogadorY <= 890)
    {
        return true;
    }

    // Oasis
    if (jogadorX >= 490 && jogadorX <= 740 &&
        jogadorY >= 850 && jogadorY <= 955)
    {
        return true;
    }

    // Templo
    if (jogadorX >= 1080 && jogadorX <= 1235 &&
        jogadorY >= 350 && jogadorY <= 470)
    {
        return true;
    }

    return false;
}

void desenhar_interacao(void)
{
    // Guarda os textos que serao exibidos
    const char *titulo;
    const char *mensagem1;
    const char *mensagem2;

    // Verifica se o aviso da cada esta aberto
    if (mostrarAvisoCasa)
    {
        titulo = "CASA EGIPCIA";

        if (encontrou_chave_casa())
        {
            mensagem1 = "A porta esta destrancada!";
            mensagem2 = "Pressione ENTER para entrar.";
        }
        else
        {
            mensagem1 = "A porta esta trancada.";
            mensagem2 = "Procure pistas no mercado.";
        }
    }
    else if (mostrarAvisoMercado)
    {
        titulo = "MERCADO EGIPCIO";
        mensagem1 = "Examine os objetos para descobrir o segredo.";
        mensagem2 = "Aperte ENTER para entrar no mercado";
    }
    else if (mostrarAvisoLago)
    {
        titulo = "Oasis";
        if (chaveOasisEncontrada)
        {
            mensagem1 = "O mapa revelou onde estar o tesouso.";
            mensagem2 = "Encontrou a segunda chave do templo.";
        }
        else
        {
            mensagem1 = "Voce ainda nao sabe onde esta o tesouro.";
            mensagem2 = "Procure um mapa dentro da casa egipcia.";
        }
    }
    else if (mostrarAvisoTemplo)
    {
        titulo = "Templo Egipcio";
        if (templo_destrancado())
        {
            mensagem1 = "As tres chaves foram encontradas!";
            mensagem2 = "Porta destrancada! Pressione ENTER para entrar.";
        }
        else
        {

            mensagem1 = "A porta esta trancada conclua as missoes";
            mensagem2 = "Explore o mapa";
        }
    }
    else
    {
        return;
    }
    if (fonteAviso == NULL)
        return;
    // fundo da caixa de dialogo
    al_draw_filled_rectangle(
        200, 600, 1100, 750,
        al_map_rgb(60, 40, 25));

    // Desenha a borda dourada
    al_draw_rectangle(
        200, 600, 1100, 750,
        al_map_rgb(230, 185, 85),
        3);

    // titulo
    al_draw_text(
        fonteAviso,
        al_map_rgb(255, 215, 120),
        650, 630,
        ALLEGRO_ALIGN_CENTER,
        titulo);

    // mensagem1
    al_draw_text(
        fonteAviso,
        al_map_rgb(255, 255, 255),
        650, 655,
        ALLEGRO_ALIGN_CENTER,
        mensagem1);

    // mensagem2
    al_draw_text(
        fonteAviso,
        al_map_rgb(255, 255, 255),
        650, 680,
        ALLEGRO_ALIGN_CENTER,
        mensagem2);

    // Pista para a proxima missao
    if (mostrarAvisoLago && chaveOasisEncontrada)
    {
        al_draw_text(
            fonteAviso,
            al_map_rgb(255, 215, 120),
            650, 700,
            ALLEGRO_ALIGN_CENTER,
            "Na chave, ha o desenho de um antigo arco de pedra...");
    }

    // Instrucao para fechar
    al_draw_text(
        fonteAviso,
        al_map_rgb(255, 255, 255),
        650, 730,
        ALLEGRO_ALIGN_CENTER,
        "Pressione E para fechar");
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
bool encontrou_chave_oasis(void)
{
    return chaveOasisEncontrada;
}

bool tentar_entrar_templo(int tecla)
{
    if (tecla == ALLEGRO_KEY_ENTER &&
        mostrarAvisoTemplo &&
        templo_destrancado())
    {
        mostrarAvisoTemplo = false;
        return true;
    }

    return false;
}