#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NIVEAU 6 // 4-7

void generePlateau(int[NIVEAU][NIVEAU][4]);
int* corresp(int[NIVEAU][NIVEAU][4],int, int, int);
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

  tab[5][5][3] = 1;
  affichePlateau(tab);

  *corresp(tab,5,5,3) = 1;
  affichePlateau(tab);

  putchar('\n');
}

void affichePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  printf("\n   ");
  for (int t=0; t<NIVEAU; t++)
    printf(" %d ",t+1);
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
        plateau[t][h][i] = 0;
}

int* corresp(int plateau[NIVEAU][NIVEAU][4],int x, int y, int z)
{
  if (z==0)
    return &(plateau[x][(y-1)%NIVEAU][(z+2)%4]);
  else if (z==1)
    return &(plateau[(x+1)%NIVEAU][y][(z+2)%4]);
  else if (z==2)
    return &(plateau[x][(y+1)%NIVEAU][(z+2)%4]);
  else if (z==3)
    return &(plateau[(x-1)%NIVEAU][y][(z+2)%4]);
  else
    return NULL;
}
