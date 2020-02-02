#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct CoordPiece CoordPiece;
struct CoordPiece{int x; int y;};

int NIVEAU = 4;

// genere le plateau
void generePlateau(int[NIVEAU][NIVEAU][4]);
// recherche la premiere place vide d'une piece (-1 si aucune place)
int premierePlace(int[4]);
// retourne la case mémoire de la case correspondante a la case de coordonné x y z
int* corresp(int[NIVEAU][NIVEAU][4],int, int, int);
// affiche le plateau pour ide et windows
void affichePlateauAvant(int[NIVEAU][NIVEAU][4]);
char getChar(int);
// affiche le plateau avec mise en forme et couleur
void affichePlateau(int[NIVEAU][NIVEAU][4]);
void getCharN(int[NIVEAU][NIVEAU][4],int, int, int, char[10]);
//echange des pieces
void echangePiece(int[NIVEAU][NIVEAU][4],int,int,int,int);
// fait tourner la piece
void rotationPiece(int[NIVEAU][NIVEAU][4],int,int);
// -1 si pas terminer
int puzzleTermine(int[NIVEAU][NIVEAU][4]);
// jeu
int choixJoueur();
void choisirEtiquette(int[NIVEAU][NIVEAU][4],CoordPiece*);
void tournerJoueur(int[NIVEAU][NIVEAU][4]);
void echangerJoueur(int[NIVEAU][NIVEAU][4]);


int main(int argc, char *argv[])
{
  srand(time(NULL));
  printf("Hello Word\n");
  printf("Projet de licence 2\n");
  printf("Par Quentin Germain et Thibault Dupré\n");

  do
  {
    printf("\nQu'elle est la taille de votre plateau (2-7) : ");
    scanf("%d",&NIVEAU);
  } while(NIVEAU<2||NIVEAU>7);

  int plateau[NIVEAU][NIVEAU][4];

  generePlateau(plateau);

  do
  {
    affichePlateau(plateau);
    if (choixJoueur()==1)
      tournerJoueur(plateau);
    else
      echangerJoueur(plateau);
  } while (puzzleTermine(plateau)==-1);

  affichePlateau(plateau);
  printf("\n\nBien Jouer mon cher, tu as gagné!!!");

  putchar('\n');
}

int choixJoueur()
{
  int choix;
  do
  {
    printf("Voulez-vous faire tourner une pièce ou en échanger deux ?\n");
    printf("1. Tourner\n2. Echanger\nVotre choix (1/2) : ");
    scanf("%d", &choix);
  } while (choix!=1&&choix!=2);
  return choix;
}

void choisirEtiquette(int plateau[NIVEAU][NIVEAU][4],CoordPiece* cp)
{
  printf("\nchoisis ta piece :");
  int choix;
  do
  {
    affichePlateau(plateau);
    printf("\nVotre nombre (1-%d) : ",NIVEAU);
    scanf("%d", &choix);
  } while (choix<1||choix>NIVEAU);
  cp->x = choix;
  char lettre;
  do
  {
    affichePlateau(plateau);
    printf("Votre lettre (a-%c) : ",'a'+NIVEAU-1);
    scanf("%c", &lettre);
  } while (lettre<'a'||lettre>'a'+NIVEAU-1);
  cp->y = lettre-'a'+1;
}

void tournerJoueur(int plateau[NIVEAU][NIVEAU][4])
{
  CoordPiece cp;
  choisirEtiquette(plateau,&cp);
  rotationPiece(plateau,cp.x-1,cp.y-1);
}

void echangerJoueur(int plateau[NIVEAU][NIVEAU][4])
{
  CoordPiece cp[2];
  choisirEtiquette(plateau,cp);
  choisirEtiquette(plateau,cp+1);
  echangePiece(plateau,cp[0].x-1,cp[0].y-1,cp[1].x-1,cp[1].y-1);
}

void affichePlateauAvant(int plateau[NIVEAU][NIVEAU][4])
{
  printf("\n-------------------------\n   ");
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
  printf("-------------------------\n");
}
char getChar(int t)
{
  if (t>=0&&t<=3)
    return 'A'+t;
  return 'X';
}

void affichePlateau(int plateau[NIVEAU][NIVEAU][4])
{
  printf("\n");
  char* doub[6] = {"\u2554","\u2557","\u255D","\u255A","\u2550","\u2551"};
  char* simp[7] = {"\u250C","\u2510","\u2518","\u2514","\u2500","\u2502","\u253C"};
  printf("\n     ");
  for (int t=0; t<NIVEAU; t++)
    printf(" %s%s%s%s%s ",doub[0],doub[4],doub[4],doub[4],doub[1]);
  printf("\n     ");
  for (int t=0; t<NIVEAU; t++)
    printf(" %s %d %s ",doub[5],t+1,doub[5]);
  printf("\n     ");
  for (int t=0; t<NIVEAU; t++)
    printf(" %s%s%s%s%s ",doub[3],doub[4],doub[4],doub[4],doub[2]);

  for (int t=0; t<NIVEAU; t++)
  {
    printf("\n     ");
    for (int h=0; h<NIVEAU; h++)
      printf("  %s%s%s  ",simp[0],simp[4],simp[1]);
    printf("\n     ");
    for (int h=0; h<NIVEAU; h++)
    {
      char temp[17];
      getCharN(plateau,h,t,0,temp);
      printf("  %s%s%s  ",simp[5],temp,simp[5]);
    }
    simp[0] = "\u250C"; // wtf...
    printf("\n%s%s%s%s%s",doub[0],doub[4],doub[4],doub[4],doub[1]);
    for (int h=0; h<NIVEAU; h++)
      printf("%s%s%s%s%s%s%s",simp[0],simp[4],simp[6],simp[4],simp[6],simp[4],simp[1]);
    printf("\n%s %c %s",doub[5],'a'+t,doub[5]);
    for (int h=0; h<NIVEAU; h++)
    {
      char temp[17];
      getCharN(plateau,h,t,3,temp);
      printf("%s%s%s#%s",simp[5],temp,simp[5],simp[5]);
      getCharN(plateau,h,t,1,temp);
      printf("%s%s",temp,simp[5]);
    }
    printf("\n%s%s%s%s%s",doub[3],doub[4],doub[4],doub[4],doub[2]);
    for (int h=0; h<NIVEAU; h++)
      printf("%s%s%s%s%s%s%s",simp[3],simp[4],simp[6],simp[4],simp[6],simp[4],simp[2]);
    printf("\n     ");
    for (int h=0; h<NIVEAU; h++)
    {
      char temp[17];
      getCharN(plateau,h,t,2,temp);
      printf("  %s%s%s  ",simp[5],temp,simp[5]);
    }
    printf("\n     ");
    for (int h=0; h<NIVEAU; h++)
      printf("  %s%s%s  ",simp[3],simp[4],simp[2]);
  }
  printf("\n");
}
void getCharN(int plateau[NIVEAU][NIVEAU][4],int x, int y, int z, char temp[17])
{
  temp[0] = '\e';
  temp[1] = '[';
  temp[2] = '4';
  if (plateau[x][y][z]==*corresp(plateau,x,y,z))
    temp[3] = '3';
  else
    temp[3] = '7';
  temp[4] = 'm';
  temp[5] = '\e';
  temp[6] = '[';
  temp[7] = '3';

  temp[8] = '0';

  temp[9] = ';';
  temp[10] = '1';
  temp[11] = 'm';
  temp[12] = 'A'+plateau[x][y][z];
  temp[13] = '\e';
  temp[14] = '[';
  temp[15] = '0';
  temp[16] = 'm';
  temp[17] = '\0';

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
  for (int t=0; t<1000; t++)
    echangePiece(plateau,rand()%NIVEAU,rand()%NIVEAU,rand()%NIVEAU,rand()%NIVEAU);
  for (int t=0; t<1000; t++)
    rotationPiece(plateau,rand()%NIVEAU,rand()%NIVEAU);
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

void echangePiece(int plateau[NIVEAU][NIVEAU][4],int x1,int y1,int x2,int y2)
{
  int temp[4];
  for (int t=0; t<4; t++)
    temp[t] = plateau[x1][y1][t];
  for (int t=0; t<4; t++)
    plateau[x1][y1][t] = plateau[x2][y2][t];
  for (int t=0; t<4; t++)
    plateau[x2][y2][t] = temp[t];
}

void rotationPiece(int plateau[NIVEAU][NIVEAU][4],int x, int y)
{
  int temp[4];
  for (int t=0; t<4; t++)
    temp[t] = plateau[x][y][t];
  for (int t=0; t<4; t++)
    plateau[x][y][(t+1)%4] = temp[t];
}

int puzzleTermine(int plateau[NIVEAU][NIVEAU][4])
{
  for (int t=0; t<NIVEAU; t++)
    for (int h=0; h<NIVEAU; h++)
      for (int i=0; i<4; i++)
        if (plateau[t][h][i]!=*corresp(plateau,t,h,i))
          return -1;
  return 0;
}
