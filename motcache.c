/** @Fichier        motcache.c
 *  @Description     Résolution de mots caches
 *
 *  Ce fichier contient la fonction main() du programme motcache.
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


/*
 * Une structure a quatre champs, emballe comme une seule entité
 * pour conserver les informations de repérage de pour chaque mots
 * trouver.
 */

typedef struct
{
   int pos;
   int wordLength;
   int caller;
   char *puzzelPointer;
} FIstr;

/*--------------------------définitions des prototypes des fonctions.--------------------------*/

void loadPuzzle(FILE *file);

int removeNewLineChar(char *str);

int sumOfAsciiCodes(char str[], int length, int d);

int verticalSearch(char *word, int wordLength, int nbrWord, FIstr *structArray);

void horizontalSearch(char *word, int wordLength, int nbrWord, FIstr *structArray);

int horiVertiSearch(char *p, char *t, int pLen, int d, int nbrWord, int caller, FIstr *structArray);

void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer, FIstr *structArray);

void horiVertiFillWord(int nbrWord, FIstr *structArray);

void ShowResult();

void validerUsage(int argc);

void searchWords(FILE *file, int wordLength);

/*-------------Variables globales - les valeurs sont connues dans le programme.-------------*/

char puzzle[13][13];



int main(int argc, char **argv)
{


   FILE *file;
   int wordLength;

   wordLength = 0;

   validerUsage(argc);
   file = fopen(argv[1], "r");
   if (file == NULL)
   {
      printf("Erreur fichier source\n %s\n", strerror(errno));
      exit(1);
   }

   loadPuzzle(file);
   searchWords(file, wordLength);

   if (fclose(file) == EOF)
   {
      printf("Erreur lors de la fermeture du fichier.\n");
      return 1;
   }

   ShowResult();
   return 0;
}

/*------------------------------Fonctions sont définie en bas------------------------------*/


/** @Description  validation des arguments
 *
 *  validerUsage()
 *  valide que le programme a bien reçu en paramètre une fichier.txt
 *
 *
 *  @param argc     => paramètre qu'a reçu le programme
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



/** @Description  remplir le tableau
 *
 *  loadPuzzle()
 *  lit une ligne du fichier fait appel  à  removeNewLineChar()
 *  pour supprimer le caractère du retour à la ligne
 *  puis copier la chaine de caractère dans la grille
 *
 *  @param    un pointeur ver le fichier a lire
 *  @retour   rien
 */


void loadPuzzle(FILE *file)
{
   int i;
   char line[13 + 1];
   for (i = 0; i < 13; ++i)
   {
      fgets(line, 13 + 1, file);
      removeNewLineChar(line);
      strcpy(&(puzzle[i][0]), line);
   }
}




/** @Description   appele les méthodes de recherche
 *
 *  searchWords()
 *  lit une chaque mot à rechercher puis fait appel aux méthodes
 *  de recherche soit verticalement ou horizontalement
 *
 *  @param *file             => pointeur de ficher a lire
 *  @param wordLength        => la longueur du mot rechercher
 *  @param nbrWord           => le nombre du mot recherche
 *  @retour nombre des mots
 */


void searchWords(FILE *file, int wordLength)
{
   char word[13 + 1];
   int nbrWord = 0;
   FIstr structArray[60];

   while (fgets(word, 13 + 1, file) != NULL && word[0] != '\n')
   {
      wordLength = removeNewLineChar(word);

      if (!(verticalSearch(word, wordLength, nbrWord, structArray)))
      {
         horizontalSearch(word, wordLength, nbrWord, structArray);

      }
      ++nbrWord;
   }

   horiVertiFillWord(nbrWord,structArray);

}


/** @Description   recherche verticale
 *
 *  verticalSearch()
 *  fait appel à la méthode qui recherche dans la grille
 *  mais avec des paramètres lui permettant de ce déplacé
 *  verticalement dans la grille
 *
 *
 *  @param *word             => pointeur vers le mot à chercher
 *  @param wordLength        => la longueur du mot rechercher
 *  @param nbrWord           => le nombre du mot recherche
 *  @retour rien
 */


int verticalSearch(char *word, int wordLength, int nbrWord, FIstr *structArray)
{
   int j, retourV;

   for (j = 0; j < 13; ++j)
   {
      retourV = horiVertiSearch(word, &(puzzle[0][j]), wordLength, 13, nbrWord, 1, structArray);
      if (retourV)
      {
         break;
      }
   }
   return j < 13;
}



/** @Description   recherche horizontale
 *
 *  horizontalSearch()
 *  fait appel à la méthode qui recherche dans la grille
 *  mais avec des paramètres lui permettant de ce déplacé
 *  horizontalement dans la grille
 *
 *
 *  @param *word             => pointeur vers le mot à chercher
 *  @param wordLength        => la longueur du mot rechercher
 *  @param nbrWord           => le nombre du mot recherche
 *  @retour rien
 */


void horizontalSearch(char *word, int wordLength, int nbrWord, FIstr *structArray)
{
   int i, retourH;

   for (i = 0; i < 13; ++i)
   {
      retourH = horiVertiSearch(word, &(puzzle[i][0]), wordLength, 1, nbrWord, 0, structArray);
      if (retourH)
      {
         break;
      }
   }
}



/** @Description    chercher dans la grille
 *
 *
 *  horiVertiSearch()
 *  elle fait la recherche des mots dans la grille
 *  verticalement ou horizontalement dépendamment du paramètre de déplacement
 *  est dans tous les directions
 *
 *  @param *wordPointer      => pointeur du mot à chercher
 *  @param *puzzelPointer    => pointeur de la grille
 *  @param wordLength        => la longueur du mot
 *  @param d                 => le nombre de saut  pour le déplacement du pointeur
 *  @param nbrWord           => le nombre du mot
 *  @param caller            => id de la méthodes qui fait l'appelle (0 = horizontal , 1 = vertical)
 *  @retour 0 ou 1
 */


int horiVertiSearch(char *wordPointer, char *puzzelPointer, int wordLength, int d, int nbrWord, int caller,
                    FIstr *structArray)
{
   int i, j, k, targetFingerprint, pMaxIdx, tMaxIdx;
   int patternFingerprint;
   pMaxIdx = wordLength * d;
   tMaxIdx = (13 - wordLength) * d;

   patternFingerprint = sumOfAsciiCodes(wordPointer, wordLength, 1);

   targetFingerprint = sumOfAsciiCodes(puzzelPointer, pMaxIdx, d);

   for (i = 0; i <= tMaxIdx; i += d)
   {
      if (patternFingerprint == targetFingerprint)
      {
         for (j = 0, k = i; j < wordLength; ++j, k += d)
            if (wordPointer[j] != puzzelPointer[k])
            {
               break;
            }
         if (j == wordLength)
         {
            fillWordPosiDerTab(nbrWord, i, wordLength, caller, puzzelPointer, structArray);

            return 1;
         } else
         {
            for (j = wordLength - 1, k = i; j >= 0; --j, k += d)

               if (wordPointer[j] != puzzelPointer[k])
               {
                  break;
               }
            if (j == -1)
            {
               fillWordPosiDerTab(nbrWord, i, wordLength, caller, puzzelPointer, structArray);
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


/** @Description    entrer les informations dans la structure
 *
 *  filWordPosiDerTab()
 *  affect tous les paramètres pour retrouver les mots dans la grille
 *  dans les variables déclarés dans la structure
 *  ensuite  enregistrer cette structure dans un tableau
 *
 *
 *  @param  nbrWord        => le numéro du mot
 *  @param  pos            => position du mot
 *  @param  wordLength     => la longueur du mot
 *  @param  caller         => id de la méthode qui a trouvé le mot
 *  @param  *puzzelPointer => pointeur sur le début du mot
 *  @retour rien
 */


void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer, FIstr *structArray)
{
   FIstr fistr;

   fistr.pos = pos;
   fistr.wordLength = wordLength;
   fistr.caller = caller;
   fistr.puzzelPointer = puzzelPointer;

   structArray[nbrWord] = fistr;
}


/** @Description  somme des code ASCII
 *
 *  somOfAsciiCodes()
 *  calcul et retourne  la somme des code ASCII des caractères de la chaine
 *  passer en paramètre.
 *
 *  @param  chaine des caractères
 *  @param  la longueur de la chaine
 *  @param  la valeur de déplacement dans la grille
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


/** @Description  effacer les mots dans la grille
 *
 * horiVertiFillWord()
 * parcoure la grille horizontalement et verticalement
 * pour effacer les caractères constituent les mots
 * en les remplacent par '*'
 *
 *  @param   le nombre des mots dans la graille
 *  @retour  rien
 */


void horiVertiFillWord(int nbrWord, FIstr *structArray)
{
   int d, i, k, j;
   char *wordPionter;
   FIstr fistr;

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


/** @Description  Suppression de caractère '\n'
 *
 *  removeNewLineChar()
 *  Supprime Le caractère d'une nouvelle ligne ('\n'),
 *  de la ligne passer en paramètre.
 *
 *  @param *str pointeur sur la chaine a traitre
 *  @retour la langueur de la chaine traitée
 */


int removeNewLineChar(char *str)
{


   int length = (int) strlen(str) - 1;
   str[length] = '\0';
   return length;
}


/** @Description     Afficher le mot cache a la console
 *
 *  ShowResult()
 *  lit la grille de gauche à droite et de haut en bas
 *  tout caractère rencontre différant de '*' doit être afficher a la
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


