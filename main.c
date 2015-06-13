#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes definitions
void LoadPuzzle(FILE *file);

int removeNewLineChar(char *str);

int sumOfAsciiCodes(char s[], int length, int d);

int verticalSearch(char *word, int wordLength, int nbrWord);

void horizontalSearch(char *word, int wordLength, int nbrWord);

int horiVertiSearch(char *p, char *t, int pLen, int d, int nbrWord, int caller);

void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer);

void horiVertiFillWord(int nbrWord);

void ShowResult();

void validerUsage(int argc);

int searchWords(const FILE *file, int wordLength, int nbrWord);

/*
 * A structure with three fields, packed as one entity
 */
typedef struct
{
   int pos;
   int wordLength;
   int caller;
   char *puzzelPointer;
} FIstr;

//Global variables - values are known throughout the program.
char puzzle[13][13]; // puzzle array
char word[14];       // words array
FIstr fistr;         // structure of the position of the word funds
FIstr wordPosiDer[50]; // stctures array
int patternFingerprint;

int main(int argc, char **argv)
{
   FILE *file;
   int wordLength, nbrWord;
   nbrWord = 0;
   
   validerUsage(argc);
   file = fopen(argv[1], "r");
   if (file == NULL)
   {
      printf("Erreur fichier source %d\n", errno);
      exit(1);
   }

   LoadPuzzle(file);
   nbrWord = searchWords(file, wordLength, nbrWord);
   fclose(file);
   horiVertiFillWord(nbrWord);
   ShowResult();
   return 0;
}

int searchWords(const FILE *file, int wordLength, int nbrWord)
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

// Functions are defined  below

/*
 * A structure with three fields, packed as one entity
 */
void validerUsage(int argc)
{
   if (argc != 2)
   {
      printf("Utilisation du programme main.c :\nmain.c <fichier source>\n");
      exit(1);
   }
}

/*
 * A structure with three fields, packed as one entity
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


/*
 * A structure with three fields, packed as one entity
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

/*
 * A structure with three fields, packed as one entity
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


/*
 * A structure with three fields, packed as one entity
 */
void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer)
{
   fistr.pos = pos;
   fistr.wordLength = wordLength;
   fistr.caller = caller;
   fistr.puzzelPointer = puzzelPointer;

   wordPosiDer[nbrWord] = fistr;
}


/*
 * A structure with three fields, packed as one entity
 */

int sumOfAsciiCodes(char s[], int length, int d)
{
   int j, sum;
   sum = 0;
   for (j = 0; j < length; j += d)
   {
      sum += s[j];
   }
   return sum;
}


/*
 * A structure with three fields, packed as one entity
 */
void horiVertiFillWord(int nbrWord)
{
   int d, i, k, j;
   char *wordPionter;

   for (k = 0; k < nbrWord; ++k)
   {
      fistr = wordPosiDer[k];
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


/*
 * A structure with three fields, packed as one entity
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


/*
 * A structure with three fields, packed as one entity
 */
int removeNewLineChar(char *str)
{
   int length = (int) strlen(str) - 1;
   str[length] = '\0';
   return length;
}



/*
 * A structure with three fields, packed as one entity
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