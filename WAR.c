#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR  10
#define NUM_MISSOES 2


typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;


Territorio* alocarMapa(int n);
void liberarMemoria(Territorio *mapa);

void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa, int n);
void exibirMissao(int missaoId);

void faseDeAtaque(Territorio *mapa, int n);
void simularAtaque(Territorio *origem, Territorio *destino);

int sortearMissao(void);
int verificarVitoria(const Territorio *mapa, int n, int missaoId);

void limparBufferEntrada(void);


Territorio* alocarMapa(int n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

void exibirMenuPrincipal(void) {
    puts("\n=== MENU PRINCIPAL ===");
    puts("1 - Atacar");
    puts("2 - Verificar missão");
    puts("0 - Sair");
    printf("Escolha: ");
}

void exibirMapa(const Territorio *mapa, int n) {
    puts("\n=== MAPA ATUAL ===");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-20s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int missaoId) {
    puts("\n=== SUA MISSÃO ===");
    switch(missaoId) {
        case 0: puts("Conquistar 2 territórios."); break;
        case 1: puts("Destruir um exército inimigo."); break;
        default: puts("Missão desconhecida."); break;
    }
}

void faseDeAtaque(Territorio *mapa, int n) {
    int origem, destino;
    exibirMapa(mapa, n);

    printf("\nEscolha o território de origem (índice): ");
    scanf("%d", &origem);
    printf("Escolha o território de destino (índice): ");
    scanf("%d", &destino);

    if (origem < 0 || origem >= n || destino < 0 || destino >= n || origem == destino) {
        puts("Jogada inválida!");
        return;
    }

    simularAtaque(&mapa[origem], &mapa[destino]);
}

void simularAtaque(Territorio *origem, Territorio *destino) {
    if (origem->tropas <= 0) {
        puts("Não há tropas suficientes para atacar!");
        return;
    }

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("Ataque rolou: %d | Defesa rolou: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        destino->tropas--;
        if (destino->tropas <= 0) {
            puts("Você conquistou o território!");
            strcpy(destino->cor, origem->cor);
            destino->tropas = 1;
            origem->tropas--;
        }
    } else {
        origem->tropas--;
        puts("Ataque falhou.");
    }
}

int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio *mapa, int n, int missaoId) {
    if (missaoId == 0) {
     
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, "Jogador") == 0) count++;
        }
        return (count >= 2);
    } 
    else if (missaoId == 1) {
       
        for (int i = 0; i < n; i++) {
            const char *corInimiga = mapa[i].cor;
            if (strcmp(corInimiga, "Jogador") == 0) continue;

            int todosZerados = 1;
            for (int j = 0; j < n; j++) {
                if (strcmp(mapa[j].cor, corInimiga) == 0 && mapa[j].tropas > 0) {
                    todosZerados = 0;
                    break;
                }
            }

            if (todosZerados) return 1; 
        }
        return 0;
    }
    return 0;
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


int main(void) {
    srand(time(NULL));


    Territorio *mapa = alocarMapa(MAX_TERRITORIOS);
    if (!mapa) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 1;
    }

    puts("\n=== Cadastro de Territórios ===\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf(">>> Território %d de %d\n", i + 1, MAX_TERRITORIOS);

        printf("Nome do território: ");
        scanf(" %29[^\n]", mapa[i].nome);

        printf("Cor do exército: ");
        scanf(" %9[^\n]", mapa[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
        puts("");
    }

   
    int missao = sortearMissao();

  
    int opcao, venceu = 0;
    do {
        exibirMapa(mapa, MAX_TERRITORIOS);
        exibirMissao(missao);
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1: faseDeAtaque(mapa, MAX_TERRITORIOS); break;
            case 2:
                if (verificarVitoria(mapa, MAX_TERRITORIOS, missao)) {
                    puts("\n🎉 Você venceu sua missão!");
                    venceu = 1;
                } else {
                    puts("Missão ainda não cumprida.");
                }
                break;
            case 0: puts("Saindo..."); break;
            default: puts("Opção inválida."); break;
        }
    } while(opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

