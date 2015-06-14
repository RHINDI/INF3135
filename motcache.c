/** @fichier        motcache.c
 *  @discription    Resolution de mots caches
 *
 *  Ce fichier contain la fonction main() du programe motcache.
 *
 *
 *  @Cours          : INF3135
 *  @Group          : 50
 *  @prof           : Jacques Berger
 *  @auteur         : Rhindi Youssef
 *  @code Permanent : RHIY20087605
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*--------------------------definitions des prototypes des fonctions.--------------------------*/

void LoadPuzzle(FILE *file);

int removeNewLineChar(char *str);

int sumOfAsciiCodes(char str[], int length, int d);

int verticalSearch(char *word, int wordLength, int nbrWord);

void horizontalSearch(char *word, int wordLength, int nbrWord);

int horiVertiSearch(char *p, char *t, int pLen, int d, int nbrWord, int caller);

void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer);

void horiVertiFillWord(int nbrWord);

void ShowResult();

void validerUsage(int argc);

int searchWords(FILE *file, int wordLength, int nbrWord);

/*
 * Une structure a quatre champs, emballe comme une seule entite
 * pour conserver les informations de reperage de pour chaque mots
 * trouver.
 */
typedef struct
{
   int pos;
   int wordLength;
   int caller;
   char *puzzelPointer;
} FIstr;

/*-------------Variables globales - les valeurs sont connues dans le programme.-------------*/

char puzzle[13][13];
char word[13 + 1];
FIstr fistr;
FIstr structArray[50];
int patternFingerprint;

int main(int argc, char **argv)
{
   FILE *file;
   int wordLength, nbrWord;
   nbrWord = 0;
   wordLength = 0;

   validerUsage(argc);
   file = fopen(argv[1], "r");
   if (file == NULL)
   {
      printf("Erreur fichier source\n %s\n", strerror(errno));
      exit(1);
   }

   LoadPuzzle(file);
   nbrWord = searchWords(file, wordLength, nbrWord);

   if (fclose(file) == EOF)
   {
      printf("Erreur lors de la fermeture du fichier.\n");
      return 1;
   }
   horiVertiFillWord(nbrWord);
   ShowResult();
   return 0;
}

/*------------------------------Functions are defined  below------------------------------*/

/** @description
 *
 *  @param d                 =>
 *  @param nbrWord           =>
 *  @param caller            =>
 *  @retour nombre des mots
 */

int searchWords(FILE *file, int wordLength, int nbrWord)
{
   while (fgets(word, 13 + 1, file) != NULL && word[0] != '\n')
   {
      wordLength = removeNewLineChar(word);
      patternFingerprint = sumOfAsciiCodes(word, wordLength, 1);

      if (!(verticalSearch(word, wordLength, nbrWord)))
      {
         horizontalSearch(word, wordLength, nbrWord);

      }
      ++nbrWord;
   }
   return nbrWord;
}


/** @description validation des arguments
 *
 *  validerUsage()
 *  valide que le programe a bien recu en parametre une fichier.txt
 *
 *
 *  @param argc     => parametre qu'a recu le programme
 *  @retour rien
 */

void validerUsage(int argc)
{
   if (argc != 2)
   {
      printf("Utilisation du programme main.c :\nmain.c <fichier source>\n");
      exit(1);
   }
}

/** @description  recherche horizontle
 *
 *  horizontalSearch()
 *  fait appel a la methode qui recherche dans la grille
 *  mais avec des parametres lui permettant de ce deplacer
 *  horizontalement dans la grille
 *
 *
 *  @param *word             => pointeur vers le mots achercher
 *  @param wordLength        => la longeur du mots rechercher
 *  @param nbrWord           => le nombre du mots recherche
 *  @retour rien
 */

void horizontalSearch(char *word, int wordLength, int nbrWord)
{
   int i, retourH;

   for (i = 0; i < 13; ++i)
   {
      retourH = horiVertiSearch(word, &(puzzle[i][0]), wordLength, 1, nbrWord, 0);
      if (retourH)
      {
         break;
      }
   }
}


/** @description  recherche verticale
 *
 *  verticalSearch()
 *  fait appel a la methode qui recherche dans la grille
 *  mais avec des parametres lui permettant de ce deplacer
 *  verticalement dans la grille
 *
 *
 *  @param *word             => pointeur vers le mots achercher
 *  @param wordLength        => la longeur du mots rechercher
 *  @param nbrWord           => le nombre du mots recherche
 *  @retour rien
 */

int verticalSearch(char *word, int wordLength, int nbrWord)
{
   int j, retourV;

   for (j = 0; j < 13; ++j)
   {
      retourV = horiVertiSearch(word, &(puzzle[0][j]), wordLength, 13, nbrWord, 1);
      if (retourV)
      {
         break;
      }
   }
   return j < 13;
}

/** @ description    chercher dans la grille
 *
 *
 *  horiVertiSearch()
 *  elle fait la recherche du mots dans la grille
 *  verticalement ou horizontalement depandament du parametre de deplacement
 *  est dans tout les directions
 *
 *  @param *wordPointer      => pointeur du mots a chercher
 *  @param *puzzelPointer    => pointeur de la grille
 *  @param wordLength        => la longeur du mots
 *  @param d                 => le nombre de sout pour le deplacement du pointeur
 *  @param nbrWord           => le nombre du mots
 *  @param caller            => id de la methode qui fait l'appelle (0 = horizontal , 1 = vertical)
 *  @retour 0 ou 1
 */

int horiVertiSearch(char *wordPointer, char *puzzelPointer, int wordLength, int d, int nbrWord, int caller)
{
   int i, j, k, targetFingerprint, pMaxIdx, tMaxIdx;

   pMaxIdx = wordLength * d;
   tMaxIdx = (13 - wordLength) * d;

   targetFingerprint = sumOfAsciiCodes(puzzelPointer, pMaxIdx, d);


   for (i = 0; i <= tMaxIdx; i += d)
   {
      if (patternFingerprint == targetFingerprint)
      {
         for (j = 0, k = i; j < wordLength; ++j, k += d) // forward match
            if (wordPointer[j] != puzzelPointer[k])
            {
               break;
            }
         if (j == wordLength)
         {
            fillWordPosiDerTab(nbrWord, i, wordLength, caller, puzzelPointer);

            return 1;
         } else
         {
            for (j = wordLength - 1, k = i; j >= 0; --j, k += d) // backward match

               if (wordPointer[j] != puzzelPointer[k])
               {
                  break;
               }
            if (j == -1)
            {
               fillWordPosiDerTab(nbrWord, i, wordLength, caller, puzzelPointer);
               return 1;
            }
         }
      } else
      {
         targetFingerprint += puzzelPointer[i + pMaxIdx] - puzzelPointer[i];
      }
   }
   return 0;
}


/** @description   entrer les informations dans la structure
 *
 *  filWordPosiDerTab()
 *  affect tout les parametres pour retrouver le mots dans la grille
 *  dans les variables declare dans la structure
 *  poui entregister cette structure dans un tableux
 *
 *
 *  @param  nbrWord        => le numero du mots
 *  @param  pos            => position du mots
 *  @param  wordLength     => la longeur du mots
 *  @param  caller         => id de la methode qui a trouver le mots
 *  @param  *puzzelPointer => pointeur sur le debut du mots
 *  @retour rien
 */

void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer)
{
   fistr.pos = pos;
   fistr.wordLength = wordLength;
   fistr.caller = caller;
   fistr.puzzelPointer = puzzelPointer;

   structArray[nbrWord] = fistr;
}


/** @description somme des code ASCII
 *
 *  somOfAsciiCodes()
 *  calcul et retourne  la somme des code ASCII des carataire de la chaine
 *  passer en parametre.
 *
 *  @param  chaine de caractaire
 *  @param  la longeur de la chaine
 *  @param  la valeur de deplacement dans la grille
 *  @retour la somme des code ascii de la chaine
 */

int sumOfAsciiCodes(char str[], int length, int d)
{
   int j, sum;
   sum = 0;
   for (j = 0; j < length; j += d)
   {
      sum += str[j];
   }
   return sum;
}


/** @description effacer les mots dans la grille
 *
 * horiVertiFillWord()
 * parcour la grille horisentallement et verticalement
 * pour effacer les carataires constituent les mots
 * en les remplacent par '*'
 *
 *  @param   le nombre des mots dans la graille
 *  @retour  rien
 */

void horiVertiFillWord(int nbrWord)
{
   int d, i, k, j;
   char *wordPionter;

   for (k = 0; k < nbrWord; ++k)
   {
      fistr = structArray[k];
      j = fistr.pos;
      wordPionter = fistr.puzzelPointer;

      d = fistr.caller == 0 ? 1 : 13;

      for (i = 0; i < fistr.wordLength; ++i)
      {
         wordPionter[j] = '*';
         j += d;
      }
   }
}


/** @description remplire le tableau
 *
 *  LoadPuzzle()
 *  lit une ligne du fichier fait appel  a  removeNewLineChar()
 *  pour supprimer le caractaire du retour a la ligne
 *  puis copier la chaine de caractaire dans la grille
 *
 *  @param    un pointeur ver le fichier a lire
 *  @retour   rien
 */

void LoadPuzzle(FILE *file)
{
   int i;

   for (i = 0; i < 13; ++i)
   {
      fgets(word, 13 + 1, file);
      removeNewLineChar(word);
      strcpy(&(puzzle[i][0]), word);
   }
}


/** @description Supremet de carataire '\n'
 *
 *  removeNewLineChar()
 *  Supprime Le carataire d'une nouvelle ligne ('\n'),
 *  de la lignes passer en parametre.
 *
 *  @param *str pointeur sur la chaine a traitre
 *  @retour la lengeur de la chaine traitee
 */

int removeNewLineChar(char *str)
{
   int length = (int) strlen(str) - 1;
   str[length] = '\0';
   return length;
}


/** @description    Afficher le mot cache
 *
 *  ShowResult()
 *  lit le tableux a deux dimention de gouche a droite et de haut en bas
 *  tout caractaire rencontre diffirant de '*' doit etre afficher a la
 *  console
 *
 *  @param non
 *  @retour rien
 */

void ShowResult()
{
   int i, j;
   for (i = 0; i < 13; ++i)
   {
      for (j = 0; j < 13; ++j)
      {
         puzzle[i][j] != '*' ? printf("%c", puzzle[i][j]) : 0;
      }
   }
   printf("\n");
}