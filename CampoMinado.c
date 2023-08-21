#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int eBomba;
    int estaAberta;
    int vizinhos;
} Celula;

//Se quiser aumentar o tamanho alterar valores:
Celula jogo[10][10];
int linha, coluna, tamanho = 10;

//Prototipos
void inicializarJogo(); //Tira possivel lixo de memoria existente na matriz
void sortearBombas(int n); //Recebe como parametro quantas bombas o jogo tera e sorteia elas pelo tabuleiro
void contarBombas(); //Atribui a jogo.vizinhos quantas bombas tem em volta chamando quantBombasVizinhas()
int quantBombasVizinhas(int l, int c); //Verifica as posicoes proximas as casas passando linha / coluna como parametro
void imprimir();
int coordenadaEhValida(int l, int c);
void jogar(); //Ler as coordenadas do teclado
void abrirCelula(int l, int c);
int ganhou();

int main(void){
    
    int opcao;
    do {
        inicializarJogo();
        sortearBombas(10);
        contarBombas();
        printf("\n\t\t\tCampo Minado");
        jogar();

        printf("\nDigite 1 para jogar novamente: ");
        scanf("%d", &opcao);
    } while(opcao == 1);

    return 0;
}

//Procedimento para inicializar
void inicializarJogo() {
    for(linha = 0; linha < tamanho; linha++) {
        for(coluna = 0; coluna < tamanho; coluna++) {
            jogo[linha][coluna].eBomba = 0;
            jogo[linha][coluna].estaAberta = 0;
            jogo[linha][coluna].vizinhos = 0;
        }
    }
}

//Procedimento para sortear n bombas
void sortearBombas(int n) {
    int i;
    srand(time(NULL));

    for (i = 1; i < n; i++) {
        linha = rand() % tamanho;
        coluna = rand() % tamanho;
        if(jogo[linha][coluna].eBomba == 0)
            jogo[linha][coluna].eBomba = 1;
        else
            i--;
    }
}

void contarBombas() { 
    for(linha = 0; linha < tamanho; linha++) {
        for(coluna = 0; coluna < tamanho; coluna++) {
            jogo[linha][coluna].vizinhos = quantBombasVizinhas(linha, coluna);
        }
    }
}

//Procedimento para imprimir o jogo
void imprimir() {

    printf("\n\n\t   ");
    for(linha = 0; linha < tamanho; linha++)
        printf(" %d  ", linha);// indices das colunas
    printf("\n\t  -----------------------------------------\n");


    for(linha = 0; linha < tamanho; linha++) {
        printf("\t%d |", linha);
        for(coluna = 0; coluna < tamanho; coluna++) {
            if(jogo[linha][coluna].estaAberta) {
                if(jogo[linha][coluna].eBomba)
                    printf(" * ");
                else
                    printf(" %d ", jogo[linha][coluna].vizinhos);
            } else
                printf("   ");
            printf("|");
        }
        printf("\n\t  -----------------------------------------\n");
    }
}

/*
    funcao que retorna a quantidade de bombas na vizinhanca de l c
    n - quantidade de bombas
*/
int quantBombasVizinhas(int l, int c) {
    /* 
       l - 1 e c /Cima
       l + 1 e c /Baixo
       l e c + 1 /Direita
       l e c - 1 /Esquerda
    */

    int quantidade = 0;

    if(coordenadaEhValida(l - 1, c) && jogo[l-1][c].eBomba)
        quantidade++;
    if(coordenadaEhValida(l + 1, c) && jogo[l+1][c].eBomba)
        quantidade++;
    if(coordenadaEhValida(l, c + 1) && jogo[l][c + 1].eBomba)
        quantidade++;
    if(coordenadaEhValida(l, c - 1) && jogo[l][c - 1].eBomba)
        quantidade++;

    return quantidade;
}

/*
    funcao que diz se um par de coordenadas eh valido ou nao
    1 - eh valido
    0 - nao eh valido    
*/
int coordenadaEhValida(int l, int c) {
    if(l >= 0 && l < tamanho && c >= 0 && c < tamanho)
        return 1;
    else
        return 0;

}

//Procedimento jogor que faz a leitura das coordenadas
void jogar() {
    int l, c;

    do{
        imprimir();
        do {
            printf("\nDigite as coordenadas de linha e coluna: ");
            scanf("%d%d", &l, &c);

            if(coordenadaEhValida(l, c) == 0 || jogo[l][c].estaAberta == 1)
                printf("\nCoordenada invalida ou ja aberta!");

        } while(coordenadaEhValida(l, c) == 0 || jogo[l][c].estaAberta == 1);

        abrirCelula(l, c);
    } while (ganhou() != 0 && jogo[l][c].eBomba == 0);

    if(jogo[l][c].eBomba == 1)
        printf("\n\tQue pena! Voce perdeu!!!\n");
    else
        printf("\n\tParabens! Voce ganhou!!!\n");

    imprimir();
}

//Procedimento para abrir e coordenada digitada pelo usuario
void abrirCelula(int l, int c) {
    
    if (coordenadaEhValida(l, c) == 1 && jogo[l][c].estaAberta == 0) {
        jogo[l][c].estaAberta = 1;
        if (jogo[l][c].vizinhos == 0) {
            /*
                l - 1 e c acima
                l + 1 e c abaixo
                l e c + 1 direita
                l e c - 1 esquerda
            */
            abrirCelula(l - 1, c); 
            abrirCelula(l + 1, c);
            abrirCelula(l, c + 1);
            abrirCelula(l, c - 1);
        }
    }
}

//Funcao para verificar vitoria 1 == ganhou 0 == nao ganhou ainda
int ganhou() {
    int quantidade = 0;

    for(linha = 0; linha < tamanho; linha++) {
        for(coluna = 0; coluna < tamanho; coluna++) {
            if(jogo[linha][coluna].estaAberta == 0 && jogo[linha][coluna].eBomba == 0)
                quantidade++;
        }
    }
    return quantidade;
}
