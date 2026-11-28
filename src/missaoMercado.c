#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "missaoMercado.h"
#include <allegro5/allegro_primitives.h>
#define TOTAL_PECAS 4
#include <allegro5/allegro_font.h>

// ponteiro que guarda a imagem da ativação do mercado
static ALLEGRO_BITMAP *imagemMercado = NULL;
// Imagem do pedaço da chave que esta no vaso
static ALLEGRO_BITMAP *ImagemPeçaVaso = NULL;
// verifica se tela da imagem do mercado esta aberta

static ALLEGRO_BITMAP *ImagemPeçaPapiro = NULL;
static ALLEGRO_BITMAP *ImagemPeçaTinteiro = NULL;
static ALLEGRO_BITMAP *ImagemPeçaCaixote = NULL;

static ALLEGRO_FONT *fonteMercado = NULL;

static bool mercadoAberto = false;
// Guarda se o jogador encontrou do vaso
static bool encontrouPecaVaso = false;
// Gurada se o jogador encontro a peça do papiro
static bool encontrouPecaPapiro;
// Guarda se o jogador encontrou a peça do tinteiro
static bool encontrouPecaTinteiro;
// Guarda se o jogador encontrou a peça do caixote
static bool encontrouPecaCaixote;

// Verifique se a chave foi totalmente montada
static bool casaDestrancada = false;

// controla a mensagem de missão conluida
static bool mostrarAvisoDestrancada = false;

// Posição das peças nos 4 quadrados
static int quadradoX[TOTAL_PECAS] = {857, 954, 857, 954};
static int quadradoY[TOTAL_PECAS] = {312, 312, 412, 412};

// Posição das peças na silhueta da chave
static int encaixeX[TOTAL_PECAS] = {739, 764, 755, 756};
static int encaixeY[TOTAL_PECAS] = {318, 340, 388, 440};

// guarda as peças que foram encaixadas
static bool pecaEncaixada[TOTAL_PECAS] = {false};

// qual peça esta sendo arrastada
static int pecaArrastada = -1;

// posição do mouse durante o arraste
static int mouseArrasteX = 0;
static int mouseArrasteY = 0;

bool inicia_mercado(void)
{
    // carrega imagem do mercado
    imagemMercado = al_load_bitmap("assets/imagens/mercado.png");

    // Carrega a imagem da peca da lua
    ImagemPeçaVaso = al_load_bitmap("assets/imagens/pecaVaso.png");

    ImagemPeçaPapiro = al_load_bitmap("assets/imagens/pecaPapiro.png");
    ImagemPeçaTinteiro = al_load_bitmap("assets/imagens/pecaTinteiro.png");
    ImagemPeçaCaixote = al_load_bitmap("assets/imagens/pecaCaixote.png");

    fonteMercado = al_create_builtin_font();

    if (ImagemPeçaVaso == NULL)
    {
        printf("Erro ao carregar a peca do vaso!\n");
        return false;
    }

    if (ImagemPeçaPapiro == NULL)
    {
        printf("Erro ao carregar a peca do papiro!\n");
        return false;
    }
    if (ImagemPeçaTinteiro == NULL)
    {
        printf("Erro ao carregar a peca do tinteiro!\n");
        return false;
    }
    if (ImagemPeçaCaixote == NULL)
    {
        printf("Erro ao carregar a peça do caixote!\n");
        return false;
    }

    // verfica se a imagem foi carregada
    if (imagemMercado == NULL)
    {
        printf("Erro ao carregar imagem do mercado");
        return false;
    }

    if (fonteMercado == NULL)
    {
        printf("Erro ao carregar fonte do mercado!\n");
        return false;
    }

    printf("Imagens Carregadas");
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
    // se o mercado estiver fechado ou a imagem nao existir ele fecha a função sem desenhar
    if (!mercadoAberto || imagemMercado == NULL)
    {
        return;
    }
    // verifica se a imagem foi carregada e o mercado aberto
    if (mercadoAberto && imagemMercado != NULL) // tel do mercado aberta e imagem mercado doferente de null
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
    // Desenha borda vermelha se encontrou a peça
    if (encontrouPecaVaso)
    {
        al_draw_rectangle(
            330, 280, 390, 340,
            al_map_rgb(255, 0, 0),
            3 // espessura da borda
        );
    }
    // Desenha borda vermelha se encontrou a peça
    if (encontrouPecaPapiro)
    {
        al_draw_rectangle(
            396, 380, 448, 438,
            al_map_rgb(255, 0, 0),
            3 // espessura da borda
        );
    }
    // Desenha borda vermelha se encontrou a peça
    if (encontrouPecaTinteiro)
    {
        al_draw_rectangle(
            286, 504, 338, 564,
            al_map_rgb(255, 0, 0),
            3 // espessura da borda
        );
    }
    // Desenha borda vermelha se encontrou a peça
    if (encontrouPecaCaixote)
    {
        al_draw_rectangle(
            1117, 671, 1172, 725,
            al_map_rgb(255, 0, 0),
            3 // espessura da borda
        );
    }
    // Se encontrou a peça do vaso, desenha no primeiro quadrado
    if (encontrouPecaVaso && ImagemPeçaVaso != NULL && pecaArrastada != 0 && !pecaEncaixada[0])
    {
        al_draw_scaled_bitmap(
            ImagemPeçaVaso,
            0, 0,
            al_get_bitmap_width(ImagemPeçaVaso),
            al_get_bitmap_height(ImagemPeçaVaso),
            857, 312,
            80, 80,
            0);
    }
    if (encontrouPecaPapiro && ImagemPeçaPapiro != NULL && pecaArrastada != 1 && !pecaEncaixada[1])
    {
        al_draw_scaled_bitmap(
            ImagemPeçaPapiro,
            0, 0,
            al_get_bitmap_width(ImagemPeçaPapiro),
            al_get_bitmap_height(ImagemPeçaPapiro),
            954, 312,
            80, 80,
            0);
    }
    if (encontrouPecaTinteiro && ImagemPeçaTinteiro != NULL && pecaArrastada != 2 && !pecaEncaixada[2])
    {
        al_draw_scaled_bitmap(
            ImagemPeçaTinteiro,
            0, 0,
            al_get_bitmap_width(ImagemPeçaTinteiro),
            al_get_bitmap_height(ImagemPeçaTinteiro),
            857, 412,
            80, 80,
            0);
    }
    if (encontrouPecaCaixote && ImagemPeçaCaixote != NULL && pecaArrastada != 3 && !pecaEncaixada[3])
    {
        al_draw_scaled_bitmap(
            ImagemPeçaCaixote,
            0, 0,
            al_get_bitmap_width(ImagemPeçaCaixote),
            al_get_bitmap_height(ImagemPeçaCaixote),
            954, 412,
            80, 80,
            0);
    }

    // vetor que guarda as quatro imagens das peças
    ALLEGRO_BITMAP *imagensPecas[TOTAL_PECAS] = {
        ImagemPeçaVaso, ImagemPeçaPapiro, ImagemPeçaTinteiro, ImagemPeçaCaixote};
    // desenha a peça selencionada acompanhando o mouse
    if (pecaArrastada != -1)
    {
        ALLEGRO_BITMAP *peca = imagensPecas[pecaArrastada];
        if (peca != NULL)
        {
            al_draw_scaled_bitmap(
                peca,
                0, 0,
                al_get_bitmap_width(peca),
                al_get_bitmap_height(peca),
                mouseArrasteX - 40,
                mouseArrasteY - 40,
                80, 80, // nossa peça mede 80 subtraimos metade para que o centro acompanhe o cursor
                0);
        }
    }
    // percorre as quatro peças
    for (int i = 0; i < TOTAL_PECAS; i++)
    {
        // desenha a peça se ela foi encaixada
        if (pecaEncaixada[i] && imagensPecas[i] != NULL)
        {
            al_draw_scaled_bitmap(
                imagensPecas[i],
                0, 0,
                al_get_bitmap_width(imagensPecas[i]),
                al_get_bitmap_height(imagensPecas[i]),
                encaixeX[i],
                encaixeY[i],
                48, 48,
                0);
        }
    }

    if (mostrarAvisoDestrancada && fonteMercado != NULL)
    {
        // Fundo da caixa de mensagem
        al_draw_filled_rectangle(
            200, 600, 1100, 750,
            al_map_rgb(60, 40, 25));

        // Borda dourada
        al_draw_rectangle(
            200, 600, 1100, 750,
            al_map_rgb(230, 185, 85),
            3);

        // Titulo
        al_draw_text(
            fonteMercado,
            al_map_rgb(255, 215, 120),
            650, 630,
            ALLEGRO_ALIGN_CENTER,
            "CASA DESTRANCADA!");

        // Primeira mensagem
        al_draw_text(
            fonteMercado,
            al_map_rgb(255, 255, 255),
            650, 655,
            ALLEGRO_ALIGN_CENTER,
            "Voce montou a chave da casa egipcia!");

        // Segunda mensagem
        al_draw_text(
            fonteMercado,
            al_map_rgb(255, 255, 255),
            650, 680,
            ALLEGRO_ALIGN_CENTER,
            "Agora voce pode explorar a casa.");

        // Instrucao
        al_draw_text(
            fonteMercado,
            al_map_rgb(255, 255, 255),
            650, 715,
            ALLEGRO_ALIGN_CENTER,
            "Pressione ESC para voltar ao mapa");
    }
}

void liberar_mercado(void)
{
    // verifica se a imagem existe na memoria antes de destruila
    if (imagemMercado != NULL)
    {
        // libera a memoria utilizada pela imagem
        al_destroy_bitmap(imagemMercado);
    }
    // Libera a imagem da peça do vaso
    if (ImagemPeçaVaso != NULL)
    {
        al_destroy_bitmap(ImagemPeçaVaso);
        ImagemPeçaVaso = NULL;
    }
    if (fonteMercado != NULL)
    {
        al_destroy_font(fonteMercado);
        fonteMercado = NULL;
    }
    // o ponteiro não aponta mais para a imagem
    imagemMercado = NULL;
}
void clique_objetos_mercado(int mouseX, int mouseY)
{
    if (mercadoAberto) // verifique se o mercado esta ativado
    {
        printf("Clique no mercado: X= %d, Y= %d \n", mouseX, mouseY);

        // verifica se o jogador clicou na peça do vaso
        if (mouseX >= 330 && mouseX <= 390 && mouseY >= 280 && mouseY <= 340)
        {
            // marcar a peça como encontrada
            if (!encontrouPecaVaso)
            {
                encontrouPecaVaso = true;
                printf("Encontrou pedaço da chave\n");
            }
        }
        // verifica se o jogador clicou na peça do papiro
        if (mouseX >= 396 && mouseX <= 448 && mouseY >= 380 && mouseY <= 438)
        {
            // marcar a peça como encontrada
            if (!encontrouPecaPapiro)
            {
                encontrouPecaPapiro = true;
                printf("Encontrou pedaço da chave\n");
            }
        }
        // verifica se o jogador clicou na peça do vaso
        if (mouseX >= 286 && mouseX <= 338 && mouseY >= 504 && mouseY <= 564)
        {
            // marcar a peça como encontrada
            if (!encontrouPecaTinteiro)
            {
                encontrouPecaTinteiro = true;
                printf("Encontrou pedaço da chave\n");
            }
        }
        // verifica se o jogador clicou na peça do vaso
        if (mouseX >= 1100 && mouseX <= 1142 && mouseY >= 650 && mouseY <= 697)
        {
            // marcar a peça como encontrada
            if (!encontrouPecaCaixote)
            {
                encontrouPecaCaixote = true;
                printf("Encontrou pedaço da chave\n");
            }
        }
    }
}
void iniciar_arraste_mercado(int mouseX, int mouseY)
{
    if (!mercadoAberto)
    {
        return;
    } // sai da função

    // verifica quais peças foram arrastadas
    bool encontrou[TOTAL_PECAS] = {
        encontrouPecaVaso, encontrouPecaPapiro, encontrouPecaTinteiro, encontrouPecaCaixote};
    // Percorre o vetor dos 4 quadrados
    for (int i = 0; i < TOTAL_PECAS; i++)
    {
        // verifica se o clique  aconteceu dentro do quadrado
        if (mouseX >= quadradoX[i] &&
            mouseX <= quadradoX[i] + 80 &&
            mouseY >= quadradoY[i] &&
            mouseY <= quadradoY[i] + 80)
        {
            // so pode arrastar uma peça encontrada e que ainda nao foi encaixada
            if (encontrou[i] && !pecaEncaixada[i])
            {
                pecaArrastada = i;
                mouseArrasteX = mouseX;
                mouseArrasteY = mouseY;
                printf("Iniciou arraste da peça %d \n", i + 1);
                return;
            }
        }
    }
}
void mover_peca_mercado(int mouseX, int mouseY)
{
    // verifica se alguma peça esta sendo arrastada
    if (mercadoAberto && pecaArrastada != -1)
    {
        // atualiza a posição do mouse
        mouseArrasteX = mouseX;
        mouseArrasteY = mouseY;
    }
}
void soltar_peca_mercado(int mouseX, int mouseY)
{
    // verifica se alguma peça esta sendo arrastada
    if (pecaArrastada == -1)
    {
        return;
    }
    // verifica se o mouse esta sobre a silhueta
    if (mouseX >= 705 && mouseX <= 838 && mouseY >= 295 && mouseY <= 510)
    {
        // marca a peça selecionada como encaixada
        pecaEncaixada[pecaArrastada] = true;
        printf("Peça %d encaixada \n", pecaArrastada + 1);
    }
    // Verifica se as quatro pecas foram encaixadas
    if (pecaEncaixada[0] &&
        pecaEncaixada[1] &&
        pecaEncaixada[2] &&
        pecaEncaixada[3] &&
        !casaDestrancada)
    {
        casaDestrancada = true;
        mostrarAvisoDestrancada = true;
    }
    // Para arrastar a peça
    pecaArrastada = -1;
}
bool encontrou_chave_casa(void){
    return casaDestrancada;
}
void resetar_mercado(void)
{
    mercadoAberto = false;

    encontrouPecaVaso = false;
    encontrouPecaPapiro = false;
    encontrouPecaTinteiro = false;
    encontrouPecaCaixote = false;

    casaDestrancada = false;
    mostrarAvisoDestrancada = false;

    for (int i = 0; i < TOTAL_PECAS; i++)
    {
        pecaEncaixada[i] = false;
    }

    pecaArrastada = -1;
    mouseArrasteX = 0;
    mouseArrasteY = 0;
}