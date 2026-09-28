#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// Configuração da Janela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float FPS = 60.0;

// Estrutura do retângulo
typedef struct {
float x;
float y;
float width;
float height;
ALLEGRO_COLOR color;
} Rectangle;

// Função de detecção de colisão AABB
bool check_collision(Rectangle r1, Rectangle r2) {
if (r1.x < r2.x + r2.width &&
r1.x + r1.width > r2.x &&
r1.y < r2.y + r2.height &&
r1.y + r1.height > r2.y) {
return true; // Houve colisão
}
return false; // Não há colisão
}

int main() {
// 1. Inicialização dos componentes do allegro
if (!al_init()) {
fprintf(stderr, "Falha ao inicializar a Allegro 5.\n");
return -1;
}

if (!al_install_keyboard()) {
    fprintf(stderr, "Falha ao inicializar o teclado.\n");
    return -1;
}

if (!al_init_primitives_addon()) {
    fprintf(stderr, "Falha ao inicializar o addon de primitivas.\n");
    return -1;
}

al_init_font_addon();
if (!al_init_ttf_addon()) {
    fprintf(stderr, "Falha ao inicializar o addon de fontes TTF.\n");
    return -1;
}

// 2. Criação dos recursos do allegro
ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
if (!display) {
    fprintf(stderr, "Falha ao criar a janela.\n");
    return -1;
}
al_set_window_title(display, "PoC - Teste de Colisao (Allegro 5)");

ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
ALLEGRO_FONT* font = al_create_builtin_font();

// 3. Registro das fontes de eventos
al_register_event_source(event_queue, al_get_display_event_source(display));
al_register_event_source(event_queue, al_get_timer_event_source(timer));
al_register_event_source(event_queue, al_get_keyboard_event_source());

// 4. Definição das entidades
Rectangle player = { 100.0f, 100.0f, 60.0f, 60.0f, al_map_rgb(0, 255, 0) };
Rectangle obstacle = { 350.0f, 250.0f, 100.0f, 100.0f, al_map_rgb(0, 0, 255) };

float speed = 5.0f;
bool key[4] = { false, false, false, false }; // UP, DOWN, LEFT, RIGHT
enum { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT };

bool running = true;
bool redraw = true;
bool is_colliding = false;

al_start_timer(timer);

// 5. Loop Principal do Jogo
while (running) {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    if (event.type == ALLEGRO_EVENT_TIMER) {
        // atualiza os movimento do playe
        if (key[KEY_UP] && player.y > 0) 
            player.y -= speed;
        if (key[KEY_DOWN] && player.y + player.height < SCREEN_HEIGHT) 
            player.y += speed;
        if (key[KEY_LEFT] && player.x > 0) 
            player.x -= speed;
        if (key[KEY_RIGHT] && player.x + player.width < SCREEN_WIDTH) 
            player.x += speed;

        // Verifico a colisão
        is_colliding = check_collision(player, obstacle);

        // Atualiza cor do player 
        if (is_colliding) {
            player.color = al_map_rgb(255, 0, 0); // Vermelho em colisão
        } else {
            player.color = al_map_rgb(0, 255, 0); // Verde normal
        }

        redraw = true;
        // Configura as tecla
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:    key[KEY_UP] = true; break;
            case ALLEGRO_KEY_DOWN:  key[KEY_DOWN] = true; break;
            case ALLEGRO_KEY_LEFT:  key[KEY_LEFT] = true; break;
            case ALLEGRO_KEY_RIGHT: key[KEY_RIGHT] = true; break;
            case ALLEGRO_KEY_ESCAPE: running = false; break;
        }
    } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:    key[KEY_UP] = false; break;
            case ALLEGRO_KEY_DOWN:  key[KEY_DOWN] = false; break;
            case ALLEGRO_KEY_LEFT:  key[KEY_LEFT] = false; break;
            case ALLEGRO_KEY_RIGHT: key[KEY_RIGHT] = false; break;
        }
    } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        running = false;
    }

    // Renderiza
    if (redraw && al_is_event_queue_empty(event_queue)) {
        redraw = false;

        al_clear_to_color(al_map_rgb(30, 30, 30)); // Fundo cinza escuro

        // Desenhar o obstaculo
        al_draw_filled_rectangle(
            obstacle.x, obstacle.y,
            obstacle.x + obstacle.width, obstacle.y + obstacle.height,
            obstacle.color
        );

        // Desenha o jogador
        al_draw_filled_rectangle(
            player.x, player.y,
            player.x + player.width, player.y + player.height,
            player.color
        );

        // Desenhar o status da colisao
        al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Use as setas do teclado para mover o bloco verde.");
        
        if (is_colliding) {
            al_draw_text(font, al_map_rgb(255, 50, 50), 10, 30, 0, "STATUS: COLISAO DETECTADA!");
        } else {
            al_draw_text(font, al_map_rgb(100, 255, 100), 10, 30, 0, "STATUS: Sem colisao");
        }

        al_flip_display();
    }
}

// 6. Limpeza pra libera memoria
al_destroy_font(font);
al_destroy_timer(timer);
al_destroy_event_queue(event_queue);
al_destroy_display(display);

return 0;
}
// gcc main.c -o poc_colisao.exe -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf - compila esse baguio ai