// Declara as funções
#ifndef CAMERA_H //include guarde, evitar que o cabeçalho que é o .h seja incluido mais de uma vez
#define CAMERA_H

// atualiza a a camera seguindo o personagem
void atualizar_camera(float jogadorX, float jogadorY,float larguraTela, float alturaTela);

//retornar a posição da camera
float posicaoCameraX(void);
float posicaoCameraY(void);

#endif // encerra compilação condicional do ifndef