#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "curiosidades.h"

// Pontos do mundo de 2304 x 1536 (NAO sao coordenadas de tela).
// Para ajustar as descobertas, altere os quatro numeros de cada linha:
// centro X, centro Y, alcance horizontal, alcance vertical.
typedef struct {
    float x, y, alcanceX, alcanceY;
    const char *caminho;
    const char *titulo;
    const char *texto;
    ALLEGRO_BITMAP *imagem;
    bool descoberta;
} Descoberta;

static Descoberta descobertas[] = {
    {
        380.0f, 390.0f, 240.0f, 170.0f,
        "assets/imagens/piramides.png",
        "AS PIRÂMIDES DO EGITO",
        "As pirâmides de Gizé foram construídas como monumentos funerários "
        "para faraós. Elas são um marco da engenharia do Egito Antigo.",
        NULL, false
    },
    {
        1810.0f, 340.0f, 220.0f, 130.0f,
        "assets/imagens/nilo.png",
        "O RIO NILO",
        "O Rio Nilo era uma importante via de transporte no Egito Antigo. "
        "Barcos levavam pessoas e mercadorias entre cidades e povoados.",
        NULL, false
    }
};

#define TOTAL_DESCOBERTAS ((int)(sizeof(descobertas) / sizeof(descobertas[0])))

static ALLEGRO_FONT *fonteReserva = NULL;
static int indiceAberto = -1;
static bool fechando = false;
static float opacidadePainel = 0.0f;
static float opacidadeAviso = 0.0f;
static float tempoPulso = 0.0f;

static float limitar(float valor, float minimo, float maximo)
{
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

static int descoberta_perto(float jogadorX, float jogadorY)
{
    for (int i = 0; i < TOTAL_DESCOBERTAS; i++) {
        Descoberta *d = &descobertas[i];
        if (!d->descoberta && d->imagem != NULL &&
            fabsf(jogadorX - d->x) <= d->alcanceX &&
            fabsf(jogadorY - d->y) <= d->alcanceY) {
            return i;
        }
    }
    return -1;
}

bool iniciar_curiosidades(void)
{
    bool carregouTudo = true;
    indiceAberto = -1;
    fechando = false;
    opacidadePainel = 0.0f;
    opacidadeAviso = 0.0f;
    tempoPulso = 0.0f;

    fonteReserva = al_create_builtin_font();
    if (fonteReserva == NULL) {
        fprintf(stderr, "Curiosidades: nao foi possivel criar fonte de reserva.\n");
    }

    for (int i = 0; i < TOTAL_DESCOBERTAS; i++) {
        descobertas[i].descoberta = false;
        descobertas[i].imagem = al_load_bitmap(descobertas[i].caminho);
        if (descobertas[i].imagem == NULL) {
            fprintf(stderr, "Curiosidades: imagem ausente: %s\n", descobertas[i].caminho);
            carregouTudo = false;
        }
    }
    // Se faltar alguma arte, as outras ainda funcionam.
    return carregouTudo;
}

bool curiosidades_aberta(void)
{
    return indiceAberto >= 0;
}

void atualizar_curiosidades(float jogadorX, float jogadorY, bool mapaLivre)
{
    tempoPulso += 1.0f / 60.0f;

    if (indiceAberto >= 0) {
        opacidadeAviso = 0.0f;
        if (fechando) {
            opacidadePainel = limitar(opacidadePainel - 0.08f, 0.0f, 1.0f);
            if (opacidadePainel <= 0.0f) {
                indiceAberto = -1;
                fechando = false;
            }
        } else {
            opacidadePainel = limitar(opacidadePainel + 0.08f, 0.0f, 1.0f);
        }
        return;
    }

    // Surge e desaparece suavemente perto de um ponto nao descoberto.
    if (mapaLivre && descoberta_perto(jogadorX, jogadorY) >= 0) {
        opacidadeAviso = limitar(opacidadeAviso + 0.06f, 0.0f, 1.0f);
    } else {
        opacidadeAviso = limitar(opacidadeAviso - 0.06f, 0.0f, 1.0f);
    }
}

bool curiosidades_tecla(int tecla, float jogadorX, float jogadorY, bool mapaLivre)
{
    // Enquanto a ilustraçao estiver aberta, nao repasse teclas ao resto do jogo.
    if (indiceAberto >= 0) {
        if (tecla == ALLEGRO_KEY_E || tecla == ALLEGRO_KEY_ESCAPE) {
            fechando = true;
        }
        return true;
    }

    if (tecla != ALLEGRO_KEY_E || !mapaLivre) {
        return false;
    }

    int i = descoberta_perto(jogadorX, jogadorY);
    if (i < 0) return false;

    indiceAberto = i;
    descobertas[i].descoberta = true; // Uma vez por partida.
    fechando = false;
    opacidadePainel = 0.0f;
    opacidadeAviso = 0.0f;
    return true;
}

void desenhar_curiosidades(ALLEGRO_FONT *fonte, bool mapaLivre)
{
    ALLEGRO_FONT *f = fonte ? fonte : fonteReserva;
    if (f == NULL) return;

    if (indiceAberto >= 0) {
        const Descoberta *d = &descobertas[indiceAberto];
        int a = (int)(255.0f * opacidadePainel);

        // Escurecimento atras do painel.
        al_draw_filled_rectangle(0, 0, 1300, 800,
            al_map_rgba(0, 0, 0, (int)(220.0f * opacidadePainel)));

        // Ilustraçao horizontal, sem distorcer a proporçao 16:9.
        if (d->imagem != NULL) {
            al_draw_tinted_scaled_bitmap(d->imagem,
                al_map_rgba(255, 255, 255, a),
                0, 0,
                al_get_bitmap_width(d->imagem),
                al_get_bitmap_height(d->imagem),
                110, 53, 1080, 608, 0);
        }

        // Moldura fina da ilustraçao.
        al_draw_rectangle(109, 52, 1191, 662,
            al_map_rgba(233, 187, 107, a), 3.0f);

        // Legenda marrom sobreposta ao rodape da arte.
        al_draw_filled_rounded_rectangle(
            110, 606, 1190, 750, 9, 9,
            al_map_rgba(45, 29, 20, (int)(240.0f * opacidadePainel)));
        al_draw_rounded_rectangle(
            110, 606, 1190, 750, 9, 9,
            al_map_rgba(231, 185, 106, a), 2.0f);

        al_draw_text(f, al_map_rgba(255, 219, 143, a),
            135, 618, 0, d->titulo);
        al_draw_multiline_text(f, al_map_rgba(255, 244, 225, a),
            135, 655, 1010, 28, 0, d->texto);
        al_draw_text(f, al_map_rgba(255, 208, 125, a),
            1160, 720, ALLEGRO_ALIGN_RIGHT, "[E] Continuar  |  [ESC] Fechar");
        return;
    }

    // Aviso do ponto de descoberta no mesmo estilo dourado do E atual.
    if (!mapaLivre || opacidadeAviso <= 0.0f) return;
    float pulso = (sinf(tempoPulso * 4.0f) + 1.0f) / 2.0f;
    int alphaFundo = (int)(210 * opacidadeAviso);
    int alphaBorda = (int)((150 + 105 * pulso) * opacidadeAviso);
    int alphaTexto = (int)((190 + 65 * pulso) * opacidadeAviso);

    al_draw_filled_rounded_rectangle(530, 30, 770, 75, 10, 10,
        al_map_rgba(60, 40, 25, alphaFundo));
    al_draw_rounded_rectangle(530, 30, 770, 75, 10, 10,
        al_map_rgba(255, 215, 120, alphaBorda), 2.5f);
    al_draw_text(f, al_map_rgba(255, 215, 120, alphaTexto),
        650, 38, ALLEGRO_ALIGN_CENTER, "[E] Investigar");
}

void liberar_curiosidades(void)
{
    for (int i = 0; i < TOTAL_DESCOBERTAS; i++) {
        if (descobertas[i].imagem != NULL) {
            al_destroy_bitmap(descobertas[i].imagem);
            descobertas[i].imagem = NULL;
        }
    }
    if (fonteReserva != NULL) {
        al_destroy_font(fonteReserva);
        fonteReserva = NULL;
    }
}
