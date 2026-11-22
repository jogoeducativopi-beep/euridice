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

#endif