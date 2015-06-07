#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void LoadPuzzle(char *argv[]);

int removeNL(char *str);

int horiVertiFingerprint(char s[], int length, int d);

int verticalSearch(char *word, int wordLength, int nbrWord);

int horizontalSearch(char *word, int wordLength, int nbrWord);

int horiVertiSearch(char *p, char *t, int pLen, int d, int nbrWord, int caller);

void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer);

void horiVertiFillWord(int nbrWord);

void ShowPuzzle();

void ShowResult();
void validerUsage(int argc);

#define MAX_COL_LINE 13
#define MAX_WORD 50


typedef struct
{
   int pos;
   int wordLength;
   int caller;
   char *puzzelPointer;
} FIstr;

char puzzle[MAX_COL_LINE][MAX_COL_LINE], word[MAX_COL_LINE + 1];
FIstr fistr;
FIstr wordPosiDer[MAX_WORD];
int  pFP;

int main(int argc, char **argv)
{
   FILE *file;
   int wordLength;
   int nbrWord = 0;

   validerUsage(argc);

   LoadPuzzle(argv);

      //ShowPuzzle();
      if ((file = fopen("words.txt", "r")) != NULL)
      {
         while (fgets(word, MAX_COL_LINE + 1, file) != NULL)
         {
            wordLength = removeNL(word);
            if (!(verticalSearch(word, wordLength, nbrWord)))
            {
               horizontalSearch(word, wordLength, nbrWord);

            }
            ++nbrWord;
         }
         fclose(file);
         horiVertiFillWord(nbrWord);
         // ShowPuzzle();
         ShowResult();
      }

   return 0;
}


void validerUsage(int argc)
{
   if (argc != 2)
   {
      printf("Utilisation du programme main.c :\nmain.c <fichier source>\n");
      exit(1);
   }
}

int horizontalSearch(char *word, int wordLength, int nbrWord)
{
   int i;
   pFP = horiVertiFingerprint(word, wordLength, 1);


   for (i = 0; i < MAX_COL_LINE; ++i)
   {
      int retourH = horiVertiSearch(word, &(puzzle[i][0]), wordLength, 1, nbrWord, 0);
      if (retourH)
      {
         break;
      }
   }
   return i < MAX_COL_LINE;
}

int verticalSearch(char *word, int wordLength, int nbrWord)
{
   int j;
   pFP = horiVertiFingerprint(word, wordLength, 1);

   for (j = 0; j < MAX_COL_LINE; ++j)
   {
      int retourV = horiVertiSearch(word, &(puzzle[0][j]), wordLength, MAX_COL_LINE, nbrWord, 1);
      if (retourV)
      {
         break;
      }
   }
   return j < MAX_COL_LINE;
}


int horiVertiSearch(char *wordPointer, char *puzzelPointer, int wordLength, int d, int nbrWord, int caller)
{
   int i, j, k, tFP;
   int pMaxIdx = wordLength * d;
   int tMaxIdx = (MAX_COL_LINE - wordLength) * d;

   tFP = horiVertiFingerprint(puzzelPointer, pMaxIdx, d);

   for (i = 0; i <= tMaxIdx; i += d)
   {
      if (pFP == tFP)
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
         tFP += puzzelPointer[i + pMaxIdx] - puzzelPointer[i];
      }
   }
   return 0;
}


void fillWordPosiDerTab(int nbrWord, int pos, int wordLength, int caller, char *puzzelPointer)
{
   fistr.pos = pos;
   fistr.wordLength = wordLength;
   fistr.caller = caller;
   fistr.puzzelPointer = puzzelPointer;

   wordPosiDer[nbrWord] = fistr;

}

int horiVertiFingerprint(char s[], int length, int d)
{
   int j, sum;
   sum = 0;
   for (j = 0; j < length; j += d)
      sum += s[j];
   return sum;
}

void horiVertiFillWord(int nbrWord)
{
   int d, i, k, j, wordLength, readingDirection;
   char *wordPonter;

   for (k = 0; k < nbrWord; ++k)
   {
      fistr = wordPosiDer[k];
      j = fistr.pos;
      wordLength = fistr.wordLength;
      readingDirection = fistr.caller;
      wordPonter = fistr.puzzelPointer;

      d = readingDirection == 0 ? 1 : MAX_COL_LINE;

      for (i = 0; i < wordLength; ++i)
      {
         wordPonter[j] = '*';
         j += d;
      }
   }
}

void LoadPuzzle(char *argv[])
{
   int i;
   FILE *file;
   file = fopen(argv[1], "r");

   if (file == NULL)
   {
      printf("Erreur fichier source %d\n", errno);
      exit(1);
   }

   for (i = 0; i < MAX_COL_LINE; ++i)
   {
      fgets(word, MAX_COL_LINE + 1, file);
      removeNL(word);
      strcpy(&(puzzle[i][0]), word);
   }
   fclose(file);

}

int removeNL(char *str)
{
   int length = (int) strlen(str) - 1;
   str[length] = '\0';
   return length;
}

void ShowPuzzle()
{
   int i, j;
   for (i = 0; i < MAX_COL_LINE; ++i)
   {
      for (j = 0; j < MAX_COL_LINE; ++j)
      {
         printf("%c", puzzle[i][j]);
      }
      printf("\n");
   }
}

void ShowResult()
{
   int i, j;
   for (i = 0; i < MAX_COL_LINE; ++i)
   {
      for (j = 0; j < MAX_COL_LINE; ++j)
      {
         puzzle[i][j] != '*' ? printf("%c", puzzle[i][j]):0;
      }
   }
   printf("\n");
}