#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "mapa.h"
#include "personagem.h"
#include "camera.h"
#include "colisao.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "interacao.h"
#include "missaoMercado.h"
#include "missaoCasa.h"

#define LARGURA_TELA 1300
#define ALTURA_TELA 800

typedef enum
{
    ESTADO_MENU,
    ESTADO_JOGO,
    ESTADO_MAPA
} EstadoJogo;

int main()
{
    printf("\nTeste colisao: %d\n",
           posicao_bloqueada(800, 200, 75, 50));

    // Inicialização do Allegro
    if (!al_init())

    {
        fprintf(stderr, "Falha ao inicializar o Allegro.\n");
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();
    // Inicializa os recursos para desenhar textos e formas
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    // Inicia o texto
    ALLEGRO_FONT *fonte_carta = al_load_ttf_font("C:/Windows/Fonts/georgia.ttf", 20, 0);

    if (!fonte_carta)
    {
        printf("ERRO: Fonte Georgia nao carregou!\n");
    }
    else
    {
        printf("SUCESSO! Fonte Georgia carregada!\n");
        printf("Altura da fonte: %d pixels\n",
               al_get_font_line_height(fonte_carta));
    }

    // inicia a imagem
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar o addon de imagem.\n");
        return -1;
    }

    if (!inicia_mercado())
    {
        printf("Nao foi possivel carregar o mercado.\n");
        return -1; // o if so sera executado quando o carregamento falhar
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

    // carrega mapa
    if (!carregar_mapa())
    {
        printf("Erro ao carregar mapa do Egito");
    }
    // Carrega personagem do mapa
    if (!carregar_personagem_mapa())
    {
        printf("Falha ao carregar personagem!");
    }

    // Carrega os recursos das interacoes
    if (!iniciar_interacao())
    {
        printf("Erro ao iniciar as interacoes!\n");
    }
    // Carrega o interior da casa
    if (!iniciar_casa())
    {
        printf("Erro ao iniciar a casa!\n");
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

    //  Abrir a carta missão inicial
    ALLEGRO_BITMAP *carta_aberta = al_load_bitmap("assets/imagens/carta_aberta.png");

    if (!carta_aberta)
    {
        printf("Erro ao carregar carta_aberta.png!\n");
    }
    else
    {
        printf("Carta aberta carregada com sucesso!\n");
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
    bool mostrar_colisoes = false;

    // Controla a mensagem das setas
    bool mostrarInstrucaoSetas = true;

    float tempo_carta = 0; // Movimento da carta

    bool carta_esta_aberta = false; // Esse comando é da abertura e fechamento da carta

    al_start_timer(timer);

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

            // Atualizar movimento no mapa
            if (estado_atual == ESTADO_MAPA && !mercado_aberto() && !interacao_aberta() && !casa_aberta())
            {
                // Movimenta a personagem dentro do mundo
                atualizar_posicao_personagem(LARGURA_MUNDO, ALTURA_MUNDO);

                // Faz a camera acompanhar a personagem
                atualizar_camera(
                    personagem_centro_x(),
                    personagem_centro_y(),
                    LARGURA_TELA,
                    ALTURA_TELA);
            }
        }

        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = false;
        }

        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        { // fecha o jogo ao pressionar esc
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                // Verifica se a tela do mercado esta aberta
                if (mercado_aberto())
                {
                    // Fecha apenas o mercado e volta ao mapa
                    fechar_mercado();
                }
                else if (casa_aberta())
                {
                    fechar_casa();
                }
                else
                {
                    // Se nao estiver no mercado, encerra o jogo
                    running = false;
                }
            }
            // Precionar espaço com a carta aberta
            else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                if (estado_atual == ESTADO_JOGO && carta_esta_aberta)
                {
                    carta_esta_aberta = false;
                    estado_atual = ESTADO_MAPA;
                    printf("Entrou no mapa do egito");
                }
            }

            // Liga ou desliga as hitboxes ao apertar F3
            else if (event.keyboard.keycode == ALLEGRO_KEY_F3 && estado_atual == ESTADO_MAPA)
            {
                mostrar_colisoes = !mostrar_colisoes;

                printf("DEBUG COLISOES: %s\n",
                       mostrar_colisoes ? "LIGADO" : "DESLIGADO");
            }

            // Registra a tecla pressionada no mapa
            if (estado_atual == ESTADO_MAPA)
            {

                // movimento do personagem
                personagem_tecla(event.keyboard.keycode, true);

                // Esconde a instrucao quando o jogador apertar uma seta
                if (event.keyboard.keycode == ALLEGRO_KEY_UP ||
                    event.keyboard.keycode == ALLEGRO_KEY_DOWN ||
                    event.keyboard.keycode == ALLEGRO_KEY_LEFT ||
                    event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    mostrarInstrucaoSetas = false;
                }

                // Verifica se o jogador quer interagir com algum local
                interacao_tecla(
                    event.keyboard.keycode,
                    personagem_centro_x(),
                    personagem_centro_y());
            }
        }

        // impedir o persongem de continuar andando ao soltar a tecla
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (estado_atual == ESTADO_MAPA)
            {
                personagem_tecla(event.keyboard.keycode, false);
                //verifica a resposta do enigma
                responder_enigma_casa(event.keyboard.keycode);
            }
        }

        // Detecta movimento do mouse
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) // evento que identifica os movimentos do mouse
        {
            if (estado_atual == ESTADO_MAPA && mercado_aberto())
            {
                mover_peca_mercado(event.mouse.x, event.mouse.y);
            }
        }

        // Clique do botão JOGAR e mouse

        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.mouse.button == 1)
            {
                int mx = event.mouse.x;
                int my = event.mouse.y;

                if (estado_atual == ESTADO_MAPA && casa_aberta())
                {
                    //Clique nos objetos do interior da casa
                    clique_objetos_casa(mx, my);
                }

                // Verifica se o jogador esta dentro do mercado
                else if (estado_atual == ESTADO_MAPA && mercado_aberto())
                {
                    clique_objetos_mercado(mx, my);

                    // verifica se clicou na peça do painel
                    iniciar_arraste_mercado(mx, my);
                }

                // Clique do botão JOGAR
                else if (estado_atual == ESTADO_MENU)
                {
                    if (mx >= btn_x &&
                        mx <= btn_x + btn_largura &&
                        my >= btn_y &&
                        my <= btn_y + btn_altura)
                    {
                        estado_atual = ESTADO_JOGO;
                        printf("Entrando no jogo...\n");
                    }
                }

                // Clique da carta
                else if (estado_atual == ESTADO_JOGO && carta)
                {
                    if (carta_esta_aberta)
                    {
                        // Fecha a carta ao clicar no X
                        if (mx >= 915 && mx <= 965 &&
                            my >= 135 && my <= 185)
                        {
                            carta_esta_aberta = false;
                        }
                    }
                    else
                    {
                        float largura_carta = 150;
                        float altura_carta = largura_carta *
                                             al_get_bitmap_height(carta) /
                                             al_get_bitmap_width(carta);

                        float carta_x = (LARGURA_TELA - largura_carta) / 2;
                        float carta_y = 510 + sinf(tempo_carta * 2.0f) * 12.0f;

                        if (mx >= carta_x &&
                            mx <= carta_x + largura_carta &&
                            my >= carta_y &&
                            my <= carta_y + altura_carta)
                        {
                            carta_esta_aberta = true;
                            printf("Carta misteriosa aberta!\n");
                        }
                    }
                }
            }
        }

        // Quando o jogador soltar o botao do mouse
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (event.mouse.button == 1 &&
                estado_atual == ESTADO_MAPA &&
                mercado_aberto())
            {
                soltar_peca_mercado(
                    event.mouse.x,
                    event.mouse.y);
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
                if (carta)
                {
                    float largura_carta = 150;

                    float altura_carta = largura_carta *
                                         al_get_bitmap_height(carta) /
                                         al_get_bitmap_width(carta);

                    // Faz a carta subir e descer
                    float movimento = sinf(tempo_carta * 1.2f) * 8.0f;

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

                // Essa estapa mostra a abertura da carta
                if (carta_esta_aberta && carta_aberta)
                {
                    // Escurece o templo
                    al_draw_filled_rectangle(
                        0, 0,
                        LARGURA_TELA, ALTURA_TELA,
                        al_map_rgba(0, 0, 0, 170));

                    // Mostra a carta aberta no centro

                    al_draw_scaled_bitmap(
                        carta_aberta,
                        0, 0,
                        al_get_bitmap_width(carta_aberta),
                        al_get_bitmap_height(carta_aberta),
                        275, 80, 750, 640,
                        0);

                    // texto da carta
                    if (fonte_carta)
                    {
                        ALLEGRO_COLOR cor_texto = al_map_rgb(75, 45, 25);

                        al_draw_text(fonte_carta, cor_texto,
                                     650, 265, ALLEGRO_ALIGN_CENTER,
                                     "CARO EXPLORADOR,");

                        al_draw_text(fonte_carta, cor_texto,
                                     650, 310, ALLEGRO_ALIGN_CENTER,
                                     "Voce chegou ao Antigo Egito.");

                        al_draw_text(fonte_carta, cor_texto,
                                     650, 335, ALLEGRO_ALIGN_CENTER,
                                     "Explore e descubra seus misterios.");

                        al_draw_text(fonte_carta, cor_texto,
                                     650, 360, ALLEGRO_ALIGN_CENTER,
                                     "Encontre as pistas para voltar.");

                        al_draw_text(fonte_carta, cor_texto,
                                     650, 410, ALLEGRO_ALIGN_CENTER,
                                     "Sua aventura começa agora!");

                        al_draw_text(
                            fonte_carta,
                            al_map_rgb(0, 0, 0),
                            650, 480,
                            ALLEGRO_ALIGN_CENTER,
                            "Pressione ESPAÇO para iniciar");
                    }
                }
            }
            // Desenha o mapa do Egito e personagem

            else if (estado_atual == ESTADO_MAPA)
            {
                // Desenha o mapa
                desenhar_mapa(
                    posicaoCameraX(),
                    posicaoCameraY());

                // Instrucao de movimentacao do personagem

                if (fonte_carta && mostrarInstrucaoSetas) // mostra ate aperta na seta
                {
                    // Fundo da mensagem
                    al_draw_filled_rectangle(
                        390, 740, 910, 785,
                        al_map_rgba(60, 40, 25, 220));

                    // Texto da mensagem
                    al_draw_text(
                        fonte_carta,
                        al_map_rgb(255, 215, 120),
                        650, 745,
                        ALLEGRO_ALIGN_CENTER,
                        "Use as SETAS para se movimentar");
                }

                // Mostra as colisoes dos objetos
                if (mostrar_colisoes)
                {
                    desenhar_colisoes(
                        posicaoCameraX(),
                        posicaoCameraY());
                }

                // Desenha o personagem
                desenha_personagem(
                    posicaoCameraX(),
                    posicaoCameraY());

                // Mostra a tecla E quando o personagem esta perto de um local
                if (jogador_perto_interacao(
                        personagem_centro_x(),
                        personagem_centro_y()) &&
                    !interacao_aberta())
                {
                    al_draw_filled_rectangle(
                        530, 30, 770, 75,
                        al_map_rgba(60, 40, 25, 220));

                    al_draw_text(
                        fonte_carta,
                        al_map_rgb(255, 215, 120),
                        650, 38,
                        ALLEGRO_ALIGN_CENTER,
                        "[E] Interagir");
                }

                // Mostra a hitbox dos pes
                if (mostrar_colisoes)
                {
                    desenhar_colisao_personagem(
                        posicaoCameraX(),
                        posicaoCameraY());
                }
                // Desenha as mensagens por cima do mapa
                desenhar_interacao();
                // desenha o mercado por cima do mapa
                desenhar_mercado();
                // desenha o interior da casa por cima do mapa
                desenhar_casa();
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
    liberar_mapa();
    liberar_personagem_mapa();
    // Libera os recursos das mensagens
    liberar_interacao();
    liberar_mercado();
    liberar_casa();
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

    // Libera a imagem da carta aberta
    if (carta_aberta)
    {
        al_destroy_bitmap(carta_aberta);
    }

    // musica
    if (musica)
    {
        al_destroy_sample(musica);
    }
    al_uninstall_audio();
    return 0;
}
