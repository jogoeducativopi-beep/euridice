#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "missaoMercado.h"
#include <allegro5/allegro_primitives.h>

// ponteiro que guarda a imagem da ativação do mercado
static ALLEGRO_BITMAP *imagemMercado = NULL;
// Imagem do pedaço da chave que esta no vaso
static ALLEGRO_BITMAP *ImagemPeçaVaso = NULL;
// verifica se tela da imagem do mercado esta aberta

static ALLEGRO_BITMAP *ImagemPeçaPapiro = NULL;
static ALLEGRO_BITMAP *ImagemPeçaTinteiro = NULL;
static ALLEGRO_BITMAP *ImagemPeçaCaixote = NULL;

static bool mercadoAberto = false;
// Guarda se o jogador encontrou do vaso
static bool encontrouPecaVaso = false;
//Gurada se o jogador encontro a peça do papiro 
static bool encontrouPecaPapiro;
//Guarda se o jogador encontrou a peça do tinteiro
static bool encontrouPecaTinteiro;
//Guarda se o jogador encontrou a peça do caixote
static bool encontrouPecaCaixote;

bool inicia_mercado(void)
{
    // carrega imagem do mercado
    imagemMercado = al_load_bitmap("assets/imagens/mercado.png");

    // Carrega a imagem da peca da lua
    ImagemPeçaVaso = al_load_bitmap("assets/imagens/pecaVaso.png");

    if (ImagemPeçaVaso == NULL)
    {
        printf("Erro ao carregar a peca da lua!\n");
        return false;
    }
    // verfica se a imagem foi carregada
    if (imagemMercado == NULL)
    {
        printf("Erro ao carregar imagem do mercado");
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
    // Se encontrou a peça do vaso, desenha no primeiro quadrado
    if (encontrouPecaVaso && ImagemPeçaVaso != NULL)
    {
        al_draw_scaled_bitmap(
            ImagemPeçaVaso,
            0, 0,
            al_get_bitmap_width(ImagemPeçaVaso),
            al_get_bitmap_height(ImagemPeçaVaso),
            850, 320, 
            80, 80,
            0);
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
    if(ImagemPeçaVaso!=NULL)
    {
        al_destroy_bitmap(ImagemPeçaVaso);
        ImagemPeçaVaso = NULL;
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
    }
}