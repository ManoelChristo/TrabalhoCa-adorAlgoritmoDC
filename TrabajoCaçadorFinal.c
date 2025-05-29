#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// Definições de constantes para o tamanho dos mapas
#define LIN1 10  // Sala Inicial
#define COL1 26
#define LIN2 10  // Vila
#define COL2 10
#define LIN3 18  // Fase 2
#define COL3 20
#define LIN4 18  // Sala Troll
#define COL4 19
#define LIN5 28  // Sala Final
#define COL5 30

/* Variáveis Globais */
int vidas = 3;          // Contador de vidas do jogador
int mortes = 0;         // Contador de mortes
HANDLE hConsole;        // Handle para console (cores)

/* Mapas do jogo */

// Sala Inicial
char mapa1[LIN1][COL1 + 1] = {
    "**************************",
    "*************?**********D*",
    "************* ********** *",
    "************* ********** *",
    "************* ********** *",
    "*                        *",
    "*                        *",
    "*                        *",
    "*                        *",
    "**************************"
};

// Posição inicial do jogador na sala 1
int px = 1, py = 8;
int portaAberta = 0;    // Estado da porta
int leuPlaca = 0;       // Flag se leu a placa

// Vila
char mapa2[LIN2][COL2 + 1] = {
    {'*','*','*','*','*','*','*','*','*','*','\0'},
    {'*',' ',' ','@','*',' ',' ','*','D','*','\0'},
    {'*',' ',' ',' ','*',' ',' ','*',' ','*','\0'},
    {'*',' ','*','*','*',' ',' ','*',' ','*','\0'},
    {'*','P',' ',' ',' ',' ',' ',' ',' ','*','\0'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*','\0'},
    {'*',' ',' ',' ',' ','P',' ',' ',' ','*','\0'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*','\0'},
    {'*',' ','*',' ',' ',' ',' ',' ',' ','*','\0'},
    {'*','*','*','*','*','*','*','*','*','*','\0'}
};

// Posição inicial do jogador na vila
int jogadorX = 8, jogadorY = 1;
int portaAberta2 = 0;   // Estado da porta
int chaveColetada = 0;  // Flag se coletou a chave

// Fase 2
char mapa3[LIN3][COL3 + 1] = {
    "********************",
    "*################@#*",
    "*##################*",
    "*##################*",
    "*##################*",
    "*##################*",
    "*##################*",
    "*                  *",
    "*##################*",
    "*                  *",
    "*                  *",
    "*   O              *",
    "*                  *",
    "*                  *",
    "*             ?    *",
    "*                ***",
    "*                D**",
    "********************"
};

// Posição inicial do jogador na fase 2
int jogadorF3X = 16, jogadorF3Y = 1;
int portaAberta3 = 0;       // Estado da porta
int chaveColetada3 = 0;     // Flag se coletou a chave
int botaoAtivado = 0;       // Flag se ativou o botão
int inimigoX = 7, inimigoY = 8; // Posição do inimigo

// Sala Troll
char mapa4[LIN4][COL4 + 1] = {
    "*******************",
    "*    *=*          *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*     O           *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*                 *",
    "*     ?           *",
    "*                 *",
    "*                 *",
    "*******************"
};

// Sala Final
char mapa5[LIN5][COL5 + 1] = {
    "******************************",
    "*#############*      M       *",
    "*#############*              *",
    "*#           #*              *",
    "*# ######### #*              *",
    "*# ##**>**## #*      ?       *",
    "*# ##*   *## #*              *",
    "*# ##     ## #*              *",
    "*# ##     ## #*              *",
    "*# ###   ### #*              *",
    "*# ####   ## #*              *",
    "*# #####  ## #*              *",
    "*# ####   ## #*              *",
    "*# ##   #### #*              *",
    "*# ##  ##### #*              *",
    "*# ##   #### #*              *",
    "*# ###    ## #*              *",
    "*# #####  ## #*              *",
    "*#        ## #*              *",
    "*########### #*              *",
    "*            #*              *",
    "* ############*              *",
    "* ############*              *",
    "* ############*              *",
    "*             *              *",
    "*############ *              *",
    "*############ *              *",
    "*############ *              *",
    "*&            *       >      *",
    "******************************"
};

// Variáveis de posição e estado
int jogadorF4X = 16, jogadorF4Y = 1;  // Sala Troll
int jogadorF5X = 28, jogadorF5Y = 1;   // Sala Final
int botaoAtivado4 = 0;                 // Botão na sala troll
int trollMessageShown = 0;             // Mensagem troll
int inimigoVX = 10, inimigoVY = 10;    // Inimigo verde 1
int inimigoV2X = 5, inimigoV2Y = 5;    // Inimigo verde 2
int salaFinalAtivada = 0;              // Flag sala final

/* Funções auxiliares */

// Centraliza texto na tela
void centralizarTexto(const char *texto) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int cols, i, espacos;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    espacos = (cols - strlen(texto)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s\n", texto);
}

// Espera o jogador pressionar 'i' para continuar
void esperarTeclaI() {
    printf("\nPressione 'i' para continuar...\n");
    while (getch() != 'i');
}

/* Funções de game over e reinício */

// Lida com o game over e opções de reinício
void gameOver() {
    int opcao = 0;
    char tecla;
    
    vidas--;
    if(vidas > 0) {
        system("cls");
        SetConsoleTextAttribute(hConsole, 12);
        if (trollMessageShown) {
            centralizarTexto("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
        } else {
            centralizarTexto("VOCE PERDEU UMA VIDA");
        }
        SetConsoleTextAttribute(hConsole, 15);
        printf("\n");
        centralizarTexto("Vidas restantes:");
        centralizarTexto(vidas == 3 ? "III" : vidas == 2 ? "II" : "I");
        Sleep(2000);
        
        // Reinicia a posição do jogador dependendo da fase atual
        if(jogadorF5X != -1) {
            jogadorF5X = 28; jogadorF5Y = 1;
        } else if(jogadorF4X != -1) {
            jogadorF4X = 16; jogadorF4Y = 1;
            inimigoVX = 10; inimigoVY = 10;
            inimigoV2X = 5; inimigoV2Y = 5;
            botaoAtivado4 = 0;
            trollMessageShown = 0;
            strcpy(mapa4[5], "*                 *");
            strcpy(mapa4[6], "*     O           *");
            strcpy(mapa4[7], "*                 *");
        } else if(jogadorF3X != -1) {
            jogadorF3X = 16; jogadorF3Y = 1;
            inimigoX = 7; inimigoY = 8;
            botaoAtivado = 0;
            chaveColetada3 = 0;
            portaAberta3 = 0;
            strcpy(mapa3[8], "*##################*");
            strcpy(mapa3[9], "*                  *");
            strcpy(mapa3[10], "*                  *");
            strcpy(mapa3[1], "*################@#*");
            mapa3[16][17] = 'D';
        } else if(jogadorX != -1) {
            jogadorX = 8; jogadorY = 1;
            chaveColetada = 0;
            portaAberta2 = 0;
            mapa2[1][3] = '@';
        } else {
            px = 1; py = 8;
            portaAberta = 0;
            leuPlaca = 0;
            mapa1[1][24] = 'D';
        }
        return;
    }
    
    // Tela de game over
    while(1) {
        system("cls");
        SetConsoleTextAttribute(hConsole, 12);
        if (trollMessageShown) {
            centralizarTexto("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
        } else {
            centralizarTexto("GAME OVER");
        }
        SetConsoleTextAttribute(hConsole, 15);
        centralizarTexto("O cacador virou a presa");
        printf("\n");
        
        centralizarTexto("Jogar novamente?");
        printf("\n");
        
        // Menu de opções após game over
        if(opcao == 0) {
            SetConsoleTextAttribute(hConsole, 14);
            centralizarTexto("-> Sim");
            SetConsoleTextAttribute(hConsole, 15);
            centralizarTexto("   Nao");
        } else {
            centralizarTexto("   Sim");
            SetConsoleTextAttribute(hConsole, 14);
            centralizarTexto("-> Nao");
            SetConsoleTextAttribute(hConsole, 15);
        }
        
        tecla = getch();
        if(tecla == 'w' || tecla == 's') {
            opcao = !opcao;
        } else if(tecla == 'i') {
            if(opcao == 0) {
                // Reinicia o jogo
                vidas = 3;
                mortes = 0;
                px = 1; py = 8;
                jogadorX = 8; jogadorY = 1;
                jogadorF3X = 16; jogadorF3Y = 1;
                jogadorF4X = 16; jogadorF4Y = 1;
                jogadorF5X = 28; jogadorF5Y = 1;
                inimigoX = 7; inimigoY = 8;
                inimigoVX = 10; inimigoVY = 10;
                inimigoV2X = 5; inimigoV2Y = 5;
                botaoAtivado = 0;
                chaveColetada3 = 0;
                portaAberta3 = 0;
                botaoAtivado4 = 0;
                trollMessageShown = 0;
                salaFinalAtivada = 0;
                strcpy(mapa3[8], "*##################*");
                strcpy(mapa3[9], "*                  *");
                strcpy(mapa3[10], "*                  *");
                strcpy(mapa3[1], "*################@#*");
                mapa3[16][17] = 'D';
                strcpy(mapa4[5], "*                 *");
                strcpy(mapa4[6], "*     O           *");
                strcpy(mapa4[7], "*                 *");
                mapa2[1][3] = '@';
                mapa1[1][24] = 'D';
                portaAberta = 0;
                leuPlaca = 0;
                chaveColetada = 0;
                portaAberta2 = 0;
                iniciarSala();
                return;
            } else {
                // Opção para voltar ao menu ou sair
                int opcaoMenu = 0;
                while(1) {
                    system("cls");
                    centralizarTexto("Voltar ao menu principal?");
                    printf("\n");
                    
                    if(opcaoMenu == 0) {
                        SetConsoleTextAttribute(hConsole, 14);
                        centralizarTexto("-> Sim");
                        SetConsoleTextAttribute(hConsole, 15);
                        centralizarTexto("   Nao (Sair)");
                    } else {
                        centralizarTexto("   Sim");
                        SetConsoleTextAttribute(hConsole, 14);
                        centralizarTexto("-> Nao (Sair)");
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                    
                    tecla = getch();
                    if(tecla == 'w' || tecla == 's') {
                        opcaoMenu = !opcaoMenu;
                    } else if(tecla == 'i') {
                        if(opcaoMenu == 0) {
                            main();
                            return;
                        } else {
                            exit(0);
                        }
                    }
                }
            }
        }
    }
}

/* Funções de movimento de inimigos */

// Move o inimigo aleatoriamente na fase 2
void moverInimigo() {
    int direcoes[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int tentativas = 0;
    int dir, novoX, novoY;
    
    while (tentativas < 10) {
        dir = rand() % 4;
        novoX = inimigoX + direcoes[dir][0];
        novoY = inimigoY + direcoes[dir][1];
        
        if (novoX >= 0 && novoX < LIN3 && novoY >= 0 && novoY < COL3 && 
            mapa3[novoX][novoY] != '*' && mapa3[novoX][novoY] != '#' &&
            mapa3[novoX][novoY] != 'D' && mapa3[novoX][novoY] != 'O' &&
            mapa3[novoX][novoY] != '?' && !(novoX == jogadorF3X && novoY == jogadorF3Y)) {
            inimigoX = novoX;
            inimigoY = novoY;
            return;
        }
        tentativas++;
    }
}

// Move o primeiro inimigo verde em direção ao jogador
void moverInimigoVerde() {
    int dx = jogadorF4X - inimigoVX;
    int dy = jogadorF4Y - inimigoVY;
    int novoX = inimigoVX, novoY = inimigoVY;
    
    if (abs(dx) > abs(dy)) {
        novoX += (dx > 0) ? 1 : -1;
    } else {
        novoY += (dy > 0) ? 1 : -1;
    }
    
    if (novoX >= 0 && novoX < LIN4 && novoY >= 0 && novoY < COL4 && 
        mapa4[novoX][novoY] != '*' && mapa4[novoX][novoY] != '?' &&
        mapa4[novoX][novoY] != '=' && !(novoX == jogadorF4X && novoY == jogadorF4Y) &&
        !(novoX == inimigoV2X && novoY == inimigoV2Y)) {
        inimigoVX = novoX;
        inimigoVY = novoY;
        return;
    }
    
    if (abs(dx) <= abs(dy)) {
        novoX = inimigoVX + ((dx > 0) ? 1 : -1);
        novoY = inimigoVY;
        if (novoX >= 0 && novoX < LIN4 && novoY >= 0 && novoY < COL4 && 
            mapa4[novoX][novoY] != '*' && mapa4[novoX][novoY] != '?' &&
            mapa4[novoX][novoY] != '=' && !(novoX == jogadorF4X && novoY == jogadorF4Y) &&
            !(novoX == inimigoV2X && novoY == inimigoV2Y)) {
            inimigoVX = novoX;
            inimigoVY = novoY;
            return;
        }
    }
    
    novoX = inimigoVX;
    novoY = inimigoVY + ((dy > 0) ? 1 : -1);
    if (novoX >= 0 && novoX < LIN4 && novoY >= 0 && novoY < COL4 && 
        mapa4[novoX][novoY] != '*' && mapa4[novoX][novoY] != '?' &&
        mapa4[novoX][novoY] != '=' && !(novoX == jogadorF4X && novoY == jogadorF4Y) &&
        !(novoX == inimigoV2X && novoY == inimigoV2Y)) {
        inimigoVX = novoX;
        inimigoVY = novoY;
    }
}

// Move o segundo inimigo verde em direção ao jogador
void moverInimigoVerde2() {
    int dx = jogadorF4X - inimigoV2X;
    int dy = jogadorF4Y - inimigoV2Y;
    int novoX = inimigoV2X, novoY = inimigoV2Y;
    
    if (abs(dx) > abs(dy)) {
        novoX += (dx > 0) ? 1 : -1;
    } else {
        novoY += (dy > 0) ? 1 : -1;
    }
    
    if (novoX >= 0 && novoX < LIN4 && novoY >= 0 && novoY < COL4 && 
        mapa4[novoX][novoY] != '*' && mapa4[novoX][novoY] != '?' &&
        mapa4[novoX][novoY] != '=' && !(novoX == jogadorF4X && novoY == jogadorF4Y) &&
        !(novoX == inimigoVX && novoY == inimigoVY)) {
        inimigoV2X = novoX;
        inimigoV2Y = novoY;
        return;
    }
    
    if (abs(dx) <= abs(dy)) {
        novoX = inimigoV2X + ((dx > 0) ? 1 : -1);
        novoY = inimigoV2Y;
        if (novoX >= 0 && novoX < LIN4 && novoY >= 0 && novoY < COL4 && 
            mapa4[novoX][novoY] != '*' && mapa4[novoX][novoY] != '?' &&
            mapa4[novoX][novoY] != '=' && !(novoX == jogadorF4X && novoY == jogadorF4Y) &&
            !(novoX == inimigoVX && novoY == inimigoVY)) {
            inimigoV2X = novoX;
            inimigoV2Y = novoY;
            return;
        }
    }
    
    novoX = inimigoV2X;
    novoY = inimigoV2Y + ((dy > 0) ? 1 : -1);
    if (novoX >= 0 && novoX < LIN4 && novoY >= 0 && novoY < COL4 && 
        mapa4[novoX][novoY] != '*' && mapa4[novoX][novoY] != '?' &&
        mapa4[novoX][novoY] != '=' && !(novoX == jogadorF4X && novoY == jogadorF4Y) &&
        !(novoX == inimigoVX && novoY == inimigoVY)) {
        inimigoV2X = novoX;
        inimigoV2Y = novoY;
    }
}

/* Funções da Fase 2 */

// Verifica se o jogador pode se mover para a posição (x,y) na fase 2
int podeMover3(int x, int y) {
    char c;
    if (x < 0 || y < 0 || x >= LIN3 || y >= COL3) return 0;
    c = mapa3[x][y];
    if (c == '*') return 0;
    if (c == 'D' && !portaAberta3) return 0;
    if (c == '?') return 0;
    if (c == '#') {
        system("cls");
        printf("Voce pisou nos espinhos!\n");
        gameOver();
        return 0;
    }
    return 1;
}

// Ativa o botão na fase 2
void ativarBotao() {
    if (jogadorF3X == 11 && jogadorF3Y == 4 && !botaoAtivado) {
        printf("Botao ativado! Barreiras removidas!\n");
        strcpy(mapa3[8], "*                  *");
        int i;
        for (i = 1; i <= 6; i++) {
            if (mapa3[i][17] != '@') {
                mapa3[i][17] = ' ';
            }
        }
        botaoAtivado = 1;
        Sleep(1000);
    }
}

// Interações na fase 2
void interagir3() {
    if (jogadorF3X == 15 && jogadorF3Y == 17) {
        printf("Placa: 'Tem um botao ali, \"O\", fique em cima dele e certamente algo acontecerÃ¡'\n");
        esperarTeclaI();
        return;
    }

    if (abs(jogadorF3X - 1) <= 1 && abs(jogadorF3Y - 17) <= 1 && mapa3[1][17] == '@') {
        chaveColetada3 = 1;
        portaAberta3 = 1;
        mapa3[1][17] = ' ';
        mapa3[16][17] = '=';
        printf("Voce pegou a chave dourada! Porta destrancada.\n");
        esperarTeclaI();
        return;
    }

    printf("Nada para interagir aqui.\n");
    esperarTeclaI();
}

// Imprime o mapa da fase 2
void imprimirMapa3() {
    int i, j;
    system("cls");
    printf("Local: Fase 2\n\n");
    for (i = 0; i < LIN3; i++) {
        for (j = 0; j < COL3; j++) {
            if (i == jogadorF3X && j == jogadorF3Y) {
                printf("&");
            } else if (i == inimigoX && j == inimigoY) {
                SetConsoleTextAttribute(hConsole, 6);
                printf("X");
                SetConsoleTextAttribute(hConsole, 15);
            } else {
                switch(mapa3[i][j]) {
                    case '@':
                        SetConsoleTextAttribute(hConsole, 6);
                        printf("@");
                        SetConsoleTextAttribute(hConsole, 15);
                        break;
                    case 'D':
                        printf(portaAberta3 ? "=" : "D");
                        break;
                    case 'O':
                        printf("O");
                        break;
                    case '?':
                        printf("?");
                        break;
                    case '#':
                        SetConsoleTextAttribute(hConsole, 12);
                        printf("#");
                        SetConsoleTextAttribute(hConsole, 15);
                        break;
                    default:
                        printf("%c", mapa3[i][j]);
                }
            }
        }
        printf("\n");
    }
}

/* Funções da Sala Troll */

// Ativa o botão na sala troll
void ativarBotao4() {
    if (jogadorF4X == 6 && jogadorF4Y == 6 && !botaoAtivado4) {
        printf("Botao ativado! Mas algo parece errado...\n");
        strcpy(mapa4[5], "*    ###          *");
        strcpy(mapa4[6], "*    #O#          *");
        strcpy(mapa4[7], "*    ###          *");
        botaoAtivado4 = 1;
        Sleep(1000);
    }
}

// Verifica movimento na sala troll
int podeMover4(int x, int y) {
    if (x < 0 || y < 0 || x >= LIN4 || y >= COL4) return 0;
    if (mapa4[x][y] == '*' || mapa4[x][y] == '?') return 0;
    if (botaoAtivado4 && (x == 5 || x == 6 || x == 7) && (y >= 5 && y <= 7)) {
        system("cls");
        printf("Voce pisou nos espinhos!\n");
        trollMessageShown = 1;
        gameOver();
        return 0;
    }
    return 1;
}

// Interações na sala troll
void interagir4() {
    if (jogadorF4X == 15 && jogadorF4Y == 6) {
        printf("Placa: 'Tem um botao logo a frente, mas a porta ja esta aberta, o que sera que isso significa?'\n");
        esperarTeclaI();
        return;
    }

    printf("Nada para interagir aqui.\n");
    esperarTeclaI();
}

// Imprime o mapa da sala troll
void imprimirMapa4() {
    int i, j;
    system("cls");
    printf("Local: Sala Secreta\n\n");
    for (i = 0; i < LIN4; i++) {
        for (j = 0; j < COL4; j++) {
            if (i == jogadorF4X && j == jogadorF4Y) {
                printf("&");
            } else if (i == inimigoVX && j == inimigoVY) {
                SetConsoleTextAttribute(hConsole, 10);
                printf("V");
                SetConsoleTextAttribute(hConsole, 15);
            } else if (i == inimigoV2X && j == inimigoV2Y) {
                SetConsoleTextAttribute(hConsole, 10);
                printf("V");
                SetConsoleTextAttribute(hConsole, 15);
            } else {
                switch(mapa4[i][j]) {
                    case 'O':
                        printf("O");
                        break;
                    case '?':
                        printf("?");
                        break;
                    case '=':
                        printf("=");
                        break;
                    case '#':
                        SetConsoleTextAttribute(hConsole, 12);
                        printf("#");
                        SetConsoleTextAttribute(hConsole, 15);
                        break;
                    default:
                        printf("%c", mapa4[i][j]);
                }
            }
        }
        printf("\n");
    }
}

/* Funções da Sala Final */

// Verifica movimento na sala final
int podeMover5(int x, int y) {
    if (x < 0 || y < 0 || x >= LIN5 || y >= COL5) return 0;
    if (mapa5[x][y] == '*' || mapa5[x][y] == '#' || mapa5[x][y] == '?') return 0;
    return 1;
}

// Interações na sala final
void interagir5() {
    if (jogadorF5X == 6 && jogadorF5Y == 21) {
        printf("Placa: 'A jornada esta prestes para acabar'\n");
        esperarTeclaI();
        return;
    }

    printf("Nada para interagir aqui.\n");
    esperarTeclaI();
}

// Imprime o mapa da sala final
void imprimirMapa5() {
    int i, j;
    system("cls");
    printf("Local: Sala Final\n\n");
    for (i = 0; i < LIN5; i++) {
        for (j = 0; j < COL5; j++) {
            if (i == jogadorF5X && j == jogadorF5Y) {
                printf("&");
            } else {
                switch(mapa5[i][j]) {
                    case 'M':
                        SetConsoleTextAttribute(hConsole, 14);
                        printf("M");
                        SetConsoleTextAttribute(hConsole, 15);
                        break;
                    case '>':
                        SetConsoleTextAttribute(hConsole, 12);
                        printf(">");
                        SetConsoleTextAttribute(hConsole, 15);
                        break;
                    case '?':
                        printf("?");
                        break;
                    case '#':
                        SetConsoleTextAttribute(hConsole, 8);
                        printf("#");
                        SetConsoleTextAttribute(hConsole, 15);
                        break;
                    default:
                        printf("%c", mapa5[i][j]);
                }
            }
        }
        printf("\n");
    }
}

/* Funções principais de cada fase */

// Controla a sala final
void iniciarSalaFinal() {
    char comando;
    int novoX, novoY;
    
    while (1) {
        imprimirMapa5();
        printf("\nUse WASD para mover, i para interagir.\n");

        if (jogadorF5X == 1 && jogadorF5Y == 14) {
            printf("Voltando para a Sala Secreta...\n");
            Sleep(1000);
            iniciarSalaTroll();
            return;
        }

        comando = getch();
        novoX = jogadorF5X;
        novoY = jogadorF5Y;

        if (comando == 'w') novoX--;
        else if (comando == 's') novoX++;
        else if (comando == 'a') novoY--;
        else if (comando == 'd') novoY++;
        else if (comando == 'i') {
            interagir5();
            continue;
        }

        if (podeMover5(novoX, novoY)) {
            jogadorF5X = novoX;
            jogadorF5Y = novoY;
        }
    }
}

// Controla a sala troll
void iniciarSalaTroll() {
    char comando;
    int novoX, novoY;
    srand((unsigned)time(NULL));
    
    while (1) {
        imprimirMapa4();
        printf("\nUse WASD para mover, i para interagir.\n");
        
        if (rand() % 2 == 0) {
            moverInimigoVerde();
        }
        if (rand() % 2 == 0) {
            moverInimigoVerde2();
        }

        ativarBotao4();

        if ((jogadorF4X == inimigoVX && jogadorF4Y == inimigoVY) || 
            (jogadorF4X == inimigoV2X && jogadorF4Y == inimigoV2Y)) {
            system("cls");
            printf("Voce foi pego pelo inimigo verde!\n");
            gameOver();
            continue;
        }

        if (jogadorF4X == 1 && jogadorF4Y == 7) {
            printf("Indo para a Sala Final...\n");
            Sleep(1000);
            salaFinalAtivada = 1;
            iniciarSalaFinal();
            return;
        }

        comando = getch();
        novoX = jogadorF4X;
        novoY = jogadorF4Y;

        if (comando == 'w') novoX--;
        else if (comando == 's') novoX++;
        else if (comando == 'a') novoY--;
        else if (comando == 'd') novoY++;
        else if (comando == 'i') {
            interagir4();
            continue;
        }

        if (podeMover4(novoX, novoY)) {
            jogadorF4X = novoX;
            jogadorF4Y = novoY;
        }
    }
}

// Controla a fase 2
void iniciarFase2() {
    char comando;
    int novoX, novoY;
    srand((unsigned)time(NULL));
    
    while (1) {
        imprimirMapa3();
        printf("\nUse WASD para mover, i para interagir.\n");
        
        if (rand() % 5 < 2) {
            moverInimigo();
        }

        ativarBotao();

        if (jogadorF3X == inimigoX && jogadorF3Y == inimigoY) {
            system("cls");
            printf("Voce foi pego pelo inimigo!\n");
            gameOver();
            continue;
        }

        if (portaAberta3 && jogadorF3X == 16 && jogadorF3Y == 17) {
            printf("Indo para a sala secreta...\n");
            Sleep(1000);
            iniciarSalaTroll();
            return;
        }

        comando = getch();
        novoX = jogadorF3X;
        novoY = jogadorF3Y;

        if (comando == 'w') novoX--;
        else if (comando == 's') novoX++;
        else if (comando == 'a') novoY--;
        else if (comando == 'd') novoY++;
        else if (comando == 'i') {
            interagir3();
            continue;
        }

        if (podeMover3(novoX, novoY)) {
            jogadorF3X = novoX;
            jogadorF3Y = novoY;
        }
    }
}

// Desenha o mapa da sala inicial
void desenharMapa1() {
    int i, j;
    system("cls");
    printf("Sala Inicial\n\n");
    for (i = 0; i < LIN1; i++) {
        for (j = 0; j < COL1; j++) {
            if (i == py && j == px) {
                printf("&");
            } else {
                printf("%c", mapa1[i][j]);
            }
        }
        printf("\n");
    }
}

// Verifica interações na sala inicial
void verificarInteracao1() {
    if (py == 2 && px == 13) {
        printf("Se leu isso estas preparado para a abertura!\n");
        leuPlaca = 1;
        mapa1[1][24] = '=';
        portaAberta = 1;
    }
    else if (py == 1 && px == 24 && !portaAberta) {
        printf("Porta trancada! Nao da para passar por aqui.\n");
    }
    esperarTeclaI();
}

// Verifica movimento na sala inicial
int podeMover1(int x, int y) {
    if (x < 0 || x >= COL1 || y < 0 || y >= LIN1) return 0;
    return (mapa1[y][x] == ' ' || mapa1[y][x] == '=');
}

// Controla a sala inicial
void iniciarSala() {
    char tecla;
    int novoX, novoY;
    while (1) {
        desenharMapa1();
        if (py == 1 && px == 24 && mapa1[py][px] == '=') {
            printf("Indo para a vila...\n");
            Sleep(1000);
            iniciarVila();
            return;
        }
        tecla = getch();
        novoX = px;
        novoY = py;
        if (tecla == 'w') novoY--;
        else if (tecla == 's') novoY++;
        else if (tecla == 'a') novoX--;
        else if (tecla == 'd') novoX++;
        else if (tecla == 'i') {
            verificarInteracao1();
            continue;
        }
        if (podeMover1(novoX, novoY)) {
            px = novoX;
            py = novoY;
        }
    }
}

// Imprime o mapa da vila
void imprimirMapa2() {
    int i, j;
    system("cls");
    printf("Local: Vila\n\n");
    for (i = 0; i < LIN2; i++) {
        for (j = 0; j < COL2; j++) {
            if (i == jogadorX && j == jogadorY) {
                printf("&");
            } else if (mapa2[i][j] == '@') {
                SetConsoleTextAttribute(hConsole, 6);
                printf("@");
                SetConsoleTextAttribute(hConsole, 15);
            }
            else if (mapa2[i][j] == 'D') {
                printf(portaAberta2 ? "=" : "D");
            }
            else if (mapa2[i][j] == 'P') {
                printf("P");
            }
            else {
                printf("%c", mapa2[i][j]);
            }
        }
        printf("\n");
    }
}

// Verifica movimento na vila
int podeMover2(int x, int y) {
    if (x < 0 || y < 0 || x >= LIN2 || y >= COL2) return 0;
    if (mapa2[x][y] == '*' || mapa2[x][y] == 'P') return 0;
    return 1;
}

// Verifica se duas posições são adjacentes
int adjacente(int x1, int y1, int x2, int y2) {
    if (x1 == x2 && (y1 == y2+1 || y1 == y2-1)) return 1;
    if (y1 == y2 && (x1 == x2+1 || x1 == x2-1)) return 1;
    return 0;
}

// Interações na vila
void interagir2() {
    int i, j;

    for (i = 0; i < LIN2; i++) {
        for (j = 0; j < COL2; j++) {
            if (mapa2[i][j] == 'D' && adjacente(jogadorX, jogadorY, i, j) && !portaAberta2) {
                printf("Porta trancada! Encontre a chave dourada.\n");
                esperarTeclaI();
                return;
            }
        }
    }

    for (i = 0; i < LIN2; i++) {
        for (j = 0; j < COL2; j++) {
            if (mapa2[i][j] == '@' && adjacente(jogadorX, jogadorY, i, j)) {
                chaveColetada = 1;
                portaAberta2 = 1;
                mapa2[i][j] = ' ';
                mapa2[1][8] = '=';
                printf("\nVoce pegou a chave dourada! Porta destrancada.\n");
                esperarTeclaI();
                return;
            }
        }
    }

    if ((jogadorX == 5 && jogadorY == 1) || (jogadorX == 4 && jogadorY == 1)) {
        printf("\nNPC Bruto: Quer passar? EntÃ£o responda:\n");
        printf("Que dia lanÃ§a o Nintendo Switch 2?\n");
        printf("(1) 3 de junho\n(2) 7 de junho\n(3) 5 de junho\n> ");
        int resp;
        scanf("%d", &resp);
        while(getchar() != '\n');
        if (resp == 3) {
            printf("NPC: ah, nao. Bem, voce merece a chave... Vai la pegar\n");
            mapa2[4][1] = ' ';
        } else {
            printf("NPC: ERROU!\n");
        }
        esperarTeclaI();
        return;
    }

    if ((jogadorX == 7 && jogadorY == 5) || (jogadorX == 6 && jogadorY == 5)) {
        printf(chaveColetada ? 
            "\nNPC: Parabens por abrir a porta!\n" : 
            "\nNPC: A chave esta la em cima, porem o Bruto esta tapando o caminho...\n");
        esperarTeclaI();
        return;
    }

    printf("\nNada para interagir aqui.\n");
    esperarTeclaI();
}

// Controla a vila
void iniciarVila() {
    char comando;
    int novoX, novoY;
    while (1) {
        imprimirMapa2();
        printf("\nUse WASD para mover, i para interagir.\n");
        
        if (portaAberta2 && jogadorX == 1 && jogadorY == 8) {
            printf("Indo para a Fase 2...\n");
            Sleep(1000);
            iniciarFase2();
            return;
        }

        comando = getch();
        novoX = jogadorX;
        novoY = jogadorY;
        if (comando == 'w') novoX--;
        else if (comando == 's') novoX++;
        else if (comando == 'a') novoY--;
        else if (comando == 'd') novoY++;
        else if (comando == 'i') {
            interagir2();
            continue;
        }
        if (podeMover2(novoX, novoY)) {
            jogadorX = novoX;
            jogadorY = novoY;
        }
    }
}

// Função principal
int main() {
    int opcaomenu = 0;
    char tecla;
    
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Introdução do jogo
    printf("Num mundo conturbado, onde dinheiro era o que importava...\n");
    printf("Surgia um cacador inesperado, buscando o tesouro das lendas que sempre almejava.\n");
    printf("Nao era o primeiro aventureiro, outros foram mortos pelo caminho...\n");
    printf("Mas queria viver sonhos de modo mais verdadeiro, mesmo que sozinho!\n");
    printf("Entao se o cacador tiver de morrer, sera apenas o destino!\n\n");
    printf("Pressione 'i' para continuar...\n");
    do {
        tecla = getch();
    } while (tecla != 'i');

    // Menu principal
    while (1) {
        system("cls");
        printf("Botoes:\n");
        printf("W/S = NAVEGAR, I = CONFIRMAR:\n\n");
        
        if (opcaomenu == 0) {
            SetConsoleTextAttribute(hConsole, 14);
            printf("-> Jogar\n");
            SetConsoleTextAttribute(hConsole, 15);
            printf("   Creditos\n");
            printf("   Sair\n");
        } 
        else if (opcaomenu == 1) {
            printf("   Jogar\n");
            SetConsoleTextAttribute(hConsole, 14);
            printf("-> Creditos\n");
            SetConsoleTextAttribute(hConsole, 15);
            printf("   Sair\n");
        }
        else {
            printf("   Jogar\n");
            printf("   Creditos\n");
            SetConsoleTextAttribute(hConsole, 14);
            printf("-> Sair\n");
            SetConsoleTextAttribute(hConsole, 15);
        }

        tecla = getch();
        if (tecla == 'w') {
            opcaomenu--;
            if (opcaomenu < 0) opcaomenu = 2;
        }
        else if (tecla == 's') {
            opcaomenu++;
            if (opcaomenu > 2) opcaomenu = 0;
        }
        else if (tecla == 'i') {
            if (opcaomenu == 0) {
                iniciarSala();
            } 
            else if (opcaomenu == 1) {
                system("cls");
                printf("CREDITOS:\n");
                printf("Desenvolvido por: MANOEL ALVES\n");
                getch();
            }
            else {
                return 0;
            }
        }
    }
}