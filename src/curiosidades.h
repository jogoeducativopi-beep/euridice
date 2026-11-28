#ifndef CURIOSIDADES_H
#define CURIOSIDADES_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>

// Chame apos al_init_image_addon() e al_init_font_addon().
bool iniciar_curiosidades(void);

// Chame uma vez por tick do timer, enquanto o estado for ESTADO_MAPA.
void atualizar_curiosidades(float jogadorX, float jogadorY, bool mapaLivre);

// Retorna true se a tecla foi consumida pelo painel de descobertas.
bool curiosidades_tecla(int tecla, float jogadorX, float jogadorY, bool mapaLivre);

// Inclui o aviso [E] Investigar e a ilustraçao quando aberta.
// Chame POR ULTIMO no desenho do mapa, por cima do resto.
void desenhar_curiosidades(ALLEGRO_FONT *fonte, bool mapaLivre);

// true enquanto o painel estiver aberto ou desaparecendo.
bool curiosidades_aberta(void);

// Libera imagens e fonte de reserva.
void liberar_curiosidades(void);

#endif
