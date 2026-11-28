#ifndef MISSAOMERCADO_H
#define MISSAOMERCADO_H
#include <stdbool.h>

//Carrega a imagem do mercado
bool inicia_mercado(void); //sim ou nao

//Abre a imagem do mercado
void abrir_mercado(void);

//Fecha a imagem do mercado
void fechar_mercado(void);

//verifica se o mercado esta aberto
bool mercado_aberto(void); //sim ou nao

//Desenha o mercado
void desenhar_mercado(void);

//Verificar os cliques
void clique_objetos_mercado(int mouseX, int mouseY);

// Verifica se a chave da casa foi encontrada
bool encontrou_chave_casa(void); //sim ou nao

// Libera a imagem da memoria
void liberar_mercado(void);

//Começa a arrastar a peça quando o jogador clica nela
void iniciar_arraste_mercado(int mouseX, int mouseY);

//Atualiza a posição da peça quando ela é arrastada
void mover_peca_mercado(int mouseX, int mouseY);

//Verifica se a peça foi encaixada corretamente
void soltar_peca_mercado(int mouseX, int mouseY);

//resetar
void resetar_mercado(void);

#endif