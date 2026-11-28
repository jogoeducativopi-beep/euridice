#include <stdio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_audio.h>
#include "templo.h"
#include "missaoCasa.h"
#include "interacao.h"
#include "missaoRuinas.h"

// Ponteiro que guarda o video
static ALLEGRO_VIDEO *videoEsfinge = NULL;

// Guarda quando o video foi iniciado
static double inicioVideo = 0;

// Verifica se a reproducao ja comecou
static bool videoJaComecou = false;

bool video_esfinge_ativo(void)
{
    return videoEsfinge != NULL;
}


void verificar_chaves_templo(void)
{
    // Verifica se o jogador possui as tres chaves
    if (encontrou_chave_templo_casa() &&
        encontrou_chave_oasis() &&
        encontrou_terceira_chave())
    {
        printf("Todas as 3 chaves foram encontradas!\n");
        printf("O templo esta destrancado!\n");
    }
    else
    {
        printf("Ainda faltam chaves para abrir o templo.\n");
    }
}
// guarda true ou false se as 3 chaves foram encontradas
bool templo_destrancado(void)
{
    return encontrou_chave_templo_casa() &&
           encontrou_chave_oasis() &&
           encontrou_terceira_chave();
}

// Verifica se existe um video aberto
// Abre e inicia o video da Esfinge
void reproduzir_video_esfinge(void)
{
    // Evita iniciar o mesmo video duas vezes
    if (videoEsfinge != NULL)
    {
        return;
    }

    // Carrega o arquivo OGV
    videoEsfinge = al_open_video(
        "assets/videos/euridice_abertura_esfinge.ogv");

    if (videoEsfinge == NULL)
    {
        printf("Erro ao carregar video da Esfinge!\n");
        return;
    }

    // Recupera o mixer de audio do jogo
    ALLEGRO_MIXER *mixer = al_get_default_mixer();

    if (mixer == NULL)
    {
        printf("Mixer de audio indisponivel!\n");

        al_close_video(videoEsfinge);
        videoEsfinge = NULL;
        return;
    }

    // Para os sons anteriores
    al_stop_samples();

    inicioVideo = al_get_time();
    videoJaComecou = false;

    // Inicia o video com audio
    al_start_video(videoEsfinge, mixer);
}

// Desenha o video na janela do jogo
void desenhar_video_esfinge(void)
{
    if (videoEsfinge == NULL)
    {
        return;
    }

    bool tocando = al_is_video_playing(videoEsfinge);

    if (tocando)
    {
        videoJaComecou = true;
    }

    // Fecha quando terminar ou ultrapassar o tempo limite
    if ((videoJaComecou && !tocando) ||
        (al_get_time() - inicioVideo > 30.0))
    {
        al_close_video(videoEsfinge);
        videoEsfinge = NULL;
        return;
    }

    // Limpa a tela
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Recupera o quadro atual
    ALLEGRO_BITMAP *quadro = al_get_video_frame(videoEsfinge);

    if (quadro != NULL)
    {
        al_draw_scaled_bitmap(
            quadro,
            0, 0,
            al_get_bitmap_width(quadro),
            al_get_bitmap_height(quadro),
            0, 0,
            1300, 800,
            0);
    }
}