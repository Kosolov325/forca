#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "forca.h"
#include <locale.h>
#define TAMANHO_PALAVRA 20
#define MAX_LEN 200
#define BUFFER_SIZE 1000

char palavrasecreta[TAMANHO_PALAVRA];
char chutes[26];

int chutesdados = 0;
int stop = 0;
int vez = 1;

typedef struct Player {
  int points;
  char name[50];
} player;

player p1;
player p2;
player vencedor;
player perdedor;

int acabou = 0;

int letraexiste(char letra) {
    for(int j = 0; j < strlen(palavrasecreta); j++) {
        if(letra == palavrasecreta[j]) {
            return 1;
        }
    }
    return 0;
}

int chuteserrados() {
    int erros = 0;

    for(int i = 0; i < chutesdados; i++) {
        if(!letraexiste(chutes[i])) {
            erros++;
        }
    }
    return erros;
}

int enforcou() {
    return chuteserrados() >= 5;
}

int ganhou() {
    for(int i = 0; i < strlen(palavrasecreta); i++) {
        if(!jachutou(palavrasecreta[i])) {
            return 0;
        }
    }
    return 1;
}

int validaNomes(){
    for(int i = 0; i<strlen(p1.name);i++){
        if(isalpha(p1.name[i]) == 0){
            printf("\nOs nomes nao podem conter numeros nem caracteres especiais!\n");
            return 0;
        }
    }
    for(int i = 0; i<strlen(p2.name);i++){
        if(isalpha(p2.name[i]) == 0){
            printf("\nOs nome nao podem conter numeros nem caracteres especiais!\n");
            return 0;
        }
    }
    return 1;
}

void abertura() {
     int valid = 0;
     while(valid == 0){
        printf("\n/****************/\n");
        printf("\n/ Jogo da Forca */\n");
        printf("\n/****************/\n");

        printf("\nDigite o nome do primeiro jogador:");
        scanf("%s", &p1.name);
        printf("\nDigite o nome do segundo jogador:");
        scanf("%s", &p2.name);
        p1.points = 0;
        p2.points = 0;
        valid = validaNomes();
     }
}

void trocarVez(){
    if (vez==1){
        vez=2;
    }
    else{
        vez=1;
    }
}
void chuta() {
    char chute;
    printf("Qual letra? ");
    scanf("%s", &chute);

    if(letraexiste(chute)) {
        printf("Voce acertou: a palavra tem a letra %c\n\n", chute);
        if (vez == 1){
            p1.points++;
        }
        else{
            p2.points++;
        }
    } else {
        printf("\nVoce errou: a palavra NAO tem a letra %c\n\n", chute);
    }

    chutes[chutesdados] = chute;
    chutesdados++;
    trocarVez();
}

int jachutou(char letra) {
    int achou = 0;
    for(int j = 0; j < chutesdados; j++) {
        if(chutes[j] == letra) {
            achou = 1;
            break;
        }
    }
    return achou;
}

void desenhaforca() {

    int erros = chuteserrados();

    printf("Vez do jogador %s \n", (vez==1? p1.name : p2.name));
    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (erros>=1?'(':' '), (erros>=1?'_':' '), (erros>=1?')':' '));
    printf(" |      %c%c%c  \n", (erros>=2?'\\':' '), (erros>=2?'|':' '), (erros>=2?'/': ' '));
    printf(" |       %c     \n", (erros>=3?'|':' '));
    printf(" |      %c %c   \n", (erros>=4?'/':' '), (erros>=4?'\\':' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("\n\n");

    for(int i = 0; i < strlen(palavrasecreta); i++) {

        if(jachutou(palavrasecreta[i])) {
            printf("%c ", palavrasecreta[i]);
        } else {
            printf("_ ");
        }

    }
    printf("\n");
}

void escolhepalavra() {
    char line[1024];
    int count = 0;

    FILE* f;

    f = fopen("palavras.txt", "r");
    if(f == 0) {
        printf("Banco de dados de palavras nao disponivel\n\n");
        exit(1);
    }

     while (fgets(line , sizeof(line) , f )!= NULL)
   {
      count++;

    }

    srand(time(NULL));
    int r = rand() % count + 1;
    count = 0;

    rewind(f);
    while (fgets(line , sizeof(line) , f )!= NULL)
    {
      count++;
      if(count == r){
        int size = strlen(line);
        strcat(palavrasecreta, line);
        palavrasecreta[size - 1] = NULL;
        break;
      }

    }

    fclose(f);
}


void adicionapalavra() {
    char quer;

    printf("\nVoce deseja adicionar uma nova palavra no jogo (S/N)?");
    scanf(" %c", &quer);

    if(quer == 'S') {
        char novapalavra[TAMANHO_PALAVRA];

        printf("Digite a nova palavra, em letras maiusculas: ");
        scanf("%s", novapalavra);

        FILE* f;

        f = fopen("palavras.txt", "r+");
        if(f == 0) {
            printf("Banco de dados de palavras nao disponível\n\n");
            exit(1);
        }

        fprintf(f, "%s\n", novapalavra);

        fclose(f);
    }
}

void continuar(){
    char cont;
    printf("\nDeseja voltar ao menu? (S/N)");
    scanf("%s", &cont);
    if (cont == 'S' || cont == 's'){
        stop = 0;
    }
    else{
        stop = 1;
    }
}

void checarGanhadores(){
    if (p1.points > p2.points ){
             vencedor = p1;
             perdedor = p2;
             acabou = 1;
        }
        else if(p1.points == p2.points){
            printf("O jogo terminou em empate!");
            acabou = 2;
        }
        else{
            vencedor = p2;
            perdedor = p1;
            acabou = 1;
        }
}

int jogo(){
    abertura();
    escolhepalavra();

    do {
        desenhaforca();
        chuta();
    } while (!ganhou() && !enforcou());

    if(ganhou()) {
        checarGanhadores();


        printf("\nO vencendor e %s\n", vencedor.name);

        printf("\nParabens, voce ganhou!\n\n");

        printf("       ___________      \n");
        printf("      '._==_==_=_.'     \n");
        printf("      .-\\:      /-.    \n");
        printf("     | (|:.     |) |    \n");
        printf("      '-|:.     |-'     \n");
        printf("        \\::.    /      \n");
        printf("         '::. .'        \n");
        printf("           ) (          \n");
        printf("         _.' '._        \n");
        printf("        '-------'       \n\n");

    } else {
        checarGanhadores();

        printf("\nPuxa, %s foi enforcado!\n", perdedor.name);

        printf("\nO vencendor e %s\n", vencedor.name);

        printf("A palavra era **%s**\n\n", palavrasecreta);

        printf("       ___________      \n");
        printf("      '._==_==_=_.'     \n");
        printf("      .-\\:      /-.    \n");
        printf("     | (|:.     |) |    \n");
        printf("      '-|:.     |-'     \n");
        printf("        \\::.    /      \n");
        printf("         '::. .'        \n");
        printf("           ) (          \n");
        printf("         _.' '._        \n");
        printf("        '-------'       \n\n");

    }

    if (acabou == 2){
        continuar();
        return;
    }
    else{
      for(int i=0;i<strlen(vencedor.name);i++){
                vencedor.name[i] = toupper(vencedor.name[i]);
            }
      adicionarRanking(vencedor.name, 1);
      continuar();
    }
}

int menu(){
        int option = 0;
        printf("\n||===========================================================||\n");
        printf("\n||                                                           ||\n");
        printf("\n||          _____   _____    _____    _____       __         ||\n");
        printf("\n||         |       |     |  |     |  |           |  |        ||\n");
        printf("\n||         |_____  |     |  |_____/  |          |    |       ||\n");
        printf("\n||         |       |     |  |   |    |         |______|      ||\n");
        printf("\n||         |       |     |  |    |   |        |        |     ||\n");
        printf("\n||         |       |_____|  |     |  |_____  |          |    ||\n");
        printf("\n||                                                           ||\n");
        printf("\n||===========================================================||\n");
        printf("\n1 - Jogar");
        printf("\n2 - Ver Ranking");
        printf("\n3 - Cadastrar Plavras");
        printf("\n4 - Creditos");
        printf("\n5 - Sair");
        printf("\nEscolha uma das opcoes? (1-5)");
        scanf("%d", &option);

        return option;
}

void print_image(FILE *fptr)
{
    char read_string[MAX_LEN];

    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
        printf("%s",read_string);
}

int adicionarRanking(char name[50], int points){
   char line[1024];
   char s1[10];
   int count = 0;
   int countTmp = 0;
   int exist = 0;

   FILE *file;
   FILE * fTemp;

   file = fopen("ranking.txt", "r+");
   while (fgets(line , sizeof(line) , file )!= NULL)
   {
      count++;
      if (strstr(line , name)!= NULL){
          exist = 1;

          for(int i = 0; i<strlen(line); i++){
               if(isdigit(line[i]) == 1){
                    for(int a = 0; a<sizeof(s1); a++){
                        if (isdigit(s1[a]) == 0){
                            s1[a] = line[i];
                            break;
                        }
                    }
                }
            }
            break;
         }
    }
    if (exist == 1){
        int i = atoi(s1);
        char s2[10];
        fTemp = fopen("replace.tmp", "w");
        points += i;
        sprintf(s2, "%d", points);
        strcat(name, " = ");
        strcat(name, s2);
        strcat(name, "\n");

        rewind(file);
         while ((fgets(line, sizeof(line), file)) != NULL)
        {
            countTmp++;
            if (countTmp == count){
                fputs(name, fTemp);
            }
            else{
                fputs(line, fTemp);
            }
        }

        fclose(file);
        fclose(fTemp);

        remove("ranking.txt");
        rename("replace.tmp", "ranking.txt");
    }
    else{
        fprintf(file, "%s = %d \n", name, points);
        fclose(file);
    }

   }

void verRanking(){
    int c;
    FILE *file;
    file = fopen("ranking.txt", "r");
    printf("\n");
    printf("=====RANKING=====\n");

    if (file) {
        while ((c = getc(file)) != EOF){
            putchar(c);
        }

    fclose(file);
    }
    printf("\n=================\n");
    continuar();
}

void creditos(){
    char *filename = "creditos.txt";
    FILE *fptr = NULL;

    if((fptr = fopen(filename,"rw+")) == NULL)
    {
        fprintf(stderr,"Nao foi possivel encontrar o arquivo %s\n",filename);

    }

    print_image(fptr);
    fclose(fptr);
}


int main() {
    setlocale(LC_ALL, "");

    while (stop == 0){
        int option = menu();
        switch (option)
        {
        case 1:
            if(acabou == 0){
                jogo();
                continue;
            }
            else{
                printf("\nO jogo ja acabou!, renincie o programa!");
                continue;
            }
        case 2:
            verRanking();

            continue;
        case 3:
            adicionapalavra();
            continue;
        case 4:
            creditos();
            continuar();
            continue;
        case 5:
            stop = 1;
        }
    }

}
