/*#############################################################################
#                               Trabalho 5                                    #
#############################################################################*/

/*#####################################
#_____________________________________#
# Nome : Eduardo Niederauer Rodrigues #
#_____________________________________#
#_____________________________________#
# Nome : Matheus Balduino G. dos Reis #
#_____________________________________#
# Programa: clássico jogo Threes.     #
#_____________________________________#
#####################################*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "tela.h"
#include <unistd.h>

#define ALTURA_TELA 600
#define LARGURA_TELA 600

#define QTDHS 10 // Define a quantidade permitida de highscores
#define TAMN 7 // Define o tamanho do nome do usuario

#define transp 0
#define azul 1
#define vermelho 2
#define verde 3
#define amarelo 4
#define preto 5
#define laranja 6
#define rosa 7
#define branco 8
#define marrom 9

//Registro que define uma estrutura para a pontuação nome e pontos.
typedef struct
{
  char nome[TAMN];
  int pontos;
} recorde;

/*#############################################################################
#                        Funções para o mecanismo do jogo                     #
#############################################################################*/

//vetor preenchido com as probabilidades dos números.
void preenche_vetor (int vetor[], int vetor2[], int vetor3[]);

//sorteia o número menor que 6 que será usado no jogo
int sorteio(int vetor[]);

//sorteia o número maior que 6 que será usado no jogo
int sorteio_maior(int sort[]);

//sorteia a probabilidade de ser número maior ou menor
int sorteio_2 (int sort2[]);

//sorteia a posição a ser preenchida na matriz
void posi_matriz (int *l, int *c);

//preenche a matriz randomicamente
void preenche_matriz(int tab[4][4], int sort[]);

//Movimenta o jogo de acordo com a direção indicada.
void movimento(int tab[][4], int *placar, int sort2[],
int sort[], int sortMaior[]);

//Muda as peças para cima e verifica se as peças se unem.
int para_cima(int tab[][4]);

//muda as peças para baixo e verifica se as peças se unem.
int para_baixo ( int tab[][4]);

//muda as peças para a direita e verifica se as peças se unem.
int para_direita ( int tab[][4]);

//muda as peças para a esquerda e verifica se as peças se unem.
int para_esquerda ( int tab[][4]);

//acrescenta carta na primeira linha do jogo.
bool acrescenta_carta_lin1(int tab [][4], int carta_1 , int sort2[]);

//acrescenta carta nova na ultima linha.
bool acrescenta_carta_lin4(int tab [][4], int carta_1 , int sort2[]);

//acrescenta carta nova na primeira coluna.
bool acrescenta_carta_col1(int tab [][4], int carta_1 , int sort2[]);

//acrescenta carta nova na primeira coluna.
bool acrescenta_carta_col4(int tab [][4], int carta_1 , int sort2[]);

//fatora o número para saber quantas vezes vale essa carta comparado ao 3.
int fatorial_carta (int n);

//mostra carta sorteada
int mostra_carta(int sort2[], int sort[], int sortMaior[]);

//Cria o arquivo dos melhores scores.
void highscores(int placar);

// Registra o novo recorde se ele for maior que os anteriores
void novo_recorde(int placar, recorde *pos);

//Mostra os recordes já obtidos.
void tabela_recordes();

/*#############################################################################
#                       Funções com o Alegro, efeitos visuais                 #
#############################################################################*/
//Tela principal do jogo.
void tela_jogo(int tab[][4], int ProximaCarta, int pontacao);
//Tela final.
void tela_jogo_fim(int pontuacao, char nome[TAMN], int letra);
//le teclas do teclado.
char tela_le_tecla(void);

/*#############################################################################
#                                  Main                                       #
#############################################################################*/

int main()
{
  //Cria a janela do jogo
  tela_inicio(LARGURA_TELA, ALTURA_TELA, "Threes in C");
  //variável booleana para parar o jogo,
  //variável pontuação para retornar a pontuação do placar
  int pontuacao = 0;
  int tab[4][4]={0}; int sort[100], sort2[100], sortMaior[100];
  //srand com time, para variação do sorteio, muda o parametro do srand
  srand(time(NULL));
  preenche_vetor(sort,sortMaior, sort2);
  preenche_matriz(tab, sort);
  //char nome[100]="Digite Nome";
  movimento(tab,&pontuacao,sort2,sort,sortMaior);
  return 0;

}
/*#############################################################################
#                             Funções do jogo                                 #
#############################################################################*/

void tela_jogo(int tab[][4], int ProximaCarta, int pontuacao)
{
  int roxoubuntu = tela_cria_cor(0.4, 0.0, 0.4);
  int cinza = tela_cria_cor(0.4, 0.4, 0.4);
  char tab1[50], pro[30], pts[10];
  int x = 70;
  sprintf(pts, "%d", pontuacao);
  sprintf(pro, "%d", ProximaCarta);
  tela_inicia_desenho();
  tela_retangulo(0, 0, LARGURA_TELA, ALTURA_TELA, 5, transp, roxoubuntu);

  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      sprintf(tab1, "%d", tab[i][j]);
      if(tab[i][j] == 0){
        tela_circulo(90 + (j*x),90 + (i*x), 30 , 2, branco, roxoubuntu);
        tela_texto(90 + (j*x),90 + (i*x), 30 , transp, tab1);
      }else{
        tela_circulo(90 + (j*x),90 + (i*x), 30 , 2, branco, cinza);
        tela_texto(90 + (j*x),90 + (i*x), 30 , preto, tab1);
      }
    }
  }

  sprintf(tab1, "%d", pontuacao);
  tela_retangulo(370,350,500,400, 2, branco, cinza);
  tela_texto(420,370, 50 , preto, tab1);
  sprintf(tab1,"Pontuação");
  tela_texto(430,330, 30 , branco, tab1);
  sprintf(pro, "%d", ProximaCarta);
  tela_retangulo(370,120,440,200, 2, branco, cinza);
  tela_texto(403,155, 40 , preto, pro);
  sprintf(pro,"Próxima Carta");
  tela_texto(440,100, 30 , branco, pro);
  sprintf(tab1,"Terminar o jogo: \"P\"");
  tela_texto(230, 500, 30, branco, tab1);
  sprintf(tab1,"Recordes: \"R\"");
  tela_texto(230, 450, 30, branco, tab1);

  tela_linha(0, 600, 600, 600, 4, preto);
  tela_circulo(90, 735, 45, 1, preto, cinza);

  tela_termina_desenho();
  return;
}

void tela_jogo_fim(int pontuacao, char nome[TAMN], int letra)
{
  char fim[10];
  int roxoubuntu = tela_cria_cor(0.4, 0.0, 0.4);
  int cinza = tela_cria_cor(0.4, 0.4, 0.4);
  int poslinhax = (LARGURA_TELA/2)+letra*12;
  tela_inicia_desenho();
  tela_retangulo(0, 0, LARGURA_TELA, ALTURA_TELA, 5, transp, roxoubuntu);
  sprintf(fim, "%d", pontuacao);
  tela_retangulo(500,70,70,300, 2, branco, cinza);
  tela_texto(290,160, 100 , preto, fim);
  tela_texto_dir(300, 375, 20, branco, nome);
  tela_linha(poslinhax, 400, poslinhax + 12, 400, 4, branco);
  tela_termina_desenho();
}

int mostra_carta(int sort2[], int sort[], int sortMaior[])
{
  if(sorteio_2(sort2) == 1){
    int carta_1 = sorteio(sort);
    return carta_1;
  }else{
    int carta_2= sorteio_maior(sortMaior);
    return carta_2;
  }
}

//Função usada para sortear números menores que 6
int sorteio(int sort[])
{
  int a = rand()%100;
  return sort[a];
}

//Função usada para sortear o vetor sortMaior, números maiores que 6.
int sorteio_maior(int sort[])
{
  int x = rand()%100;
  return sort[x];
}

//sorteia qual a probabilidade de ser número maior ou menor que 6 usado nas
//funçoes para sortear e escolher se usa sorteio_maior() ou se usa sorteio().
int sorteio_2 (int sort2[])
{
  int u = rand()%100;
  return sort2[u];
}

//vetor recebe o vetor sort(chance dos menores),
//vetor2 recebe o vetor sortMaior,
//vetor3 recebe o vetor sort2(com as chances do nº maior)
void preenche_vetor (int vetor[], int vetor3[], int vetor2[])
{
  for(int i = 0; i < 100; i ++){// 100% de probabilidades
    if(i>=0 && i<35){
      vetor[i] = 1;
    }else if(i > 34 && i < 70){
      vetor[i] = 2;
    }else if(i > 69 && i < 98){
      vetor[i] = 3;
    }else{
      vetor[i] = 6;
    }
  }
  for(int i = 0; i < 100; i ++){ // 100% de probabilidades
    if(i>=0 && i<50){ // 50 % de chance
      vetor3[i] = 6;
    }else if(i > 49 && i < 80){ // 30% de chance
      vetor3[i] = 12;
    }else if(i > 79 && i < 95){ // 15% de chance
      vetor3[i] = 24;
    }else{ // 5% de chance
      vetor3[i] = 48;
    }
  }
  for(int i = 0; i < 100; i ++){// 100% de probabilidades
    if(i>=0 && i<94){
      vetor2[i] = 1; // 94% de chance de receber o menor.
    }else{
      vetor2[i] = 2; // 6% de chance de receber maior.
    }
  }
}

//sorteia posiçoes para preencher a matriz.
void posi_matriz (int *l, int *c)
{
  *l = rand()%4;
  *c = rand()%4;
}

// Preenche a Matriz aleatóriamente.
void preenche_matriz(int tab[4][4],int sort[])
{
  int li=0, co=0;
  int filled = 0;
  do{
    posi_matriz(&li,&co);
    if(tab[li][co] == 0){
      tab[li][co] = sorteio(sort);
      filled++;
    }
  }while(filled < 9);
}

//Verifica se pode acrescentar carta no tabuleiro e caso permitido, acrescenta.
bool acrescenta_carta_lin1(int tab [][4], int carta_1 , int sort2[])
{
  bool carta = false, linha_cheia = true;
  int linA, colB;
  while(carta == false){
    for(int j = 0; j < 4; j++){
      if(tab[0][j] == 0){
        linha_cheia = false;
      }
    }
    if(linha_cheia == false){
      posi_matriz(&linA, &colB);
      if(tab[0][colB] == 0){
        tab[0][colB] = carta_1;
        carta = true;
      } else {
        carta = false;
      }
    } else {
      return false;
    }
  }
  return true;
}

bool acrescenta_carta_lin4(int tab [][4], int carta_1 , int sort2[])
{
  bool carta = false, linha_cheia = true;
  int linA, colB;
  while(carta == false){
    for(int j = 0; j < 4; j++){
      if(tab[3][j] == 0){
        linha_cheia = false;
      }
    }
    if(linha_cheia == false){
      posi_matriz(&linA, &colB);
      if(tab[3][colB] == 0){
        tab[3][colB] = carta_1;
        carta = true;
      } else {
        carta = false;
      }
    } else {
      return false;
    }
  }
  return true;
}

bool acrescenta_carta_col1(int tab [][4], int carta_1 , int sort2[])
{
  bool carta = false, linha_cheia = true;
  int linA, colB;
  while(carta == false){
    for(int i = 0; i < 4; i++){
      if(tab[i][0] == 0){
        linha_cheia = false;
      }
    }
    if(linha_cheia == false){
      posi_matriz(&linA, &colB);
      if(tab[linA][0] == 0){
        tab[linA][0] = carta_1;
        carta = true;
      } else{
        carta = false;
      }
    } else {
      return false;
    }
  }
  return true;
}

bool acrescenta_carta_col4(int tab [][4], int carta_1 , int sort2[])
{
  bool carta = false, linha_cheia = true;
  int linA, colB;
  while(carta == false){
    for(int i = 0; i < 4; i++){
      if(tab[i][3] == 0){
        linha_cheia = false;
      }
    }
    if(linha_cheia == false){
      posi_matriz(&linA, &colB);
      if(tab[linA][3] == 0){
        tab[linA][3] = carta_1;
        carta = true;
      } else{
        carta = false;
      }
    } else {
      return false;
    }
  }
  return true;
}

//Fatora o valor do número formado no jogo,
//para saber quantas vezes a nova carta pontua.
int fatorial_carta (int n)
{
  if(n == 3){
    return 1;
  }else{
    return fatorial_carta (n/2) + 1;
  }
}

//Função que faz o mecanismo do jogo funcionar.
//Tab passa a tabela do jogo, *placar ponteiro da variável pontuação,
//sort passa qual número menor que 6 entra, ja na probabilidade.
//sort2 passa a probabilidade do maior ou menor entrar,
//sort3 passa qual número maior que 6 entra.
//final 1, 2, 3 e 4 indicam que as cartas não estão mais mexendo.
//carta_1 indica a próxima peça
void movimento(int tab[][4], int *placar, int sort2[], int sort[], int sortMaior[])
{
  int carta_1;
  tela_le_tecla();
  bool fim = true;
  bool acabalogo1 = true, acabalogo2 = true;
  bool acabalogo3 = true, acabalogo4 = true;
  bool primeiraVez = true;

  for(;fim;){
    if(primeiraVez){
      carta_1 = mostra_carta(sort2,sort,sortMaior);
      tela_jogo(tab,carta_1,*placar);
      primeiraVez = false;
    }
    tela_jogo(tab,carta_1,*placar);
    switch(tela_le_tecla())
    {
      case('w'):
      *placar = *placar + para_cima(tab);
      acabalogo1 = acrescenta_carta_lin4(tab,carta_1,sort2);
      carta_1 = mostra_carta(sort2,sort,sortMaior);
      tela_jogo(tab,carta_1,*placar);
      break;
      case('s'):
      *placar = *placar + para_baixo(tab);
      acabalogo2 = acrescenta_carta_lin1(tab,carta_1,sort2);
      carta_1 = mostra_carta(sort2,sort,sortMaior);
      tela_jogo(tab,carta_1,*placar);
      break;
      case('d'):
      *placar = *placar + para_direita(tab);
      acabalogo3 = acrescenta_carta_col1(tab,carta_1,sort2);
      carta_1 = mostra_carta(sort2,sort,sortMaior);
      tela_jogo(tab,carta_1,*placar);
      break;
      case('a'):
      *placar = *placar + para_esquerda(tab);
      acabalogo4 = acrescenta_carta_col4(tab,carta_1,sort2);
      carta_1 = mostra_carta(sort2,sort,sortMaior);
      tela_jogo(tab,carta_1,*placar);
      break;
      case('p'):
      acabalogo1 = false;
      acabalogo2 = false;
      acabalogo3 = false;
      acabalogo4 = false;
      break;
      case('r'):
      tabela_recordes();
      tela_jogo(tab,carta_1,*placar);
      break;

    }
    if((acabalogo1 == false) && (acabalogo2 == false)
    && (acabalogo3 == false) && (acabalogo4 == false)){
      highscores(*placar);
      fim = false;
    }
  }
}

int para_cima(int tab [][4])
{
  int pontos1 = 0, res1 = 0, placar1[4]={0};
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 4; j++){
      int v = tab[i][j];
      if(tab[i][j] == 0 && tab [i+1][j] != 0){
        tab[i][j]=tab[i+1][j];
        tab[i+1][j]=0;
      }else if (tab[i+1][j] + tab[i][j] == 3){
        if((tab[i][j] == 1 && tab[i+1][j] == 2) ||
        (tab[i+1][j] == 1 && tab[i][j] == 2)){
          tab[i][j]+=tab[i+1][j];
          tab[i+1][j]=0;
        }
      }else if(tab[i+1][j] == tab[i][j] && tab[i+1][j] != 0){
        if(tab[i][j] + tab[i+1][j] != 4 &&
        tab[i+1][j] + tab[i][j] != 2){
          tab[i][j]+=tab[i+1][j];
          tab[i+1][j]=0;
          placar1[i] += pontos1 + (pow(3,fatorial_carta(v)));
        }
      }
    }
  }
  for(int k = 0; k < 3; k++){
    res1 += placar1[k];
  }
  return res1;
}

int para_baixo ( int tab[][4])
{
  int pontos2 = 0, res2 = 0, placar2[4]={0};
  for(int i = 2; i >= 0; i--){
    for (int j = 0; j <= 3; j++){
      int v2 = tab[i+1][j];
      if(tab[i+1][j] == 0 && tab[i][j] != 0){
        tab[i+1][j] = tab[i][j];
        tab[i][j] = 0;
      }
      if(tab[i+1][j] + tab[i][j] == 3){
        if((tab[i][j] == 1 && tab[i+1][j] == 2) ||
        (tab[i][j] == 2 && tab[i+1][j] == 1)){
          tab[i+1][j] += tab[i][j];
          tab[i][j] = 0;
        }
      }
      if(tab[i+1][j] == tab[i][j] && tab[i+1][j] != 0){
        if(tab[i][j] + tab[i+1][j] != 4 &&
        tab[i+1][j] + tab[i][j] != 2){
          tab[i+1][j] += tab[i][j];
          tab[i][j] = 0;
          placar2[i] += pontos2 + (pow(3,fatorial_carta(v2)));
        }
      }
    }
  }
  for(int k = 0; k < 3; k++){
    res2 += placar2[k];
  }
  return res2;
}

int para_direita ( int tab[][4])
{
  int pontos3 = 0, res3 = 0, placar3[4]={0};
  for(int i = 0; i < 4; i++){
    for (int j = 2; j >= 0; j--){
      int v3 = tab[i][j+1];
      if(tab[i][j+1] == 0 && tab[i][j] != 0){
        tab[i][j+1] = tab[i][j];
        tab[i][j] = 0;
      }
      if(tab[i][j+1] + tab[i][j] == 3){
        if((tab[i][j] == 1 && tab[i][j+1] == 2) ||
        (tab[i][j+1] == 1 && tab[i][j] == 2)){
          tab[i][j+1] += tab[i][j];
          tab[i][j] = 0;
        }
      }
      if(tab[i][j+1] == tab[i][j] && tab[i][j+1] != 0){
        if(tab[i][j] + tab[i][j+1] != 4 &&
        tab[i][j+1] + tab[i][j] != 2){
          tab[i][j+1] += tab[i][j];
          tab[i][j] = 0;
          placar3[i] += pontos3 + (pow(3,fatorial_carta(v3)));
        }
      }
    }
  }
  for(int k = 0; k < 3; k++){
    res3 += placar3[k];
  }
  return res3;
}

int para_esquerda ( int tab[][4])
{
  int pontos4 = 0, res4 = 0, placar4[4]={0};
  for(int i = 0; i < 4; i++){
    for (int j = 0; j < 3; j++){
      int v4 = tab[i][j];
      if(tab[i][j] == 0 && tab[i][j+1] != 0){
        tab[i][j] = tab[i][j+1];
        tab[i][j+1] = 0;
      }
      if(tab[i][j] + tab[i][j+1] == 3){
        if((tab[i][j] == 1 && tab[i][j+1] == 2) ||
        (tab[i][j+1] == 1 && tab[i][j] == 2)){
          tab[i][j] += tab[i][j+1];
          tab[i][j+1] = 0;
        }
      }
      if(tab[i][j+1] == tab[i][j] && tab[i][j+1] != 0){
        if(tab[i][j] + tab[i][j+1] != 4 &&
        tab[i][j+1] + tab[i][j] != 2){
          tab[i][j] += tab[i][j+1];
          tab[i][j+1] = 0;
          placar4[i] += pontos4 + (pow(3,fatorial_carta(v4)));
        }
      }
    }
  }
  for(int k = 0; k < 4; k++){
    res4 += placar4[k];
  }
  return res4;
}

void highscores(int placar)
{
  int i, shift;
  FILE *tabela;
  recorde melhores[QTDHS];

  if(fopen("highscores.txt","r+") == NULL){
    tabela = fopen("highscores.txt", "w+");
    for(i = 0; i < QTDHS; i++){
      fprintf(tabela, "aaaaaa 0\n");
    }
    fclose(tabela);
  }

  tabela = fopen("highscores.txt","r+w+");

  for(i = 0; i < QTDHS; i++){
    fscanf(tabela,"%s %d", melhores[i].nome, &melhores[i].pontos);
  }

  for(i = 0; i < QTDHS; i++){
    if(placar > melhores[i].pontos){
      for(shift = QTDHS-1; shift >= i; shift--){
        melhores[shift].pontos = melhores[shift-1].pontos;
        strcpy(melhores[shift].nome, melhores[shift-1].nome);
      }
      novo_recorde(placar, &melhores[i]);
      break;
    }
  }

  rewind(tabela); //Volta para o inicio do arquivo
  for(i = 0; i < QTDHS; i++){
    fprintf(tabela, "%s %d\n", melhores[i].nome, melhores[i].pontos);
  }
  fclose(tabela);
  tabela_recordes(placar);
}

// Verifica se a pontuação é melhor que dos antigos records
void novo_recorde(int placar, recorde *pos)
{
  char novo_nome[TAMN];
  for(int i = 0; i < TAMN; i++){
    novo_nome[i] ='\0';
  }

  int letra=0;
  char mod='a';

  while(mod != '\n') {
    mod=tela_le_tecla();
    if (mod=='\b') {
      if (novo_nome[letra]=='\0' && letra>0) {
        letra--;
      }
      novo_nome[letra]='\0';
    }
    else if(mod!='\0' && mod!='\n' && mod != '\0'){
      novo_nome[letra]=mod;
      if (letra<TAMN-2) {
        letra++;
      }
    }
    tela_jogo_fim(placar, novo_nome, letra);
  }
  strcpy(pos->nome, novo_nome);
  pos->pontos = placar;
}

void tabela_recordes()
{
  int roxoubuntu = tela_cria_cor(0.4, 0.0, 0.4);
  FILE *tabela;
  recorde melhores[QTDHS];
  int i, y = 50;
  char tab1[50];
  char saida = '0';
  char recname[7];
  char recptos[20];
  bool fim = true;

  if(fopen("highscores.txt","r+") == NULL){
    tabela = fopen("highscores.txt", "w+");
    for(i = 0; i < QTDHS; i++){
      fprintf(tabela, "aaaaaa 0\n");
    }
    fclose(tabela);
  }

  tabela=fopen("highscores.txt", "r+");

  //leitura dos highscores
  for (i = 0; i < QTDHS; i++) {
    fscanf(tabela,"%s %d", melhores[i].nome, &melhores[i].pontos);
  }

  tela_inicia_desenho();
  //Limpa a tela
  tela_retangulo(0, 0, LARGURA_TELA, ALTURA_TELA, 5, transp, roxoubuntu);

  for (i = 0; i < QTDHS; i++) {
    //gera as strings a serem desenhadas
    sprintf(recname, "%s", melhores[i].nome);
    sprintf(recptos, "%d", melhores[i].pontos);
    tela_texto_dir(50, y, 30, branco, recname);
    tela_texto_dir(300, y, 30, branco, recptos);
    sprintf(tab1,"Aperte \"ENTER\" para sair!");
    tela_texto_dir(50, 500, 30, branco, tab1);
    y += 30;
  }
  tela_termina_desenho();

  fclose(tabela);
  do{
    //condicao de saida do jogo
    saida=tela_le_tecla();
    if (saida =='\n') {
      fim=false;
    }
   }while(fim);
}
