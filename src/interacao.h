#ifndef INTERACAO_H
#define INTERACAO_H
#include <stdbool.h>

//Recurso para mostrar as mensagens
bool iniciar_interacao(void);

//Verifica se o jogador apertou E perto do local indicado
void interacao_tecla(int tecla, float jogadorX, float jogadorY);

//Desenha a mensagem na tela
void desenhar_interacao(void);

//libera a memoria
void liberar_interacao(void);
#endif