#ifndef INTERACAO_H
#define INTERACAO_H
#include <stdbool.h>

//Recurso para mostrar as mensagens
bool iniciar_interacao(void);

// Verifica se alguma mensagem esta aberta
bool interacao_aberta(void);

//Verifica se o jogador apertou E perto do local indicado
void interacao_tecla(int tecla, float jogadorX, float jogadorY);

//Desenha a mensagem na tela
void desenhar_interacao(void);

// Verifica se o personagem esta perto de um algum local 
bool jogador_perto_interacao(float jogadorX, float jogadorY);

//libera a memoria
void liberar_interacao(void);

//se esta com a chave
bool encontrou_chave_oasis(void);

bool tentar_entrar_templo(int tecla);

#endif