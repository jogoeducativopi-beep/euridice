
#include "colisao.h"
#include "mapa.h"

// Converte as coordenadas da imagem para o mundo ampliado
#define ESCALA_X (LARGURA_MUNDO / 1448.0f)
#define ESCALA_Y (ALTURA_MUNDO / 1086.0f)

// Coordenadas medidas na imagem original

static const Retangulo obstaculos[] = {

    // TEMPLO - mantemos a escadaria livre
    {493, 15, 165, 338},   // Lateral esquerda
    {790, 15, 155, 338},   // Lateral direita
    {637, 8, 149, 232},    // Porta fechada

    // PALMEIRAS PROXIMAS AO TEMPLO
    {446, 92, 27, 192},    // Palmeira grande esquerda
    {423, 190, 22, 84},    // Palmeira pequena esquerda
    {953, 125, 46, 170},    // Palmeira direita

    // COLUNAS DO LAGO
    {242, 322, 160, 167},  // Estrutura das colunas

    // PALMEIRAS DO LAGO
    {103, 326, 26, 200},   // Palmeira grande
    {58, 470, 19, 77},     // Palmeira pequena

    // CASA - telhado e paredes
    {208, 680, 176, 83},   // Parte superior da casa
    {145, 775, 257, 170},  // Corpo principal
    {398, 768, 146, 152},  // Tenda lateral da casa

    // PALMEIRA PERTO DA CASA
    {101, 738, 29, 137},

    // TENDA DO MERCADO
    {1005, 349, 199, 105}, // Toldo superior
    {978, 453, 56, 78},    // Caixas
    {1017, 449, 189, 86},  // Mercadorias
    {1177, 468, 65, 83},   // Vasos


    // ARCO E RUINAS
    {1068, 720, 104, 52},   // Parte superior do arco
    {1030, 748, 46, 145},  // Pilar esquerdo
    {1160, 770, 34, 123},  // Pilar direito
    {945, 803, 85, 38},     //topo das pedras ruina esq
    {879, 850, 150, 43},  // Pedras esquerdas
    {1168, 800, 97, 70},  // Pedras direitas

    // PALMEIRA DAS RUINAS
    {1257, 750, 23, 150},

    // PIRAMIDE
    {75, 176, 260, 19},  // Base inferior
    {79, 128, 250, 51},  // Parte inferior
    {117, 62, 174, 64},  // Parte central
    {160, 7, 28, 55},    // Ponta

    // PEDRAS A ESQUERDA DA PIRAMIDE
    {10, 207, 115, 46},

    // PIRAMIDE PEQUENA DA DIREITA
    {330, 75, 45, 30},   // Parte superior
    {305, 108, 100, 30}, // Parte central
    {280, 130, 125, 20}, // Base

    // PEDRAS DO DESERTO
    {555, 470, 95, 18},  // base da pedra
    {575, 448, 40, 18},  // ponta / bloco de cima
    {870, 705, 92, 18},  // base da pedra2
    {900, 686, 60, 16},  // parte de cima2
    {36, 998, 135, 34},  // Pedras perto da casa base3
    {60, 970, 75, 28},   //topo3
    {287, 240, 84, 35},
    {872, 972, 66, 38}
};

// Pontos que acompanham a margem do Rio Nilo
typedef struct
{
    float y;
    float x;
} PontoMargem;

// A agua fica a direita da margem
static const PontoMargem margemNilo[] = {
    {0, 1448}, {55, 1415}, {75, 1250}, {100, 1154},
    {130, 1091}, {155, 1072}, {182, 1057}, {205, 1058},
    {240, 1107}, {276, 1114}, {304, 1158}, {331, 1204},
    {366, 1270}, {406, 1311}, {446, 1337}, {482, 1332},
    {516, 1297}, {555, 1297}, {610, 1319}, {662, 1358},
    {735, 1393}, {788, 1380}, {851, 1372}, {905, 1350},
    {933, 1327}, {960, 1281}, {991, 1181}, {1020, 1098},
    {1047, 992}, {1086, 861}
};

bool colisao(Retangulo jogador, Retangulo obstaculo)
{
    return jogador.x + jogador.width >= obstaculo.x &&
           jogador.x <= obstaculo.x + obstaculo.width &&
           jogador.y + jogador.height >= obstaculo.y &&
           jogador.y <= obstaculo.y + obstaculo.height;
}

// Calcula onde a margem do rio esta em cada altura
static float limite_nilo(float y)
{
    int total = sizeof(margemNilo) / sizeof(margemNilo[0]);

    for (int i = 0; i < total - 1; i++)
    {
        if (y >= margemNilo[i].y && y <= margemNilo[i + 1].y)
        {
            float trecho = (y - margemNilo[i].y) /
                           (margemNilo[i + 1].y - margemNilo[i].y);

            return margemNilo[i].x +
                   trecho * (margemNilo[i + 1].x - margemNilo[i].x);
        }
    }

    return 1448.0f;
}

// Verifica se a posicao fica dentro do Nilo
static bool dentro_nilo(float x, float y)
{
    // Deixa a passagem de madeira do pier livre
    if (x >= 1035 && x <= 1245 && y >= 205 && y <= 285)
        return false;

    return x >= limite_nilo(y);
}

// Verifica se a posicao fica dentro do lago
static bool dentro_lago(float x, float y)
{
    // Aproximacao oval da agua do lago
    float dx = (x - 288.0f) / 180.0f;
    float dy = (y - 506.0f) / 89.0f;

    return dx * dx + dy * dy <= 1.0f;
}

// Verifica todos os obstaculos do mapa
bool posicao_bloqueada(float x, float y,
                      float largura, float altura)
{
    Retangulo pes;

    // Colisao apenas na parte inferior do personagem
    pes.x = (x + largura * 0.30f) / ESCALA_X;
    pes.y = (y + altura * 0.80f) / ESCALA_Y;

    pes.width = (largura * 0.40f) / ESCALA_X;
    pes.height = (altura * 0.20f) / ESCALA_Y;

    // Verifica todas as construcoes e objetos
    int total = sizeof(obstaculos) / sizeof(obstaculos[0]);

    for (int i = 0; i < total; i++)
    {
        if (colisao(pes, obstaculos[i]))
        {
            return true;
        }
    }

    // Pontos da area dos pes para testar a agua
    float esquerda = pes.x;
    float centro = pes.x + pes.width / 2;
    float direita = pes.x + pes.width;
    float baixo = pes.y + pes.height;

    // Colisao com o lago
    if (dentro_lago(esquerda, baixo) ||
        dentro_lago(centro, baixo) ||
        dentro_lago(direita, baixo))
    {
        return true;
    }

    // Colisao com o Rio Nilo
    if (dentro_nilo(esquerda, baixo) ||
        dentro_nilo(direita, baixo))
    {
        return true;
    }

    // Caminho livre
    return false;
}

void desenhar_colisoes(float cameraX, float cameraY)
{
    // Desenha as colisoes dos objetos
    int total = sizeof(obstaculos) / sizeof(obstaculos[0]);

    for (int i = 0; i < total; i++)
    {
        float x1 = obstaculos[i].x * ESCALA_X - cameraX;
        float y1 = obstaculos[i].y * ESCALA_Y - cameraY;

        float x2 = (obstaculos[i].x + obstaculos[i].width)
                   * ESCALA_X - cameraX;

        float y2 = (obstaculos[i].y + obstaculos[i].height)
                   * ESCALA_Y - cameraY;

        // Fundo vermelho transparente
        al_draw_filled_rectangle(
            x1, y1, x2, y2,
            al_map_rgba(255, 0, 0, 65)
        );

        // Contorno vermelho
        al_draw_rectangle(
            x1, y1, x2, y2,
            al_map_rgb(255, 0, 0),
            2
        );
    }

    // Colisao do lago
    float lagoX = 288 * ESCALA_X - cameraX;
    float lagoY = 506 * ESCALA_Y - cameraY;
    float raioX = 180 * ESCALA_X;
    float raioY = 89 * ESCALA_Y;

    al_draw_filled_ellipse(
        lagoX, lagoY,
        raioX, raioY,
        al_map_rgba(0, 100, 255, 65)
    );

    al_draw_ellipse(
        lagoX, lagoY,
        raioX, raioY,
        al_map_rgb(0, 100, 255),
        3
    );

    // Margem do Rio Nilo
    int totalPontos = sizeof(margemNilo) /
                      sizeof(margemNilo[0]);

    for (int i = 0; i < totalPontos - 1; i++)
    {
        al_draw_line(
            margemNilo[i].x * ESCALA_X - cameraX,
            margemNilo[i].y * ESCALA_Y - cameraY,

            margemNilo[i + 1].x * ESCALA_X - cameraX,
            margemNilo[i + 1].y * ESCALA_Y - cameraY,

            al_map_rgb(0, 100, 255),
            3
        );
    }
}

