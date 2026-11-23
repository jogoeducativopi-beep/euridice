#ifndef MISSAORUINAS_H
#define MISSAORUINAS_H

#include <stdbool.h>

// Inicializa os recursos da missao
bool iniciar_missao_ruinas(void);

// Verifica se o jogador esta perto das ruinas
bool missao_ruinas_perto(float jogadorX, float jogadorY);

// Verifica se o enigma esta aberto
bool missao_ruinas_aberta(void);

// Recebe as teclas do jogador
void missao_ruinas_tecla(int tecla, float jogadorX, float jogadorY);

// Fecha a janela do enigma
void fechar_missao_ruinas(void);

// Desenha o enigma na tela
void desenhar_missao_ruinas(void);

// Verifica se a terceira chave foi conquistada
bool encontrou_terceira_chave(void);

// Libera os recursos
void liberar_missao_ruinas(void);

#endif
