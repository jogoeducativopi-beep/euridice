#ifndef MISSAOCASA_H
#define MISSAOCASA_H

#include <stdbool.h>

// Carrega a imagem do interior da casa
bool iniciar_casa(void);

// Abre a janela da casa
void abrir_casa(void);

// Fecha a janela da casa
void fechar_casa(void);

// Verifica se estamos dentro da casa
bool casa_aberta(void);

// Desenha o interior da casa
void desenhar_casa(void);

// Verifica os cliques nos objetos
void clique_objetos_casa(int mouseX, int mouseY);

// Libera a imagem da memoria
void liberar_casa(void);

#endif
