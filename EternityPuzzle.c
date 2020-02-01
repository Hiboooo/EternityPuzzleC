#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NIVEAU 6 // 4-7

void generePlateau(int[NIVEAU][NIVEAU][4]);
void affichePlateau(int[NIVEAU][NIVEAU][4]);
char getChar(int);

int main(int argc, char *argv[])
{
  srand(time(NULL));
  printf("Hello Word\n");
  printf("Projet de licence 2\n");
  printf("Par Quentin Germain et Thibault Dupré\n");

  int tab[NIVEAU][NIVEAU][4];
  generePlateau(tab);
  affichePlateau(tab);

  putchar('\n');
}

void affichePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  printf("\n   ");
  for (int t=0; t<NIVEAU; t++)
    printf(" %d ",t);
  putchar('\n');
  for (int t=0; t<NIVEAU; t++)
  {
    printf("\n   ");
    for (int h=0; h<NIVEAU; h++)
      printf(" %c ",getChar(plateau[h][t][0]));
    printf("\n %c ",'a'+t);
    for (int h=0; h<NIVEAU; h++)
      printf("%c#%c",getChar(plateau[h][t][3]),getChar(plateau[h][t][1]));
    printf("\n   ");
    for (int h=0; h<NIVEAU; h++)
      printf(" %c ",getChar(plateau[h][t][2]));
    putchar('\n');
  }
}

char getChar(int t)
{
  switch (t)
  {
    case 0: return 'A';
    case 1: return 'B';
    case 2: return 'C';
    default: return 'D';
  }
}

void generePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  for (int t=0; t<NIVEAU; t++)
    for (int h=0; h<NIVEAU; h++)
      for (int i=0; i<4; i++)
        plateau[t][h][i] = (rand() % (3 - 0 + 1)) + 0;;
}
