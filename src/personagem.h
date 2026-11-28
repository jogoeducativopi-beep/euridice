#ifndef PERSONAGEM_H //include guarde, evitar que o cabeçalho que é o .h seja incluido mais de uma vez
#define PERSONAGEM_H

#include <stdbool.h>

//carregar as imagens do personagem
bool carregar_personagem_mapa(void);

//desenha o personagem no mapa
void desenha_personagem(float cameraX, float cameraY);

// Consulta o centro do personagem
float personagem_centro_x(void);
float personagem_centro_y(void);

// Libera as imagens da memoria
void liberar_personagem_mapa(void);

//Guarda quando uma tecla é pressionada
void personagem_tecla(int tecla, bool pressionada);

//atualiza a possição do personagem
void atualizar_posicao_personagem(int largura_mapa, int altura_mapa);

//desenha a colisão do perssonagem
void desenhar_colisao_personagem(float cameraX, float cameraY);

//resetar
void resetar_personagem(void);

#endif // encerra compilação condicional do ifndef