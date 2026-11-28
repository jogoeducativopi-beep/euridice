#ifndef FASETEMPLO_H
#define FASETEMPLO_H
#include <stdbool.h>

//verificar se esta em posse das 3 fases 
void verificar_chaves_templo(void);

//true se as 3 chaves forem encontradas
bool templo_destrancado(void);

//Inicia o video da esfinge
void reproduzir_video_esfinge(void);

//Verifica se o video esta ativo
bool video_esfinge_ativo(void);

//Desenha os quadros do video
void desenhar_video_esfinge(void);

#endif