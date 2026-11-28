#ifndef FASETEMPLO_H
#define FASETEMPLO_H

#include <stdbool.h>

// Verificacao das tres chaves
void verificar_chaves_templo(void);
bool templo_destrancado(void);

// Tela da pergunta e tela final
void iniciar_templo(void);
bool templo_ativo(void);
bool templo_finalizado(void);

// Resposta pelo teclado
void responder_enigma_esfinge(int tecla);

// Desenho e limpeza
void desenhar_templo(void);
void fechar_templo(void);

#endif
