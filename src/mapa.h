// mapa.h é responsavel por criar as funções
#ifndef MAPA_H //include guarde, evitar que o cabeçalho que é o .h seja incluido mais de uma vez
#define MAPA_H

#include <stdbool.h> // importa biblioteca de boleanos
// Declara a função que carregara o mapa com bool sim ou não
bool carregar_mapa(void);
// Declata a função que desenha o mapa
void desenhar_mapa(int largura, int altura);
// Declara a função que libera a memoria do mapa
void liberar_mapa(void);

#endif // encerra compilação condicional do ifndef