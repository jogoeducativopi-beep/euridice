#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "personagem.h"
#include "colisao.h"

// Imagem do personagem // Ponteiro para armazenar a imagem do personagem //posições das imagens
static ALLEGRO_BITMAP *frente = NULL;
static ALLEGRO_BITMAP *costa = NULL;
static ALLEGRO_BITMAP *direita = NULL;
static ALLEGRO_BITMAP *esquerda = NULL;

// guarda qual imagem sera desenhada//vai apontar qual imagem sera desenhada
static ALLEGRO_BITMAP *imagemAtual = NULL;

// Posição inicial do personagem no mapa
static float x = 1090;
static float y = 1390;

// Tamanho do personagem no mapa
static float largura = 50;
static float altura = 90;

// Vetor que guarda o estado das teclas
static bool teclas[ALLEGRO_KEY_MAX] = {false}; // static vetor restrito aquele arquivo; key max posições que representam as teclas; bool true tecla pressionada, false solta

bool carregar_personagem_mapa(void)
{
    frente = al_load_bitmap("assets/imagens/personagemFrente.png"); // tenta carregar imagem do disco para memoria
    costa = al_load_bitmap("assets/imagens/personagemCostas.png");
    direita = al_load_bitmap("assets/imagens/personagemDireita.png");
    esquerda = al_load_bitmap("assets/imagens/personagemEsquerda.png");

    // verifica se todas as imagens foram carregadas
    if (!frente || !costa || !direita || !esquerda)
    {
        printf("Erro ao carregar imagens do personagem!\n");
        liberar_personagem_mapa();
        return false;
    }
    // personagem olhando para frente
    imagemAtual = frente;
    printf("Personagem Carregado");
    return true;
}
// Guarda quando uma tecla é pressionada ou solta
void personagem_tecla(int tecla, bool pressionada)
{
    if (tecla >= 0 && tecla < ALLEGRO_KEY_MAX)
    {
        teclas[tecla] = pressionada;
    }

    if (pressionada)
    {
        if (tecla == ALLEGRO_KEY_RIGHT)
        {
            imagemAtual = direita;
        }
        else if (tecla == ALLEGRO_KEY_LEFT)
        {
            imagemAtual = esquerda;
        }
        else if (tecla == ALLEGRO_KEY_UP)
        {
            imagemAtual = costa;
        }
        else if (tecla == ALLEGRO_KEY_DOWN)
        {
            imagemAtual = frente;
        }
    }
}

// Atualizar a posição do personagem e imagem
void atualizar_posicao_personagem(int largura_mapa, int altura_mapa)
{
    float velocidade = 3;

    // guarda a posição anterior
    float xAnterior = x;
    float yAnterior = y;

    // movimento horizontal
    if (teclas[ALLEGRO_KEY_RIGHT]) // direita
    {
        x += velocidade;
    }
    if (teclas[ALLEGRO_KEY_LEFT]) // esquerda
    {
        x -= velocidade;
    }

    // Movimento vertical
    if (teclas[ALLEGRO_KEY_UP])
    {
        y -= velocidade;
    }
    if (teclas[ALLEGRO_KEY_DOWN])
    {
        y += velocidade;
    }

    // Limites horizontais
    if (x < 0)
    {
        x = 0;
    }

    if (x + largura > largura_mapa)
    {
        x = largura_mapa - largura;
    }

    // Limites verticais
    if (y < 0)
    {
        y = 0;
    }

    if (y + altura > altura_mapa)
    {
        y = altura_mapa - altura;
    }

    // Se bater na parede, volta para a posição anterior
    if (posicao_bloqueada(x, y, largura, altura))
    {
        x = xAnterior;
        y = yAnterior;

        //guarda onde o personagem estava, verifica se estar bloqueado e se sim volta para onde estava
    }
}
void desenha_personagem(float cameraX, float cameraY)
{
    if (imagemAtual != NULL)
    {
        al_draw_scaled_bitmap(
            imagemAtual,
            0, 0,
            al_get_bitmap_width(imagemAtual),
            al_get_bitmap_height(imagemAtual),
            x - cameraX, y - cameraY, // onde o personagem deve aparecer
            largura, altura,
            0);
    }
}
void liberar_personagem_mapa(void)
{
    if (frente != NULL)
    {
        al_destroy_bitmap(frente);
    }
    if (costa != NULL)
    {
        al_destroy_bitmap(costa);
    }
    if (direita != NULL)
    {
        al_destroy_bitmap(direita);
    }
    if (esquerda != NULL)
    {
        al_destroy_bitmap(esquerda);
    }
    frente = NULL;
    costa = NULL;
    direita = NULL;
    esquerda = NULL;
    imagemAtual = NULL;
}
float personagem_centro_x(void)
{
    return x + largura / 2;
}

float personagem_centro_y(void)
{
    return y + altura / 2;
}
// manter ele no centro da camera