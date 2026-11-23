#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h> //permite escrever mensagem na tela
#include <allegro5/allegro_primitives.h>
#include "interacao.h"
#include "missaoMercado.h"
#include "missaoCasa.h"

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

    // Enter abre a casa somente se estiver destrancada
    if (tecla == ALLEGRO_KEY_ENTER &&
        mostrarAvisoCasa &&
        encontrou_chave_casa())
    {
        mostrarAvisoCasa = false;
        abrir_casa();
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
            // Personagem proximo do lago
            else if (jogadorX >= 490 && jogadorX <= 740 &&
                     jogadorY >= 850 && jogadorY <= 955)
            {
                mostrarAvisoLago = true;
            }
            // Personagem proximo ao templo
            else if (jogadorX >= 1080 && jogadorX <= 1235 &&
                     jogadorY >= 350 && jogadorY <= 470)
            {
                mostrarAvisoTemplo = true;
            }
        }
    }
}

bool interacao_aberta(void)
{
    return mostrarAvisoCasa || mostrarAvisoMercado || mostrarAvisoLago || mostrarAvisoTemplo;
}

// Esse comando verifica se o personagem esta perto de um algum local
bool jogador_perto_interacao(float jogadorX, float jogadorY)
{
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
        mensagem1 = "Descubra os segredos do Oasis";
        mensagem2 = "Escave o Tesouso do Oasis";
    }
    else if (mostrarAvisoTemplo)
    {
        titulo = "Templo Egipcio";
        mensagem1 = "A porta esta trancada conclua as missoes";
        mensagem2 = "Explore o mapa";
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

    // Instrucao para fechar
    al_draw_text(
        fonteAviso,
        al_map_rgb(255, 255, 255),
        650, 715,
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
