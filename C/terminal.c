#include <stdio.h>
#include <stdlib.h>

#define TEXT_SPEED 15

static int cursorPos = 1;


void trueSleep(int ms) {
    #ifdef _WIN32
        //Sleep(1000); // Sleep 1 segundo
        Sleep(ms);
    #else
        //sleep(1); // Sleep 1 segundo
        usleep(ms*1000);
    #endif
}

// Um regex que limpa o terminal, muitas vezes mais rápido que o system("clear") e é multiplataforma
void clearTerm() {
    printf("\033[1;1H\033[2J"); 
}

// Imprime espaços pra centralizar a próxima impressão em uma margem
void centerText (int len, int borderPos) {
    for(int i=0; i<((borderPos/2) - (len/2)); i++) {
        printf(" ");
    }
}

// Conta o número de dígitos de um número recursivamente
int digitNum (int num) {
    int digits = 1;

    if (num > 10) {
        return digits + digitNum(num/10);
    }
    else {
        return digits;
    }
}

// Imprime uma mensagem devagar
void printSlow (char string[]) {
    for(int i=0; string[i]!='\0'; i++) {
        if (!(string[i] == ' ' && cursorPos == 1)) printf("%c", string[i]); // Se o caracter for um espaço e cursorPos for 1 (o cursor estiver no início da margem), não imprime o caracter
        fflush(stdout); // Alguns sistemas operacionais colocam a string inteira num buffer e imprimem tudo de uma vez em vez de imprimir um char por vez
        
        if(string[i] != '\n') {
            cursorPos++; // Aumenta a posição do cursor quando imprime um caracter que não é \n
            #ifdef _WIN32
                trueSleep(TEXT_SPEED/10); // O Windows imprime muito mais devagar que o linux, então eu diminui o tempo em 10
            #else
                trueSleep(TEXT_SPEED);
            #endif
        }
        else {
            cursorPos = 1; // Se der um \n, o cursor volta pro início, então muda pra 1
        }
    }
}

// Pega uma string de um arquivo e tira o \n
void getStringFromFile(FILE* file, int max, char* string) {
    fgets(string, max, file);
    string[strcspn(string, "\n")] = 0;
}

// Pega um array de números no arquivo, separados por espaço
void getArrayFromFile(FILE* file, int size, int *array) {
    int currentInt;
    char currentChar;

    fscanf(file, "%*s "); // Lê a string que diz o que o array representa e joga ela fora

    // Lê números até encher o array (com um espaço sobrando) ou até currentChar não ser espaço
    for (int i=0; i<size-1; i++) {
        fscanf(file, "%i%c", &currentInt, &currentChar);

        if (currentChar != ' ') break;
        else array[i] = currentInt;
    }

    // Lê o último int, mas não um char, pra não interferir com a leitura do nome do ataque básico do inimigo
    fscanf(file, "%i", &currentInt); 
    array[size-1] = currentInt;
}
