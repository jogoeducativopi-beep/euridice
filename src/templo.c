#include <stdio.h>
#include "templo.h"
#include "missaoCasa.h"
#include "interacao.h"
#include "missaoRuinas.h"

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
//guarda true ou false se as 3 chaves foram encontradas
bool templo_destrancado(void)
{
    return encontrou_chave_templo_casa() &&
           encontrou_chave_oasis() &&
           encontrou_terceira_chave();
}