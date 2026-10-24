#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGURA_TELA 1300
#define ALTURA_TELA 800

// definição dos estados da tela principal e o jogo (poc)
typedef enum {
    ESTADO_MENU,
    ESTADO_JOGO
} EstadoJogo;

// controle das setas
enum { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT };

// função de colisão 
bool verificar_colisao(float x1, float y1, float w1, float h1, 
                       float x2, float y2, float w2, float h2) {
    return (x1 + w1 >= x2 && x1 <= x2 + w2 &&
            y1 + h1 >= y2 && y1 <= y2 + h2);
}

int main() {
    // inicialização do allegro e os addons
    if (!al_init()) {
        fprintf(stderr, "falha ao inicializar o allegro.\n");
        return -1;
    }

    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();
    
    if (!al_init_image_addon()) {
        fprintf(stderr, "falha ao inicializar o addon de imagem.\n");
        return -1;
    }

    // cria o temporizador, janela e fila de eventos
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    if (!timer) return -1;

    ALLEGRO_DISPLAY *display = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!display) {
        al_destroy_timer(timer);
        return -1;
    }
    al_set_window_title(display, "Eurídice");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    ALLEGRO_FONT *font = al_create_builtin_font();

    //carrega o fundo da tela inicial
    ALLEGRO_BITMAP *fundo = al_load_bitmap("fundo.png");
    if (!fundo) {
        fprintf(stderr, "Aviso: Nao foi possivel carregar a imagem 'fundo.png'. A usar cor solida.\n");
    }

    // inicio na tela principal
    EstadoJogo estado_atual = ESTADO_MENU;

    //definições do botão "JOGAR"
    float btn_largura = 350;
    float btn_altura = 90;
    float btn_x = (LARGURA_TELA / 2.0f) - (btn_largura / 2.0f);
    float btn_y = 410; 

    // váriavel da poc
    // jogador
    float player_x = 200, player_y = 200;
    float player_w = 50, player_h = 50;
    float player_speed = 5.0f;
    bool key[4] = { false, false, false, false };

    // bloco da colisão
    float bloco_x = 800, bloco_y = 350;
    float bloco_w = 120, bloco_h = 120;

    bool running = true;
    bool redraw = true;

    al_start_timer(timer);

    // loop principal
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;

            // se estiver no modo JOGO, atualiza a posição do jogador
            if (estado_atual == ESTADO_JOGO) {
                if (key[KEY_UP])    player_y -= player_speed;
                if (key[KEY_DOWN])  player_y += player_speed;
                if (key[KEY_LEFT])  player_x -= player_speed;
                if (key[KEY_RIGHT]) player_x += player_speed;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false;
            }
            // Controles da poc
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)    key[KEY_UP] = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)  key[KEY_DOWN] = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)  key[KEY_LEFT] = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) key[KEY_RIGHT] = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)    key[KEY_UP] = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)  key[KEY_DOWN] = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)  key[KEY_LEFT] = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) key[KEY_RIGHT] = false;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button == 1 && estado_atual == ESTADO_MENU) { // apenas no menu
                int mx = event.mouse.x;
                int my = event.mouse.y;

                // teste de colisão do mouse com o botão jogar invisível
                if (mx >= btn_x && mx <= btn_x + btn_largura &&
                    my >= btn_y && my <= btn_y + btn_altura) {
                    
                    printf("Botão JOGAR presssionado.. entrando no jogo...\n");
                    estado_atual = ESTADO_JOGO; // mudança de tela
                }
            }
        }

        // criação das telas
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            if (estado_atual == ESTADO_MENU) {
                // Desenha a tela inicial (1300x800)
                if (fundo) {
                    al_draw_scaled_bitmap(fundo, 0, 0, 
                        al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 
                        0, 0, LARGURA_TELA, ALTURA_TELA, 0);
                } else {
                    al_clear_to_color(al_map_rgb(25, 45, 80)); 
                }

                // HITBOX DO BOTÃO JOGAR
                // descomenta pra hitbox aparecer
                // al_draw_rectangle(btn_x, btn_y, btn_x + btn_largura, btn_y + btn_altura, al_map_rgb(255, 0, 0), 2);

            } 
            else if (estado_atual == ESTADO_JOGO) {
                // cria a tela da poc
                al_clear_to_color(al_map_rgb(30, 30, 30));

                // verifica a colisão AABB
                bool colidiu = verificar_colisao(player_x, player_y, player_w, player_h,
                                                 bloco_x, bloco_y, bloco_w, bloco_h);

                // desenha o bloco estático
                al_draw_filled_rectangle(bloco_x, bloco_y, bloco_x + bloco_w, bloco_y + bloco_h, al_map_rgb(100, 100, 255));

                // desenha o jogador (muda de cor se colidir)
                ALLEGRO_COLOR cor_jogador = colidiu ? al_map_rgb(255, 60, 60) : al_map_rgb(60, 255, 60);
                al_draw_filled_rectangle(player_x, player_y, player_x + player_w, player_y + player_h, cor_jogador);

                // textos informativos
                al_draw_text(font, al_map_rgb(255, 255, 255), 30, 30, 0, "Use as setas do teclado para mover.");
                if (colidiu) {
                    al_draw_text(font, al_map_rgb(255, 100, 100), 30, 60, 0, "COLISAO DETETADA!");
                }
            }

            al_flip_display();
        }
    }

    // limpeza de Recursos
    if (fundo) al_destroy_bitmap(fundo);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}

//gcc telainicio.c -o menu_euridice -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image comando pra compilar