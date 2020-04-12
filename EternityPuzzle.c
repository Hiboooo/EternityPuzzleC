
/***
 *    ------------------------------------------------------------------------------------------------
 *    ----------d88888b-d888888b-d88888b-d8888b.-d8b---db-d888888b-d888888b-db----db------------------
 *    ----------88'-----`~~88~~'-88'-----88--`8D-888o--88---`88'---`~~88~~'-`8b--d8'------------------
 *    ----------88ooooo----88----88ooooo-88oobY'-88V8o-88----88-------88-----`8bd8'-------------------
 *    ----------88~~~~~----88----88~~~~~-88`8b---88-V8o88----88-------88-------88---------------------
 *    ----------88.--------88----88.-----88-`88.-88--V888---.88.------88-------88---------------------
 *    ----------Y88888P----YP----Y88888P-88---YD-VP---V8P-Y888888P----YP-------YP---------------------
 *    ------------------------------------------------------------------------------------------------
 *    ------------------------------------------------------------------------------------------------
 *    ---------------d8888b.-db----db-d88888D-d88888D-db------d88888b-------.o88b.--------------------
 *    ---------------88--`8D-88----88-YP--d8'-YP--d8'-88------88'----------d8P--Y8--------------------
 *    ---------------88oodD'-88----88----d8'-----d8'--88------88ooooo------8P-------------------------
 *    ---------------88~~~---88----88---d8'-----d8'---88------88~~~~~------8b-------------------------
 *    ---------------88------88b--d88--d8'-db--d8'-db-88booo.-88.----------Y8b--d8--------------------
 *    ---------------88------~Y8888P'-d88888P-d88888P-Y88888P-Y88888P-------`Y88P'--------------------
 *    ------------------------------------------------------------------------------------------------
 *    ------------------------------------------------------------------------------------------------
 *    d8888b.--.d8b.--d8888b.-------.d88b.------d888b-------d88888b-d888888b------d888888b----d8888b.-
 *    88--`8D-d8'-`8b-88--`8D------.8P--Y8.----88'-Y8b------88'-----`~~88~~'------`~~88~~'----88--`8D-
 *    88oodD'-88ooo88-88oobY'------88----88----88-----------88ooooo----88------------88-------88---88-
 *    88~~~---88~~~88-88`8b--------88----88----88--ooo------88~~~~~----88------------88-------88---88-
 *    88------88---88-88-`88.------`8P--d8'-db-88.-~8~------88.--------88------------88----db-88--.8D-
 *    88------YP---YP-88---YD-------`Y88'Y8-VP--Y888P-------Y88888P----YP------------YP----VP-Y8888D'-
 *    ------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* compteurs d'actions */
long double COUNT_STEP_E = -1;
long double COUNT_STEP_R = -1;

/* taille du plateau */
int SIZE = 5;

/* fonctions de base du jeu */
void videBuff(); // fonction de vidage du buffer
int init(); // premiere interface (pour choisir entre joueur ou utiliser les algorithmes)
void play(); // interface et gestion du jeu
void generePlateau(int[SIZE][SIZE][4]); // generation d'un plateau melange
int premierePlace(int[4]); // fonction intermediaire de generePlateau qui pour une piece donnee
// retourne la premiere place non remplie de celle-ci
int* corresp(int[SIZE][SIZE][4],int, int, int); // fonction qui retourne l'addresse de l'emplacement correspondant d'un emplacement donne
void rotationPiece(int[SIZE][SIZE][4],int,int); // rotation d'une piece
void echangePiece(int[SIZE][SIZE][4],int,int,int,int); // echange de 2 pieces
int countConstraintRespect(int[SIZE][SIZE][4]); // retourne le nombre de contraintes respectees
void affichePlateauAme(int[SIZE][SIZE][4]); // affiche le plateau
void getCharN(int[SIZE][SIZE][4],int, int, int, char[10]); // fonction intermediaire de affichePlateauAme

/* fonctions de base de l'algorithme */
void algorithmes(); // interface et debut de gestion des algorithmes
int strCmp(char*,char*); // meme fonction que strcmp de string.h
void mesurePerformance(int,int); // fonction de mesure de performances
void runAlgo(char*,int); // interface et gestion pour ALGO
void algorithmeSimple(int[SIZE][SIZE][4]); // la fonction pour l'algorithme simple
int coincide(int[SIZE][SIZE][4],int,int,int,int); // fonction intermediaire de l'algorithme simple
// permettant de savoir si une piece donnee placee a une place donnee respecte les contraintes de cette place

/* structure de donnees et fonctions pour l'algorithme de backtracking avec kd-arbre */
typedef struct
{
  int nbRot; //nombre de rotations a appliquer a la piece
  int* posPiece; // position de la piece
} action;
typedef struct _tree *tree;
struct _tree
{
  tree parent; // adresse du parent
  action act; // l'action a faire
  int nbConsResp; // le nombre de contraintes respectees
  int nbChild; // le nombre de fils
  tree* child; // les fils
};
void algorithmeBacktracking(int[SIZE][SIZE][4]); // la fonction pour l'algorithme
/* fonction de controle de la structure*/
void treeNew(tree,action,int); // genere une branche
tree createTrunk(int); // genere le tronc
void treeNewChild(tree,int); // genere les fils
void treePrint(tree); // affiche une branche
void treeDestroy(tree); // detruit la branche et les sous branches de l'arbre fourni
/* fonction de tris des fils*/
void treeChildSortBulle(tree); // trie le premier fils sachant que tous les autres sont deja tries
void treeChildSortFusion(tree); // trie les fils
void _treeChildSortFusion(tree,int,int); // fonction intermediaire de treeChildSortFusion
void _fusion(tree,int,int,int); // fonction intermediaire de treeChildSortFusion
/* fonction intermediaire de l'algorithme*/
int* coincide2(int[SIZE][SIZE][4],int[2],int[2]); // nouvelle fonction qui prend en compte qu'une piece
// peut correspondre a une place donnee plusieurs fois
int countNbChild(int[SIZE][SIZE][4],int[2]); // compte les nombres d'enfants
void treeGenereChild(int[SIZE][SIZE][4],tree,int[2]); // genere les enfants
void treeRefreshConstraint(tree); // fonction qui refresh les contraintes de l'arbre,
// partant de la branche actuelle jusqu'a qu'il n'y ait plus besoin ou qu'on soit arrive au tronc
int deadLeaf(tree); // regarde si une branche n'est plus viable
void treePrintCons(tree); // affiche la branche et les sous branches de l'arbre fournit
void _treePrintCons(tree,int); // fontion intermediaire de treePrintCons

int main(int argc, char *argv[])
{
  printf("\nBonjour, bienvenue dans EternityPuzzleC\n");
  printf("Projet de licence 2\n"); // petit texte de presentation
  printf("Par Quentin Germain et Thibault Dupre\n");
  srand(time(NULL));
  char t[2] = {0};
  do
  {
    if (init()==1) play();
    else algorithmes();
    printf("\nRecommencer (O/N): ");
    fgets(t,2,stdin); // utilisation de fgets pour securiser les entrés clavier
    videBuff(); // lorsqu'une entrer est faite le tampon doit toujours etre pris en compte
  } while (t[0]!='N');
  putchar('\n');
  return EXIT_SUCCESS;
}

void videBuff()
{
  //while (getchar()!='\n');
  scanf("%*[^\n]");
  getchar();
}
int init()
{
  char choix[2] = {0};
  do
  {
    printf("\nQue voulez-vous faire ?");
    printf("\n1. Lancer un partie");
    printf("\n2. Decouvrir les algorithmes");
    printf("\nVotre choix (1/2) : ");
    fgets(choix,2,stdin);
    videBuff();
  } while(choix[0]!='1'&&choix[0]!='2');
  return choix[0]-'0';
}
void play()
{
  printf("\nTuto:\nPour faire tourner une piece d'un quart de tour,\n"
         "Veillez accoller l'indicateur en abcisse avec celui en ordonnee.\n"
         "\tEx: '1a'\n" // petit tuto
         "Pour echanger deux pieces,\n"
         "La meme procedure mais pour les deux piece.\n"
         "\tEx: '1b2a'\n");
  char t[2] = {0};
  do // choix de la taille du plateau
  {
    printf("\nQuelle est la taille de votre plateau (2-7) : ");
    fgets(t,2,stdin);
    videBuff();
  } while(t[0]<'2'||t[0]>'7');
  SIZE = t[0]-'0';
  int plateau[SIZE][SIZE][4];
  generePlateau(plateau); // generation du plateau
  COUNT_STEP_E =    // initialisation des indiquateurs pour initialiser
  COUNT_STEP_R = 0; // le systeme qui compte les echanges et les rotations
  do // la boucle de jeu
  {
    affichePlateauAme(plateau); // affichage du plateau
    char input[5] = {0};
    printf(">>> ");
    for (int s=0; s<4; s++) // recuperation des entrees du joueur plus complexe que
    // l'utilisation de fgets pour pouvoir avoir une mutli entree
      if ((input[s] = getchar())=='\n')
        break;
      else
        if (s==3)
          videBuff();
    int ech = 0; // variable permettant de savoir quelle action faire
    if (input[2]>='1'&&input[2]<='0'+SIZE)
      if (input[3]>='a'&&input[3]<='a'+SIZE)
        ech = 1; // le joueur veut faire un echange
    if (input[0]>='1'&&input[0]<='0'+SIZE)
      if (input[1]>='a'&&input[1]<='a'+SIZE)
      {
        if (ech)
          echangePiece(plateau,input[0]-'1',input[1]-'a',input[2]-'1',input[3]-'a');
        else
          rotationPiece(plateau,input[0]-'1',input[1]-'a');
      }
  } while (countConstraintRespect(plateau)<2*SIZE*SIZE); // tant que le plateau n'est pas resolu continuer
  affichePlateauAme(plateau); // affiche la plateau resolu
  printf("\n\nBien Joué mon cher, " // et flate le joueur
         "\navec un total de %d rotations et %d échanges,"
         "\nTU AS GAGNE!!!\n",(int)COUNT_STEP_R,(int)COUNT_STEP_E);
}
void generePlateau(int plateau[SIZE][SIZE][4])
{
  for (int t=0; t<SIZE; t++)
    for (int h=0; h<SIZE; h++)
      for (int i=0; i<4; i++)
        plateau[t][h][i] = -1; // initialisation de toutes les cases a -1
  for (int t=0; t<SIZE; t++)
    for (int h=0; h<SIZE; h++) // pour chaque piece
      while(premierePlace(plateau[t][h])!=-1) // si cette piece n'est pas completement remplie
      {
        int prpl = premierePlace(plateau[t][h]); // on stock la place restante
        plateau[t][h][prpl] =
        *corresp(plateau,t,h,prpl) = rand()%4;
        // on stock une valeur random a cette place et a la place correspondante
      }
  for (int t=0; t<SIZE; t++)
    for (int h=0; h<SIZE; h++) // on melange le plateau
    {
      if (rand()%10!=0)
        echangePiece(plateau,t,h,rand()%SIZE,rand()%SIZE);
      if (rand()%10!=0)
        for (int i=rand()%4; i>0; i--)
          rotationPiece(plateau,t,h);
    }
}
int premierePlace(int piece[4])
{
  for (int t=0; t<4; t++) // pour chaque case de la piece
    if (piece[t]==-1) // si une case n'est pas remplie
      return t; // retourne son indice
  return -1; // sinon retourne -1
}
int* corresp(int plateau[SIZE][SIZE][4],int x, int y, int z)
{
  // cette fonction retourne la case correspondante
  switch(z) // en fonction de l'orientation on modifie x et y
  {
    case 0: y = (y==0)? SIZE-1: y-1; break;
    case 1: x = (x==SIZE-1)? 0: x+1; break;
    case 2: y = (y==SIZE-1)? 0: y+1; break;
    case 3: x = (x==0)? SIZE-1: x-1;
  }
  z = (z+2)%4; // on inverse l'orientation
  return &(plateau[x][y][z]); // on retourne le pointeur correspondant
}
void echangePiece(int plateau[SIZE][SIZE][4],int x1,int y1,int x2,int y2)
{
  if (x1<0||x1>=SIZE||y1<0||y1>=SIZE) // si les composantes sont mauvaises on fait rien
    return;
  if (x2<0||x2>=SIZE||y2<0||y2>=SIZE)
    return;
  if (COUNT_STEP_E!=-1) // si l'indiquateur d'echange est demarre
    COUNT_STEP_E++; // on indique un echange
  // et on echange les pieces
  int temp[4] = {0};
  for (int t=0; t<4; t++)
    temp[t] = plateau[x1][y1][t];
  for (int t=0; t<4; t++)
    plateau[x1][y1][t] = plateau[x2][y2][t];
  for (int t=0; t<4; t++)
    plateau[x2][y2][t] = temp[t];
}
void rotationPiece(int plateau[SIZE][SIZE][4],int x, int y)
{
  if (x<0||x>=SIZE||y<0||y>=SIZE) // si les composantes sont mauvaises on fait rien
    return;
  if (COUNT_STEP_R!=-1) // si l'indiquateur de rotation est demarre
    COUNT_STEP_R++;  // on indique une rotation
  //et on fait tourner la piece
  int temp[4] = {0};
  for (int t=0; t<4; t++)
    temp[t] = plateau[x][y][t];
  for (int t=0; t<4; t++)
    plateau[x][y][(t+1)%4] = temp[t];
}
int countConstraintRespect(int plateau[SIZE][SIZE][4])
{
  int count = 0;
  for (int t=0; t<SIZE; t++)
    for (int h=0; h<SIZE; h++)
      for (int i=0; i<4; i++) // pour chaque case
        if (plateau[t][h][i]==*corresp(plateau,t,h,i)) // si les valeurs sont egals
          count++; // on incremente
  return count/2;
  // on retourne le resultat divise par 2, car on a compte chaque correspondance 2 fois
}
void affichePlateauAme(int plateau[SIZE][SIZE][4])
{
  // ne pas ouvrir XD
  printf("\n");
  // caractere unicode
  // https://en.wikipedia.org/wiki/Box_Drawing_(Unicode_block)
  char* doub[6] = {"\u2554","\u2557","\u255D","\u255A","\u2550","\u2551"};
  char* simp[7] = {"\u250C","\u2510","\u2518","\u2514","\u2500","\u2502","\u253C"};
  printf("\n     ");
  for (int t=0; t<SIZE; t++)
    printf(" %s%s%s%s%s ",doub[0],doub[4],doub[4],doub[4],doub[1]);
  printf("\n     ");
  for (int t=0; t<SIZE; t++)
    printf(" %s %d %s ",doub[5],t+1,doub[5]);
  printf("\n     ");
  for (int t=0; t<SIZE; t++)
    printf(" %s%s%s%s%s ",doub[3],doub[4],doub[4],doub[4],doub[2]);

  for (int t=0; t<SIZE; t++)
  {
    printf("\n     ");
    for (int h=0; h<SIZE; h++)
      printf("  %s%s%s  ",simp[0],simp[4],simp[1]);
    printf("\n     ");
    for (int h=0; h<SIZE; h++)
    {
      char temp[17];
      getCharN(plateau,h,t,0,temp);
      printf("  %s%s%s  ",simp[5],temp,simp[5]);
    }
    printf("\n%s%s%s%s%s",doub[0],doub[4],doub[4],doub[4],doub[1]);
    for (int h=0; h<SIZE; h++)
      printf("%s%s%s%s%s%s%s",simp[0],simp[4],simp[6],simp[4],simp[6],simp[4],simp[1]);
    printf("\n%s %c %s",doub[5],'a'+t,doub[5]);
    for (int h=0; h<SIZE; h++)
    {
      char temp[17];
      getCharN(plateau,h,t,3,temp);
      printf("%s%s%s#%s",simp[5],temp,simp[5],simp[5]);
      getCharN(plateau,h,t,1,temp);
      printf("%s%s",temp,simp[5]);
    }
    printf("\n%s%s%s%s%s",doub[3],doub[4],doub[4],doub[4],doub[2]);
    for (int h=0; h<SIZE; h++)
      printf("%s%s%s%s%s%s%s",simp[3],simp[4],simp[6],simp[4],simp[6],simp[4],simp[2]);
    printf("\n     ");
    for (int h=0; h<SIZE; h++)
    {
      char temp[17];
      getCharN(plateau,h,t,2,temp);
      printf("  %s%s%s  ",simp[5],temp,simp[5]);
    }
    printf("\n     ");
    for (int h=0; h<SIZE; h++)
      printf("  %s%s%s  ",simp[3],simp[4],simp[2]);
  }
  printf("\n");
}
void getCharN(int plateau[SIZE][SIZE][4],int x, int y, int z, char temp[17])
{
  // sequence ansi
  // https://en.wikipedia.org/wiki/ANSI_escape_code#Platform_support
  sprintf(temp,"\e[40m\e[30;1m%c\e[0m",'A'+plateau[x][y][z]);
  if (plateau[x][y][z]==*corresp(plateau,x,y,z))
    temp[3] = '3'; // jaune
  else
    temp[3] = '7'; // blanc
}

void algorithmes()
{
  printf("\nTuto:\nTrois commandes s'offrent a vous:\n"
         "\tALGO 'taille du plateau'\n"
         "\tALGO 'taille du plateau' 'taille de l'echantillon'\n"
         "\tEXIT\n"
         "La premiere affichera le plateau melange et ensuite le plateau resolu.\n" // petit tuto
         "La taille devra etre entre 2 et 7 pieces.\n"
         "\tEx: 'ALGO 4' ou 'ALGO 7'\n"
         "La deuxieme affichera le temps, le nombre de rotation et d'echange total et moyen.\n"
         "Cette commande n'est pas bloquante, Si vous demandez quelque chose de trop long, vous devrez quitter vous meme (ctrl+C)\n"
         "\tEx: 'ALGO 4 30' ou 'ALGO 2 100000' 'ALGO 7 1'\n"
         "Pour finir, sachez qu'un algorithmes de backtracking avec kd-arbre a aussi ete mis en place.\n"
         "Mais celui-ci est moins performant et semble encore avoir quelques defauts.\n"
         "Si cela ne vous fait pas peur, remplacez 'ALGO' par 'ALGO2'\n\n");
  char t[20] = {0};
  do
  {
    printf(">>> ");
    for (int s=0; s<20; s++)
      if ((t[s] = getchar())=='\n')
        break;
      else
        if (s==19)
          videBuff();
    if (strCmp(t,"ALGO ")) // si le debut du texte est egal a "ALGO "
      runAlgo(t+5,1); // on va sur l'algorithme simple en oubliant les premiers caracteres
    else if (strCmp(t,"ALGO2")) // sinon, si le debut du texte est egal a "ALGO2"
      runAlgo(t+6,2); // on va sur l'algorithme complexe en oubliant les premiers caracters
  } while(!strCmp(t,"EXIT")); // tant que le debut du texte est egal a "EXIT"
}
int strCmp(char *str, char *val)
{
  for (int t=0; t<5&&val[t]!='\0'&&str[t]!='\0'; t++)
  // tant qu'on arrive pas sur une fin de chaine on compare
      if (str[t]!=val[t])
        return 0;
  return 1;
}
void mesurePerformance(int sizeEchantillon, int whatAlgo)
{
  double countTime = 0; // compteur de temps
  long double countstepR = 0; // compteur de rotations
  long double countstepE = 0; // compteur d'echanges
  clock_t debut, fin; // variable tampon pour le temps

  int plateau[SIZE][SIZE][4];

  for (int h=0; h<sizeEchantillon; h++) // pour tous les lances demander
  {
    generePlateau(plateau); // on genere un plateau
    COUNT_STEP_E = 0; // on demarre les indiquateurs
    COUNT_STEP_R = 0;
    debut = clock(); // on stock l'heure
    if (whatAlgo == 2) // suivant l'algo on va faire l'un ou l'autre
      algorithmeBacktracking(plateau);
    else
      algorithmeSimple(plateau);
    fin = clock(); // on stock l'heure
    countstepE += COUNT_STEP_E; // et on ajoute aux compteurs
    countstepR += COUNT_STEP_R;
    countTime += ((double)fin-debut)/CLOCKS_PER_SEC;
  }

  printf("total: temps %.8fs",countTime); // affichage des totaux
  printf("\t(ROT: %13.2LF ECH: %13.2LF)\n",countstepR,countstepE);
  countTime/=sizeEchantillon;
  countstepE/=sizeEchantillon;
  countstepR/=sizeEchantillon;
  printf("moyen: temps %.8fs",countTime); // affichage des moyennes
  printf("\t(ROT: %13.2LF ECH: %13.2LF)\n",countstepR,countstepE);
}
void runAlgo(char *str, int whatAlgo)
{
  if (str[0]<'2'||str[0]>'7') // si le premier char ne represente pas une taille acceptable et s'arrete
    return;
  SIZE = str[0]-'0'; // on stock la taille
  if (str[1]!=' '||str[2]<'1'||str[2]>'9')
  // si le char suivant n'est pas un espace ou si le char encore suivant n'est pas un nombre
  // on lance l'algo sur un plateau
  {
    int plateau[SIZE][SIZE][4];
    generePlateau(plateau);
    affichePlateauAme(plateau);
    if (whatAlgo == 2) // suivant l'algo on va faire l'un ou l'autre
      algorithmeBacktracking(plateau);
    else
      algorithmeSimple(plateau);
    affichePlateauAme(plateau);
  }
  else
  {
    int nb=0; // sinon on calcule le nombre (sans atoi())
    for (int t=0; str[2+t]!='\0'; t++)
      if (str[2+t]<'0'||str[2+t]>'9') break;
      else nb = nb*10+(str[2+t]-'0');
    if (nb) mesurePerformance(nb,whatAlgo); // et on lance la fonction
  }
}
void algorithmeSimple(int data[SIZE][SIZE][4])
{
  int pieceCoincide;
  for (int y=0; y<SIZE; y++) // pour toute la largeur
  {
    if (y==0) // si y==0 c'est que l'on recommence l'algo, on reprend donc une nouvelle graine
      echangePiece(data,SIZE-1,SIZE-1,rand()%SIZE,rand()%SIZE); // on place une piece random en derniere place
    for (int x=0, continueNext=1; x<SIZE && continueNext==1; x++) // pour toute la longeur et tant que continueNext == 1
    {
      continueNext=0; // on met continueNext a 0 (il repassera a un seulement si on peut continuer)

      // je recherche une piece correspondante
      for (int yRech=SIZE-1; yRech>=y && continueNext==0; yRech--) // pour toutes les cases non verouillees
        for (int xRech=SIZE-1;                                     // en commencant par la fin
             xRech>=(yRech==y?x:0) && continueNext==0;  // la ternaire permet de ne pas prendre de piece verrouillee
             xRech--)                                   // si on recherche sur la meme hauteur
          {
            pieceCoincide = coincide(data,x,y,xRech,yRech); // on verifie si la piece potentielle coincide bien dans la position
            if (pieceCoincide!=0) // si elle coincide
            {
              continueNext=1; // on peut continuer
              for (int t = pieceCoincide%4; t>0; t--)
                rotationPiece(data,xRech,yRech); // on fait les modifications
              echangePiece(data,x,y,xRech,yRech);
            }
          }

      if (continueNext==0) // si on peut pas continuer on repart du tout debut!
        y = -1;
    }
  }
}
int coincide(int data[SIZE][SIZE][4], int xPos, int yPos, int xPie, int yPie)
{
  int neso[] = {(yPos==0)?0:1,  // les contraintes a respecter en fonction de la postion...
                (xPos==SIZE-1)?1:0,
                (yPos==SIZE-1)?1:0,
                (xPos==0)?0:1};
  int nbContrainte = 0;
  for (int t=0; t<4; t++)
    nbContrainte+=neso[t]; // on compte le nombre de contraintes a respecter
  int valide;
  for (int t=0; t<4; t++) // pour chaque rotation que peut prendre la piece
  {
    valide = nbContrainte; // on stock dans valide le nombre de contrainte
    for (int h=0; h<4; h++) // pour chaque case de la piece
      if (neso[h]==1) // si cette contrainte doit etre respectee
        if (*corresp(data,xPos,yPos,h) == data[xPie][yPie][(h-t)<0?h-t+4:h-t]) // et si elle est respectee (la ternaire sert de modulo)
          valide--; // on decremente
    if (valide==0) // si toutes les contraintes devant etre respectees sont respectees
      return t+4; // on retourne le nombre de rotation (le +4 permet de gerer le cas ou aucune rotation est necessaire)
  }
  return 0;
  // sinon on retourne 0 (qui sinifie que le piece ne correspond pas, et pas qu'elle a besoin de 0 rotation pour correspondre)
}

void algorithmeBacktracking(int plateau[SIZE][SIZE][4])
{
  tree t = createTrunk(
    countConstraintRespect(plateau) // on cree le tronc avec le nombre de contraintes actuelement respectees
  );
  tree trunk = t; // on stock le tronc
  int s=1;
  int pos[2];

  while (s<SIZE*SIZE) { // tant que l'on a pas reussi a atteidnre la fin
      pos[0] = s%SIZE; // on calcule la position actuelle
      pos[1] = s/SIZE;

      treeNewChild(t, // on genere le nombre d'enfant
        countNbChild(plateau,pos)
      );

      if (t->nbChild!=0) { // si il y en a plus de 0
        treeGenereChild(plateau,t,pos); // on genere les enfants
        treeChildSortFusion(t); // on les tris
      }

      treeRefreshConstraint(t); // on refresh les contraintes

      // on remonte l'arbre tant qu'on est pas au tronc et que
      // la branche actuelle n'est plus le meilleur fils de la branche parente
      while (t->parent!=NULL && (t!=t->parent->child[0] || deadLeaf(t)==1))
      {
        s--; // le curseur et les positions s'adaptent
        pos[0] = s%SIZE;
        pos[1] = s/SIZE;
        for (int u=0; u<4-t->act.nbRot; u++)
          rotationPiece(plateau,pos[0],pos[1]); // on remet les plateaux en place
        echangePiece(plateau,pos[0],pos[1],t->act.posPiece[0],t->act.posPiece[1]);
        t = t->parent; // et la branche parent devient la nouvelle branche courante
      }

      // ensuite on redescend jusqu'a retrouver une branche sans fils
      while (t->nbChild!=0)
      {
        s++; // le curseur s'adapte
        t = t->child[0]; // le meilleur fils deviens la branch courante
        echangePiece(plateau,pos[0],pos[1],t->act.posPiece[0],t->act.posPiece[1]);
        for (int u=0; u<t->act.nbRot; u++) // on reconstruit le plateau
          rotationPiece(plateau,pos[0],pos[1]);
        pos[0] = s%SIZE; // les positions s'adaptent
        pos[1] = s/SIZE;
      }
  }
  treeDestroy(trunk); // on detruit tout l'arbre
}
void treePrintCons(tree t)
{
  _treePrintCons(t,0); // appel la fonction recurante
}
void _treePrintCons(tree t, int space)
{
    printf(" %d \n", t->nbConsResp); // affiche le nombre de contrainte respecter
    if (t->nbChild!=0) // si il a des fils
      for (int s=0; s<t->nbChild; s++) { // pour chaque fils
        for (int u=0; u<=space; u++) // on ajoute un decalge de plus en plus grand grace a space
          printf(" | ");
        _treePrintCons(t->child[s],space+1); // on rappelle la fonction
    }
}
int deadLeaf(tree t)
{
  for (int v=0; v<t->nbChild; v++)
    if (t->child[v]->nbConsResp==0)
      return 0;
  return 1;
}
void treeRefreshConstraint(tree t)
{
  if (t->nbChild==0) { // si il n'y a pas d'enfant
    t->nbConsResp = -1; // on detruit la branche
    t=t->parent; // et le parent devient la branche courante
  }
  do { // tant qu'on est pas arrive au tronc
    treeChildSortBulle(t); // on trie le premier fils avec tous les autres fils deja tries
    t->nbConsResp = t->child[0]->nbConsResp; // le meilleur fils remonte son nombre de contrainte au parent
    // si tout les fils sont a -1, le -1 remonte donc
    t=t->parent; // et le parent devient la branche courante
  } while(t!=NULL);
}
void treeGenereChild(int data[SIZE][SIZE][4], tree t, int pos[2])
{
  int posParc[2];
  int *coinTab;
  int count = 0;
  for (posParc[1]=pos[1];posParc[1]<SIZE;posParc[1]++) // pour chaque piece possible
    for (posParc[0]=(posParc[1]==pos[1]?pos[0]:0);posParc[0]<SIZE;posParc[0]++)// (on oublie donc les pieces verouiller)
    {
      coinTab = coincide2(data,pos,posParc);
      if (coinTab[0]!=0) // si la piece coincide
        for (int s=1; s<coinTab[0]+1; s++) // pour chacune de ses rotations qui respectent les contraintes
        {
          action act = (action){coinTab[s],(int*)malloc(2*sizeof(int))}; // on cree un objet action avec la rotation
          act.posPiece[0] = posParc[0]; // et les positions
          act.posPiece[1] = posParc[1];

          echangePiece(data,pos[0],pos[1],posParc[0],posParc[1]);
          for (int u=0; u<coinTab[s]; u++) // on echange et tourne la piece
            rotationPiece(data,pos[0],pos[1]);

          treeNew(t->child[count++],act,countConstraintRespect(data)); // on calcule le nombre de contraintes quelle respecte


          for (int u=0; u<4-coinTab[s]; u++)
            rotationPiece(data,pos[0],pos[1]); // et on remet en place
          echangePiece(data,pos[0],pos[1],posParc[0],posParc[1]);
        }
      free(coinTab);
    }
}
int countNbChild(int data[SIZE][SIZE][4], int pos[2])
{
  int count = 0;
  int posParc[2];
  for (posParc[1]=pos[1];posParc[1]<SIZE;posParc[1]++)
    for (posParc[0]=(posParc[1]==pos[1]?pos[0]:0);posParc[0]<SIZE;posParc[0]++)
    {
      int* coinc = coincide2(data,pos,posParc);
      count += coinc[0];
      free(coinc);
    }
  return count;
}
int* coincide2(int data[SIZE][SIZE][4], int pos[2], int pie[2])
{
  int nbPieVal[] = {0,0,0,0,0};
  // la premiere place va stocker le nombre de rotation possible
  // les autres case stock les rotations

  // la fonctionnement est le meme que coincide()
  int neso[] = {(pos[1]==0)?0:1,
                (pos[0]==SIZE-1)?1:0,
                (pos[1]==SIZE-1)?1:0,
                (pos[0]==0)?0:1};

  int nbContrainte = 0;
  for (int t=0; t<4; t++)
    nbContrainte+=neso[t];

  int pieceValide;
  for (int t=0; t<4; t++)
  {
    pieceValide = nbContrainte;
    for (int h=0; h<4; h++)
      if (neso[h]==1)
        if (*corresp(data,pos[0],pos[1],h) == data[pie[0]][pie[1]][(h-t)<0?h-t+4:h-t])
          pieceValide--;
    if (pieceValide==0)
      nbPieVal[++nbPieVal[0]] = t;
  }

  // genere un tableau de taille nbPieVal[0]+1
  // la premiere place stock la taille
  // les autres stock les rotations
  if (nbPieVal[0]==0)
    return (int*)calloc(1,sizeof(int));
  int* res = (int*)malloc(nbPieVal[0]+1);
  res[0] = nbPieVal[0];
  for (int t=1; t<=res[0]; t++)
    res[t] = nbPieVal[t];
  return res;
}
void treeNew(tree t, action act, int nbConsResp)
{
  t->act = act; // initialisation de la branche
  t->nbConsResp = nbConsResp;
  t->child = NULL;
  t->nbChild = 0;
}
tree createTrunk(int nbConsResp)
{
  tree t = (tree)malloc(sizeof(struct _tree)); // creation du tronc
  treeNew(t,(action){0,(int*)calloc(2,sizeof(int))},nbConsResp);
  t->parent = NULL; // on met le parent a NULL
  return t;
}
void treeNewChild(tree t, int sizeChild)
{
  if (sizeChild==0)
    t->nbChild = 0;
  else
  {
    t->child = (tree*)calloc(sizeChild,sizeof(tree));
    for (int c=0; c<sizeChild; c++)
    {
      t->child[c] = (tree)malloc(sizeof(struct _tree));
      t->child[c]->parent = t;
    }
    t->nbChild = sizeChild;
  }
}
void treePrint(tree t)
{
  printf("-\nADR: %p\n-\n",t);
  printf("ADR-Parent: %p\n",t->parent);
  printf("Action:\tRot:%d\n\tPosX:%d\n\tPosY:%d\n",t->act.nbRot,
          t->act.posPiece[0],t->act.posPiece[1]);
  printf("nbConstraintRespect: %d\n",t->nbConsResp);
  printf("nbChild: %d\n",t->nbChild);
  printf("ADR-Child: %p\n",t->child);
  if (t->nbChild>0)
    for (int s=0; s<t->nbChild; s++)
      printf(" %p: %d\n",t->child[s],t->child[s]->nbConsResp);
  printf("-\n\n");
}
void treeDestroy(tree t)
{
  if (t->child!=NULL)
  {
    for (int c=t->nbChild-1; c>=0; c--)
      treeDestroy(t->child[c]);
    free(t->child);
  }
  free(t->act.posPiece);
  free(t);
}
void treeChildSortBulle(tree t)
{
  tree tmp;
  for (int s=0; s<t->nbChild-1; s++)
    if (t->child[s]->nbConsResp < t->child[s+1]->nbConsResp)
    {
      tmp = t->child[s];
      t->child[s] = t->child[s+1];
      t->child[s+1] = tmp;
    }
    else
      break;
}
void treeChildSortFusion(tree t)
{
  _treeChildSortFusion(t,0,t->nbChild-1);
}
void _treeChildSortFusion(tree t, int d, int f)
{
  if (d!=f)
  {
    int m=(f+d)/2;
    _treeChildSortFusion(t,d,m);
    _treeChildSortFusion(t,m+1,f);
    _fusion(t,d,m,f);
  }
}
void _fusion(tree t, int d, int m, int f)
{
  tree *tmp = (tree*)malloc((m-d+1)*sizeof(tree));
  int comptG = d;
  int comptD = m+1;

  for (int s=d; s<=m; s++)
    tmp[s-d] = t->child[s];

  for (int s=d; s<=f; s++)
    if (comptG == m+1)
      break;
    else if (comptD == f+1 ||
            tmp[comptG-d]->nbConsResp > t->child[comptD]->nbConsResp)
      t->child[s] = tmp[comptG++-d];
    else
      t->child[s] = t->child[comptD++];

  free(tmp);
}
