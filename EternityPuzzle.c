#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NIVEAU 6 // 4-7

// genere le plateau
void generePlateau(int[NIVEAU][NIVEAU][4]);
// recherche la premiere place vide d'une piece (-1 si aucune place)
int premierePlace(int[4]);
// retourne la case mémoire de la case correspondante a la case de coordonné x y z
int* corresp(int[NIVEAU][NIVEAU][4],int, int, int);
// affiche le plateau
void affichePlateau(int[NIVEAU][NIVEAU][4]);
// converti les datas pour l'affichage
char getChar(int);

int main(int argc, char *argv[])
{
  srand(time(NULL));
  printf("Hello Word\n");
  printf("Projet de licence 2\n");
  printf("Par Quentin Germain et Thibault Dupré\n");

  int plateau[NIVEAU][NIVEAU][4];

  generePlateau(plateau);
  affichePlateau(plateau);

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
    case 3: return 'D';
    default: return 'X';
  }
}

void generePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  for (int t=0; t<NIVEAU; t++)
    for (int h=0; h<NIVEAU; h++)
      for (int i=0; i<4; i++)
        plateau[t][h][i] = -1; // init le tableau a vide
  for (int t=0; t<NIVEAU; t++)
    for (int h=0; h<NIVEAU; h++)
      while(premierePlace(plateau[t][h])!=-1) // tant qu'il y a de la place
      {
        int n = rand()%4; // prend une data de maniere random
        int prpl = premierePlace(plateau[t][h]); // recupaire la premiere place vide de la piece
        plateau[t][h][prpl] = n; // met la data dans a cette place
        *corresp(plateau,t,h,prpl) = n; // met aussi la data dans la case correspondante
      }
}

int premierePlace(int piece[4])
{
  for (int t=0; t<4; t++)
    if (piece[t]==-1)
      return t;
  return -1;
}

int* corresp(int plateau[NIVEAU][NIVEAU][4],int x, int y, int z)
{
  switch(z)
  {
    case 0: y = (y==0)? NIVEAU-1: y-1;
            break;
    case 1: x = (x==NIVEAU-1)? 0: x+1;
            break;
    case 2: y = (y==NIVEAU-1)? 0: y+1;
            break;
    case 3: x = (x==0)? NIVEAU-1: x-1;
  }

  z = (z<2)? z+2: z-2;

  return &(plateau[x][y][z]);
}
