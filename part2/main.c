#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------structures-------------------------*/

typedef struct name
{
      char name[100];
} name;

/*iterations*/

typedef struct iterations
{
      char from[100];
      char dest[100];
      char symbol[100];
} iterations;

/* automaton */
typedef struct Automaton
{
      name nodes[100];
      name symbols[100];
      name init_state[100];
      name final_state[100];
      iterations iterations[100];
      int nb_iterations, nb_nodes, nb_symbols, nb_init_state, nb_final_state;

} Automaton;

/*------------------------- functions-------------------------*/

/*---Part 1---*/

void GetInput(char text[], Automaton *A1);    /* read an automaton from a .txt file*/
void show_iterations(Automaton A1);           /* print iterations as (from)--(symbol)-->(destination) */
void generate_dot(Automaton A1, char name[]); /* genere le fichier .dot */

/*---Part 2---*/

int isInitial(char name[], Automaton A1);       /* if return 1 then it is an initial state*/
int isFinal(char name[], Automaton A1);         /* if return 1 then it is a final state */
int test(char *word, Automaton A1);             /* testing if a word is accepted by the automaton*/
void test_from_file(Automaton A1, char text[]); /* testing a list of words*/

/* ---------------------- the main function ---------------*/

int main(int argc, char *argv[])
{

      Automaton A; /*A variable of type Automaton*/

      char text1[] = "automate1.txt"; /* name of the file of the automaton*/
      GetInput(text1, &A);            /* stocking info of the file in the struct Automate A */

      // FOR Part 1 AND NOT NECESSARY FOR PART 2

      show_iterations(A);
      char name[] = "output.dot";
      generate_dot(A, name);

      // Part 2

      /*------------ testing a word----------------*/

      int result;
      char mot[] = "bbbbbbc"; /* the word to test */

      printf("\nlet us test the word %s :\n", mot);

      result = test(mot, A);

      if (result == 1)
      {
            puts("Accepted");
      }
      else
      {
            puts("Not Accepted");
      }

      char list[] = "liste.txt"; /* Name of the file containing our list of words */

      printf("\nLet us test the words of the file %s, we will print onlt the accepted words!\n\n", list);

      test_from_file(A, list);

      return 0;
}

/*----------------------------------Functions Of Part 2---------------------------------------------*/

/* Testing if a word is accepted */
int test(char *word, Automaton A1)
{
      int i, j, k;
      char current_node[100];    // current state
      int length = strlen(word); // words' length

      char temp[2];

      /* if the word is empty */
      if (length == 0)
      {
            return 0;
      }

      /* let the current state one of the initial states */
      for (i = 0; i < A1.nb_init_state; i++)
      {
            strcpy(current_node, A1.init_state[i].name);

            /*Test each symbol*/
            for (j = 0; j < length; j++)
            {
                  temp[0] = word[j];
                  temp[1] = '\0';

                  int found = 0;
                  for (k = 0; k <= A1.nb_iterations; k++)
                  {
                        if ((strcmp(A1.iterations[k].from, current_node) == 0) && (strcmp(A1.iterations[k].symbol, temp) == 0))
                        {
                              strcpy(current_node, A1.iterations[k].dest);
                              found = 1;
                              break;
                        }
                  }
                  if (found == 0)
                  {
                        /* no such iteration is found */
                        return 0;
                  }
            }
      }

      /* if the current stare is a final state return 1*/
      return isFinal(current_node, A1);
}

/* testing words of a file*/
void test_from_file(Automaton A1, char text[])
{
      int i;
      int res;

      FILE *list;
      list = fopen(text, "r");

      if (list == NULL)
      {
            printf("ERROR!\n");
            exit(1);
      }

      char string[100];
      rewind(list);
      while (fgets(string, 50, list) != NULL)
      {
            string[strlen(string) - 1] = '\0';
            res = test(string, A1);
            if (res == 1)
            {
                  printf("%s\n", string);
            }
      }

      fclose(list);
}

int isInitial(char name[], Automaton A1)
{
      int i;
      for (i = 0; i < A1.nb_init_state; i++)
      {
            if (strcmp(name, A1.init_state[i].name) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

int isFinal(char name[], Automaton A1)
{
      int i;

      for (i = 0; i < A1.nb_final_state; i++)
      {
            if (strcmp(name, A1.final_state[i].name) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

/*--------------------------------------Functions Of Part 1------------------------------------------*/

void GetInput(char text[], Automaton *A1)
{
      FILE *fichier;
      fichier = fopen(text, "r");

      if (fichier == NULL)
      {
            printf("ERROR!\n");
            exit(1);
      }

      int i, j;
      char c;

      char line[100];
      int NombreLines = 1;
      char temp[2];

      while ((c = fgetc(fichier)) != EOF)
      {
            if (c == '\n')
            {
                  NombreLines++;
            }
      }

      rewind(fichier);

      for (i = 0; i < (NombreLines - 2); i++)
      {
            fscanf(fichier, "%s %s %s", A1->iterations[i].from, A1->iterations[i].dest, A1->iterations[i].symbol);
      }
      A1->nb_iterations = i - 1;

      j = 0;
      if (fscanf(fichier, "%s", line) == 1)
      {

            for (i = 0; i < strlen(line); i++)
            {
                  if (line[i] != ',')
                  {
                        temp[0] = line[i];
                        temp[1] = '\0';
                        strcpy(A1->init_state[j].name, temp);
                        j++;
                        A1->nb_init_state++;
                  }
            }
      }

      j = 0;
      if (fscanf(fichier, "%s", line) == 1)
      {

            for (i = 0; i < strlen(line); i++)
            {
                  if (line[i] != ',')
                  {
                        temp[0] = line[i];
                        temp[1] = '\0';
                        strcpy(A1->final_state[j].name, temp);
                        j++;
                        A1->nb_final_state++;
                  }
            }
      }
      fclose(fichier);
}

void show_iterations(Automaton A1)
{
      int i;
      for (i = 0; i <= A1.nb_iterations; i++)
      {
            printf("(%s) ---(%s)---> (%s)\n", A1.iterations[i].from, A1.iterations[i].symbol, A1.iterations[i].dest);
      }
}

void generate_dot(Automaton A1, char name[])
{
      FILE *file = NULL;
      file = fopen(name, "w");

      int i;
      fprintf(file, "digraph automate {\n");
      fprintf(file, "fontname=\"Helvetica,Arial,sans-serif\"\n");
      fprintf(file, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
      fprintf(file, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
      fprintf(file, "rankdir=LR;\n");
      fprintf(file, "node [shape = doublecircle ;]\n");

      for (i = 0; i < A1.nb_final_state; i++)
      {

            fprintf(file, ";%s\t", A1.final_state[i].name);
      }
      fprintf(file, "\nnode [shape = square ;];\n");

      for (i = 0; i < A1.nb_init_state; i++)
      {

            fprintf(file, "%s;\t", A1.init_state[i].name);
      }

      fprintf(file, "\nnode [shape = circle];\n");

      for (i = 0; i <= A1.nb_iterations; i++)
      {

            fprintf(file, " %s -> %s [label = \"%s\";];\n", A1.iterations[i].from, A1.iterations[i].dest, A1.iterations[i].symbol);
      }

      fprintf(file, "}\n");

      fclose(file);
}
