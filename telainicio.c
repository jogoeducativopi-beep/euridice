#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGURA_TELA 1300
#define ALTURA_TELA 800

typedef enum
{
    ESTADO_MENU,
    ESTADO_JOGO
} EstadoJogo;

int main()
{

    // Inicialização do Allegro
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar o Allegro.\n");
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar o addon de imagem.\n");
        return -1;
    }

    // Inicialização do áudio
    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar o audio.\n");
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar os codecs de audio.\n");
    }

    al_reserve_samples(1);

    // Criação da janela e dos eventos
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_DISPLAY *display = al_create_display(
        LARGURA_TELA, ALTURA_TELA);

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    if (!timer || !display || !event_queue)
    {
        fprintf(stderr, "Falha ao criar os recursos do jogo.\n");

        if (event_queue)
            al_destroy_event_queue(event_queue);
        if (display)
            al_destroy_display(display);
        if (timer)
            al_destroy_timer(timer);

        return -1;
    }

    al_set_window_title(display, "Euridice");

    // Carrega a imagem do menu
    ALLEGRO_BITMAP *fundo = al_load_bitmap("fundo.png");
    if (!fundo)
    {
        fprintf(stderr, "Nao foi possivel carregar fundo.png.\n");
    }

    // Carrega a música de abertura
    ALLEGRO_SAMPLE *musica = NULL;

    musica = al_load_sample("assets/musicas/musica_inicio.ogg");

    if (!musica)
    {
        fprintf(stderr, "Nao foi possivel carregar a musica.\n");
    }
    else
    {
        al_play_sample(
            musica,
            0.4,                   // Volume
            0.0,                   // Equilibrio do som
            1.0,                   // Velocidade
            ALLEGRO_PLAYMODE_LOOP, // Repetir continuamente
            NULL);
    }
    // Registra os eventos
    al_register_event_source(
        event_queue, al_get_display_event_source(display));

    al_register_event_source(
        event_queue, al_get_timer_event_source(timer));

    al_register_event_source(
        event_queue, al_get_keyboard_event_source());

    al_register_event_source(
        event_queue, al_get_mouse_event_source());

    // Estado inicial
    EstadoJogo estado_atual = ESTADO_MENU;

    // Área do botão JOGAR
    float btn_largura = 350;
    float btn_altura = 90;

    float btn_x = (LARGURA_TELA / 2.0f) - (btn_largura / 2.0f);
    float btn_y = 410;

    bool running = true;
    bool redraw = true;

    al_start_timer(timer);

    // Loop principal
    while (running)
    {

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }

        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = false;
        }

        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = false;
            }
        }

        // Clique do botão JOGAR
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {

            if (event.mouse.button == 1 &&
                estado_atual == ESTADO_MENU)
            {

                int mx = event.mouse.x;
                int my = event.mouse.y;

                if (mx >= btn_x &&
                    mx <= btn_x + btn_largura &&
                    my >= btn_y &&
                    my <= btn_y + btn_altura)
                {

                    estado_atual = ESTADO_JOGO;

                    printf("Entrando no jogo...\n");
                }
            }
        }

        // Desenho das telas
        if (redraw && al_is_event_queue_empty(event_queue))
        {

            redraw = false;

            if (estado_atual == ESTADO_MENU)
            {

                // Tela inicial
                if (fundo)
                {
                    al_draw_scaled_bitmap(
                        fundo,
                        0, 0,
                        al_get_bitmap_width(fundo),
                        al_get_bitmap_height(fundo),
                        0, 0,
                        LARGURA_TELA,
                        ALTURA_TELA,
                        0);
                }
                else
                {
                    al_clear_to_color(al_map_rgb(25, 45, 80));
                }
            }

            else if (estado_atual == ESTADO_JOGO)
            {

                // Tela vazia para desenvolver o jogo
                al_clear_to_color(al_map_rgb(25, 25, 35));
            }

            al_flip_display();
        }
    }

    // Liberação dos recursos
    if (fundo)
    {
        al_destroy_bitmap(fundo);
    }
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    // musica
    if (musica)
    {
        al_destroy_sample(musica);
    }

    al_uninstall_audio();
    return 0;
}