#include <stdio.h>

#define NIVEAU 6 // 4-7

void generePlateau(int[NIVEAU][NIVEAU][4]);
void affichePlateau(int[NIVEAU][NIVEAU][4]);

int main(int argc, char *argv[])
{
  printf("Hello Word\n");
  printf("Projet de licence 2\n");
  printf("Par Quentin Germain et Thibault Dupré");

  int tab[NIVEAU][NIVEAU][4];
  generePlateau(tab);
  affichePlateau(tab);

}

void affichePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  putchar('\n');
  printf("   ");
  for (int t=0; t<)
  for (int t=0; t<NIVEAU; t++)
    for (int h=0; h<NIVEAU; h++)
      for (int i=0; i<4; i++)
        printf("%d",plateau[t][h][i]);
}

void generePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  for (int t=0; t<NIVEAU; t++)
    for (int h=0; h<NIVEAU; h++)
      for (int i=0; i<4; i++)
        plateau[t][h][i] = 1;
}
