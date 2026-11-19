#ifndef PERSONAGEM_H //include guarde, evitar que o cabeçalho que é o .h seja incluido mais de uma vez
#define PERSONAGEM_H

#include <stdbool.h>

//carregar as imagens do personagem
bool carregar_personagem_mapa(void);

//desenha o personagem no mapa
void desenha_personagem(void);

// Libera as imagens da memoria
void liberar_personagem_mapa(void);

#endif // encerra compilação condicional do ifndef