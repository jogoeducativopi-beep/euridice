#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
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
    ALLEGRO_BITMAP *fundo = al_load_bitmap("assets/imagens/fundo.png");
    if (!fundo)
    {
        fprintf(stderr, "Nao foi possivel carregar fundo.png.\n");
    }

    // cenario da primeira fase - Egito
    ALLEGRO_BITMAP *templo = al_load_bitmap("assets/imagens/templo.png");

    if (!templo)
    {
        printf("ERRO: O templo nao foi carregado!\n");
    }
    else
    {
        printf("SUCESSO: O templo foi carregado!\n");
    }

    // Adicionando o personagem da primeira fase - Egito

    // Personagem da primeira fase - Egito
    ALLEGRO_BITMAP *personagem = al_load_bitmap("assets/imagens/personagem.png");

    if (!personagem)
    {
        printf("Erro ao carregar personagem.png!\n");
    }
    else
    {
        printf("Personagem carregada com sucesso!\n");
    }

    // Carta fechada da primeira fase - Egito
    ALLEGRO_BITMAP *carta = al_load_bitmap("assets/imagens/carta_fechada.png");

    if (!carta)
    {
        printf("Erro ao carregar carta_fechada.png!\n");
    }
    else
    {
        printf("Carta carregada com sucesso!\n");
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

    float tempo_carta = 0; // Movimento da carta

    al_start_timer(timer);

    // Loop principal
    // Loop principal
    while (running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;

            // Atualiza o movimento da carta
            if (estado_atual == ESTADO_JOGO)
            {
                tempo_carta += 1.0f / 60.0f;
            }
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
                // Desenha o templo egipcio
                // Ps: esse codigo faz carregar a imagem do templo no cenario
                if (templo)
                {
                    al_draw_scaled_bitmap(
                        templo,
                        0, 0,
                        al_get_bitmap_width(templo),
                        al_get_bitmap_height(templo),
                        0, 0,
                        LARGURA_TELA,
                        ALTURA_TELA,
                        0);
                }
                else
                {
                    al_clear_to_color(al_map_rgb(25, 25, 35));
                }

                // Adiciona o personagem no templo
                if (personagem)
                {
                    al_draw_scaled_bitmap(
                        personagem,
                        0, 0,
                        al_get_bitmap_width(personagem),
                        al_get_bitmap_height(personagem),
                        180, 430, 260, 320, // Posicao do personagem
                        0);
                }

                // Desenho da carta fechada no templo - e a posicao dela
                // Desenha a carta flutuando no templo
                if (carta)
                {
                    float largura_carta = 150;

                    float altura_carta = largura_carta *
                                         al_get_bitmap_height(carta) /
                                         al_get_bitmap_width(carta);

                    // Faz a carta subir e descer
                    float movimento = sinf(tempo_carta * 2.0f) * 12.0f;

                    float carta_x = (LARGURA_TELA - largura_carta) / 2;
                    float carta_y = 510 + movimento;

                    al_draw_scaled_bitmap(
                        carta,
                        0, 0,
                        al_get_bitmap_width(carta),
                        al_get_bitmap_height(carta),
                        carta_x, carta_y,
                        largura_carta, altura_carta,
                        0);
                }
            }

            al_flip_display(); // esse comando atualiza a imagem
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

    // Libera a imagem do templo - Esse comando evita que a imagem ocupa a memória depos que o jogo fechar
    if (templo)
    {
        al_destroy_bitmap(templo);
    }

    // Libera a imagem da personagem
    if (personagem)
    {
        al_destroy_bitmap(personagem);
    }

    // Libera a imagem da carta
    if (carta)
    {
        al_destroy_bitmap(carta);
    }

    // musica
    if (musica)
    {
        al_destroy_sample(musica);
    }

    al_uninstall_audio();
    return 0;
}