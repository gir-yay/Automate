#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

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

typedef struct
{
      char first[MAX];
      char second[MAX];
} Pair;

typedef struct
{
      iterations first;
      iterations second;
} arcPair;

/*-------------------------les fonctions-------------------------*/
/*Part 1*/

void GetInput(char text[], Automaton *A1);
void getAutomaton(Automaton *A1);
void init(Automaton *A1); /*initialisation*/
int menu();

void show_iterations(Automaton *A1);           /* print iterations as (from)--(symbol)-->(destination) */
void generate_dot(Automaton *A1, char name[]); /* genere le fichier .dot */

/*part 2*/

int isInitial(char name[], Automaton *A1);       /* if return 1 then it is an initial state*/
int isFinal(char name[], Automaton *A1);         /* if return 1 then it is a final state */
int test(char *word, Automaton *A1);             /* testing if a word is accepted by the automaton*/
void test_from_file(Automaton *A1, char text[]); /* testing a list of words*/

/*Part 3*/

void Automaton_Union(Automaton *A, Automaton *AA, Automaton *Union);
void Automaton_star(Automaton *A, Automaton *star);
void Automaton_Product(Automaton *A, Automaton *AA, Automaton *Product);

/*Part 4*/
int isreached(char *etat, Automaton *A);
Automaton reached(Automaton *A);
Automaton delete_epsilon(Automaton *A);
char *get_new_state(Automaton *nfa, char *state, char *symbol);
Automaton nfa_to_dfa(Automaton *nfa);

/* ---------------------- the main function ---------------*/

int main(int argc, char *argv[])
{

      Automaton *A = malloc(sizeof(Automaton));
      init(A);
      char text1[] = "automate1.txt";
      GetInput(text1, A);
      getAutomaton(A);

      Automaton *AA = malloc(sizeof(Automaton));
      init(AA);
      char text2[] = "automate2.txt";
      GetInput(text2, AA);
      getAutomaton(AA);

      Automaton *Union = malloc(sizeof(Automaton));
      init(Union);

      Automaton *star = malloc(sizeof(Automaton));
      init(star);

      Automaton *star2 = malloc(sizeof(Automaton));
      init(star2);

      Automaton *Product = malloc(sizeof(Automaton));
      init(Product);

      char list[] = "liste.txt";
      char list2[] = "liste2.txt";

      int choice;
      do
      {

            choice = menu();
            fflush(stdin);

            switch (choice)
            {
            case 1:

                  char name[] = "output1.dot";

                  printf("\n Automaton 1:\n");
                  generate_dot(A, name);
                  show_iterations(A);

                  printf("\n Automaton 2:\n");

                  char name2[] = "output2.dot";

                  show_iterations(AA);
                  generate_dot(AA, name2);

                  break;

            case 2:
                  int c = 0;
                  int result = 0;
                  char mot[20];

                  printf("Test using automaton 1 or 2 ?\t");
                  scanf("%d", &c);
                  fflush(stdin);

                  puts("Entrer the word to test!");
                  scanf("%s", mot);

                  printf("\nTesting the word %s ...\n", mot);
                  if (c == 1)
                  {
                        result = test(mot, A);
                        if (result == 1)
                        {
                              puts("accepted!");
                        }
                        else
                        {
                              puts("not accepted");
                        }

                        printf("\nTesting from the file %s, printing only the accepted words!\n\n", list);
                        test_from_file(A, list);
                  }

                  if (c == 2)
                  {
                        result = test(mot, AA);
                        if (result == 1)
                        {
                              puts("accepted!");
                        }
                        else
                        {
                              puts("not accepted");
                        }

                        printf("\nTesting from the file %s, printing only the accepted words!\n\n", list2);
                        test_from_file(AA, list2);
                  }

                  break;
            case 3:
                  /*-----------------------------Automate Union----------------*/

                  Automaton_Union(A, AA, Union);
                  getAutomaton(Union);
                  generate_dot(Union, "union.dot");

                  break;
            case 4:
                  /*-------------------------ETOILE d'un automate---------------*/

                  Automaton_star(A, star);
                  getAutomaton(star);
                  generate_dot(star, "star1.dot");

                  Automaton_star(AA, star2);
                  getAutomaton(star2);
                  generate_dot(star2, "star2.dot");

                  break;
            case 5:
                  /*----------------------Produit de deux automates------------------*/

                  Automaton_Product(A, AA, Product);
                  Automaton P = reached(Product);
                  getAutomaton(&P);
                  generate_dot(&P, "product.dot");

                  break;

            case 6:
                  Automaton u_sanseps = delete_epsilon(Union);
                  getAutomaton(&u_sanseps);
                  show_iterations(&u_sanseps);
                  generate_dot(&u_sanseps, "without_eps_union.dot");

                  break;
            case 7:

                  Automaton e_sanseps1 = delete_epsilon(star);
                  getAutomaton(&e_sanseps1);

                  generate_dot(&e_sanseps1, "without_eps_star1.dot");

                  Automaton e_sanseps2 = delete_epsilon(star2);
                  getAutomaton(&e_sanseps2);

                  generate_dot(&e_sanseps2, "without_eps_star2.dot");

                  break;
            case 8:
                  Automaton dfa = nfa_to_dfa(&u_sanseps);
                  getAutomaton(&dfa);
                  show_iterations(&dfa);
                  generate_dot(&dfa, "dfa.dot");
                  break;

            case 9:

                  /*Free Memory*/

                  free(A);
                  free(AA);
                  free(Union);
                  free(star);
                  free(star2);
                  free(Product);

                  exit(1);
            }

      } while (choice != 9);

      return 0;
}

/*----------------------------Part 1--------------------------*/
void init(Automaton *A1)
{
      A1->nb_final_state = 0;
      A1->nb_init_state = 0;
      A1->nb_iterations = 0;
      A1->nb_nodes = 0;
      A1->nb_symbols = 0;
}

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

void getAutomaton(Automaton *A1)
{

      int etat_i = 0, etiq_i = 0, b, j, i;

      strcpy(A1->nodes[0].name, A1->iterations[0].from);
      A1->nb_nodes = 1;
      etat_i = 1;

      for (i = 0; i <= A1->nb_iterations; i++)
      {

            b = 0;
            for (j = 0; j < A1->nb_nodes; j++)
            {
                  if (strcmp(A1->iterations[i].from, A1->nodes[j].name) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {

                  strcpy(A1->nodes[etat_i].name, A1->iterations[i].from);
                  etat_i++;
                  A1->nb_nodes++;
            }

            b = 0;
            for (j = 0; j < A1->nb_nodes; j++)
            {
                  if (strcmp(A1->iterations[i].dest, A1->nodes[j].name) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {

                  strcpy(A1->nodes[etat_i].name, A1->iterations[i].dest);
                  etat_i++;
                  A1->nb_nodes++;
            }
      }

      strcpy(A1->symbols[0].name, A1->iterations[0].symbol);
      A1->nb_symbols = 1;
      etiq_i = 1;

      for (i = 0; i <= A1->nb_iterations; i++)
      {
            b = 0;
            for (j = 0; j < A1->nb_symbols; j++)
            {
                  if (strcmp(A1->iterations[i].symbol, A1->symbols[j].name) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {
                  strcpy(A1->symbols[etiq_i].name, A1->iterations[i].symbol);
                  etiq_i++;
                  A1->nb_symbols++;
            }
      }
}

int menu()
{
      int choice;
      printf("\n--------------------------------Menu--------------------------\n");

      printf("1-show both automata and generate their .dot file\n");
      printf("2-test if a word is accepted and check a list of words\n");
      printf("3-Union   : generate .dot\n");
      printf("4-kleene star of both of the automata: genererate .dot\n");
      printf("5-Product : generate .dot\n");
      printf("6-Delete the epsilon transitions from the union\n");
      printf("7-Delete the epsilon transitions from the kleene stars\n");
      printf("8-NFA to DFA\n");
      printf("9-Quitter...\n");
      printf("your choice is :  ");

      scanf("%d", &choice);
      printf("\n");
      return choice;
}

void show_iterations(Automaton *A1)
{
      int i;
      for (i = 0; i <= A1->nb_iterations; i++)
      {
            printf("(%s) ---(%s)---> (%s)\n", A1->iterations[i].from, A1->iterations[i].symbol, A1->iterations[i].dest);
      }
}

void generate_dot(Automaton *A1, char name[])
{
      FILE *file = NULL;
      file = fopen(name, "w");
      if (file != NULL)
      {
            int i;
            fprintf(file, "digraph automate {\n");
            fprintf(file, "fontname=\"Helvetica,Arial,sans-serif\"\n");
            fprintf(file, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
            fprintf(file, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
            fprintf(file, "rankdir=LR;\n");

            fprintf(file, "\n\" \"[shape = none ;];\n");

            for (i = 0; i < A1->nb_init_state; i++)
            {

                  fprintf(file, "\" \"->%s;\n", A1->init_state[i].name);
            }

            for (i = 0; i < A1->nb_final_state; i++)
            {
                  fprintf(file, "\n%s[shape =doublecircle;];", A1->final_state[i].name);
            }

            fprintf(file, "\nnode [shape = circle;];\n");

            for (i = 0; i <= A1->nb_iterations; i++)
            {

                  fprintf(file, " %s -> %s [label = \"%s\";];\n", A1->iterations[i].from, A1->iterations[i].dest, A1->iterations[i].symbol);
            }

            fprintf(file, "}\n");
      }

      fclose(file);
}

/*-----------------------------------Part 2-------------------------------------*/

/* Testing if a word is accepted */
int test(char *word, Automaton *A1)
{
      int i, j, k;
      char current_node[100]; // current state
      int length = strlen(word);

      char temp[2];

      /* if the word is empty */
      if (length == 0)
      {
            return 0;
      }

      /* let the current state one of the initial states */
      for (i = 0; i < A1->nb_init_state; i++)
      {
            strcpy(current_node, A1->init_state[i].name);

            /*Test each symbol*/
            for (j = 0; j < length; j++)
            {
                  temp[0] = word[j];
                  temp[1] = '\0';

                  int found = 0;
                  for (k = 0; k <= A1->nb_iterations; k++)
                  {
                        if ((strcmp(A1->iterations[k].from, current_node) == 0) && (strcmp(A1->iterations[k].symbol, temp) == 0))
                        {
                              strcpy(current_node, A1->iterations[k].dest);
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
void test_from_file(Automaton *A1, char text[])
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

int isInitial(char name[], Automaton *A1)
{
      int i;
      for (i = 0; i < A1->nb_init_state; i++)
      {
            if (strcmp(name, A1->init_state[i].name) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

int isFinal(char name[], Automaton *A1)
{
      int i;

      for (i = 0; i < A1->nb_final_state; i++)
      {
            if (strcmp(name, A1->final_state[i].name) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

/*-------------------------Part 3-----------------------*/

void Automaton_Union(Automaton *A, Automaton *AA, Automaton *Union)
{
      int i, k;
      k = 0;
      Union->nb_init_state = 1;
      Union->nb_final_state = 1;
      strcpy(Union->init_state[0].name, "start");
      strcpy(Union->final_state[0].name, "end");

      for (i = 0; i <= A->nb_iterations; i++)
      {
            strcpy(Union->iterations[i].from, A->iterations[i].from);
            strcpy(Union->iterations[i].dest, A->iterations[i].dest);
            strcpy(Union->iterations[i].symbol, A->iterations[i].symbol);
      }
      k = i - 1;

      for (i = 0; i <= AA->nb_iterations; i++)
      {
            k++;
            itoa((atoi(AA->iterations[i].from) + A->nb_nodes), Union->iterations[k].from, 10);
            itoa((atoi(AA->iterations[i].dest) + A->nb_nodes), Union->iterations[k].dest, 10);
            strcpy(Union->iterations[k].symbol, AA->iterations[i].symbol);
      }
      for (i = 0; i < A->nb_init_state; i++)
      {
            k++;
            strcpy(Union->iterations[k].from, "start");
            strcpy(Union->iterations[k].dest, A->init_state[i].name);
            strcpy(Union->iterations[k].symbol, "epsilon");
      }
      for (i = 0; i < A->nb_final_state; i++)
      {
            k++;
            strcpy(Union->iterations[k].from, A->final_state[i].name);
            strcpy(Union->iterations[k].dest, "end");
            strcpy(Union->iterations[k].symbol, "epsilon");
      }

      for (i = 0; i < AA->nb_init_state; i++)
      {
            k++;
            strcpy(Union->iterations[k].from, "start");
            itoa((atoi(AA->init_state[i].name) + A->nb_nodes), Union->iterations[k].dest, 10);
            strcpy(Union->iterations[k].symbol, "epsilon");
      }
      for (i = 0; i < AA->nb_final_state; i++)
      {
            k++;
            itoa((atoi(AA->final_state[i].name) + A->nb_nodes), Union->iterations[k].from, 10);
            strcpy(Union->iterations[k].dest, "end");
            strcpy(Union->iterations[k].symbol, "epsilon");
      }
      Union->nb_iterations = k;
}

void Automaton_star(Automaton *A, Automaton *star)
{
      int i, j, k;
      star->nb_init_state = 1;
      star->nb_final_state = 1;
      strcpy(star->init_state[0].name, "start");
      strcpy(star->final_state[0].name, "end");

      for (i = 0; i <= A->nb_iterations; i++)
      {
            strcpy(star->iterations[i].from, A->iterations[i].from);
            strcpy(star->iterations[i].dest, A->iterations[i].dest);
            strcpy(star->iterations[i].symbol, A->iterations[i].symbol);
      }
      k = i - 1;

      for (i = 0; i < A->nb_init_state; i++)
      {
            k++;
            strcpy(star->iterations[k].from, "start");
            strcpy(star->iterations[k].dest, A->init_state[i].name);
            strcpy(star->iterations[k].symbol, "epsilon");
      }
      for (i = 0; i < A->nb_final_state; i++)
      {
            k++;
            strcpy(star->iterations[k].from, A->final_state[i].name);
            strcpy(star->iterations[k].dest, "end");
            strcpy(star->iterations[k].symbol, "epsilon");
      }
      k++;
      strcpy(star->iterations[k].from, "start");
      strcpy(star->iterations[k].dest, "end");
      strcpy(star->iterations[k].symbol, "epsilon");

      for (i = 0; i < A->nb_final_state; i++)
      {
            for (j = 0; j < A->nb_init_state; j++)
            {
                  k++;
                  strcpy(star->iterations[k].from, A->final_state[i].name);
                  strcpy(star->iterations[k].dest, A->init_state[j].name);
                  strcpy(star->iterations[k].symbol, "epsilon");
            }
      }

      star->nb_iterations = k;
}

void Automaton_Product(Automaton *A, Automaton *AA, Automaton *Product)
{
      Pair init_states[A->nb_init_state * AA->nb_final_state];
      Pair final_states[A->nb_init_state * AA->nb_final_state];
      Pair states[A->nb_nodes * AA->nb_nodes];

      arcPair ap[MAX];
      int nb_ap = 0;
      int count = 0;

      int i, j, k, l;

      int nb_init_states = 0;
      int nb_final_states = 0;
      int nb_states = A->nb_nodes * AA->nb_nodes;

      k = 0;

      for (i = 0; i < A->nb_nodes; i++)
      {
            for (j = 0; j < AA->nb_nodes; j++)
            {
                  strcpy(states[k].first, A->nodes[i].name);
                  strcpy(states[k].second, AA->nodes[j].name);
                  k++;
            }
      }
      k = 0;
      for (i = 0; i < A->nb_init_state; i++)
      {
            for (j = 0; j < AA->nb_init_state; j++)
            {
                  strcpy(init_states[k].first, A->init_state[i].name);
                  strcpy(init_states[k].second, AA->init_state[j].name);
                  strcpy(Product->init_state[k].name, strcat(init_states[k].first, init_states[k].second));
                  k++;
            }
      }

      nb_init_states = k;
      Product->nb_init_state = nb_init_states;

      k = 0;
      for (i = 0; i < A->nb_final_state; i++)
      {
            for (j = 0; j < AA->nb_final_state; j++)
            {
                  strcpy(final_states[k].first, A->final_state[i].name);
                  strcpy(final_states[k].second, AA->final_state[j].name);
                  strcpy(Product->final_state[k].name, strcat(final_states[k].first, final_states[k].second));

                  k++;
            }
      }

      nb_final_states = k - 1;
      Product->nb_final_state = nb_final_states;

      for (k = 0; k < nb_states; k++)
      {
            for (i = 0; i <= A->nb_iterations; i++)
            {
                  for (j = 0; j <= AA->nb_iterations; j++)
                  {
                        if (strcmp(A->iterations[i].from, states[k].first) == 0 && strcmp(AA->iterations[j].from, states[k].second) == 0)
                        {
                              for (l = 0; l < nb_states; l++)
                              {
                                    if (strcmp(A->iterations[i].dest, states[l].first) == 0 && strcmp(AA->iterations[j].dest, states[l].second) == 0 && strcmp(A->iterations[i].symbol, AA->iterations[j].symbol) == 0)
                                    {
                                          strcpy(ap[k].first.from, states[k].first);
                                          strcpy(ap[k].first.dest, states[l].first);
                                          strcpy(ap[k].first.symbol, A->iterations[i].symbol);

                                          strcpy(ap[k].second.from, states[k].second);
                                          strcpy(ap[k].second.dest, states[l].second);
                                          strcpy(ap[k].second.symbol, AA->iterations[j].symbol);

                                          strcpy(Product->iterations[count].from, strcat(ap[k].first.from, ap[k].second.from));
                                          strcpy(Product->iterations[count].dest, strcat(ap[k].first.dest, ap[k].second.dest));
                                          strcpy(Product->iterations[count].symbol, ap[k].first.symbol);
                                          count++;
                                    }
                              }
                        }
                  }
            }
      }

      Product->nb_iterations = count - 1;
}

/*Part 4*/
int isreached(char *etat, Automaton *A)
{
      int i, j;
      int count = 0;

      if ((isInitial(etat, A) == 1) || (isFinal(etat, A) == 1))
      {
            return 1;
      }
      else
      {

            for (i = 0; i <= A->nb_iterations; i++)
            {
                  if (strcmp(etat, A->iterations[i].dest) == 0)
                  {
                        count++;
                  }
            }
            if (count == 0)
            {
                  return 0;
            }
            else
            {
                  return 1;
            }
      }
}

Automaton reached(Automaton *A)
{
      int i, j;
      Automaton att;
      att.nb_init_state = A->nb_init_state;
      att.nb_final_state = A->nb_final_state;

      for (i = 0; i < A->nb_init_state; i++)
      {

            strcpy(att.init_state[i].name, A->init_state[i].name);
      }

      for (i = 0; i < A->nb_final_state; i++)
      {

            strcpy(att.final_state[i].name, A->final_state[i].name);
      }
      int k = 0;
      for (j = 0; j <= A->nb_iterations; j++)
      {
            if (isreached(A->iterations[j].from, A) == 1)
            {
                  strcpy(att.iterations[k].from, A->iterations[j].from);
                  strcpy(att.iterations[k].dest, A->iterations[j].dest);
                  strcpy(att.iterations[k].symbol, A->iterations[j].symbol);
                  k++;
            }
      }
      att.nb_iterations = k - 1;
      return att;
}

Automaton delete_epsilon(Automaton *A)
{

      Automaton sanseps;

      int i, m, n;
      int j = 0;
      int k = 0;
      int count = 0;

      int fin = 0;

      for (i = 0; i < A->nb_init_state; i++)
      {
            strcpy(sanseps.init_state[i].name, A->init_state[i].name);
      }
      sanseps.nb_init_state = A->nb_init_state;

      iterations eps_trans[20];
      int nb_eps = 0;
      for (i = 0; i <= A->nb_iterations; i++)
      {
            if (strcmp(A->iterations[i].symbol, "epsilon") == 0)
            {

                  if (isFinal(A->iterations[i].dest, A) == 0)
                  {

                        strcpy(eps_trans[j].symbol, A->iterations[i].symbol);
                        strcpy(eps_trans[j].from, A->iterations[i].from);
                        strcpy(eps_trans[j].dest, A->iterations[i].dest);
                        j++;
                  }
                  else
                  {
                        strcpy(sanseps.final_state[fin].name, A->iterations[i].from);
                        fin++;
                  }
            }
            else
            {

                  strcpy(sanseps.iterations[k].symbol, A->iterations[i].symbol);
                  strcpy(sanseps.iterations[k].from, A->iterations[i].from);
                  strcpy(sanseps.iterations[k].dest, A->iterations[i].dest);
                  k++;
            }
      }

      sanseps.nb_final_state = fin;

      nb_eps = j;

      for (n = 0; n < nb_eps; n++)
      {
            for (m = 0; m <= A->nb_iterations; m++)
            {

                  if (strcmp(eps_trans[n].dest, A->iterations[m].from) == 0)
                  {

                        strcpy(sanseps.iterations[k].symbol, A->iterations[m].symbol);
                        strcpy(sanseps.iterations[k].from, eps_trans[n].from);
                        strcpy(sanseps.iterations[k].dest, A->iterations[m].dest);

                        k++;
                  }
            }
      }

      sanseps.nb_iterations = k - 1;

      for (i = 0; i <= sanseps.nb_iterations; i++)
      {
            if (strcmp(sanseps.iterations[i].symbol, "epsilon") == 0)
            {
                  count++;
            }
      }
      if (count == 0)
      {
            return reached(&sanseps);
      }
      else
      {
            return delete_epsilon(&sanseps);
      }
}

char *get_new_state(Automaton *nfa, char *state, char *symbol)
{
      for (int i = 0; i <= nfa->nb_iterations; i++)
            if (strcmp(nfa->iterations[i].from, state) == 0 && strcmp(nfa->iterations[i].symbol, symbol) == 0)
                  return nfa->iterations[i].dest;
      return NULL;
}

// Function to convert NFA to DFA
Automaton nfa_to_dfa(Automaton *nfa)
{
      Automaton dfa;
      // Initialize DFA
      dfa.nb_nodes = 0;
      dfa.nb_symbols = nfa->nb_symbols;
      memcpy(dfa.symbols, nfa->symbols, sizeof(name) * nfa->nb_symbols);
      dfa.nb_init_state = 1;
      memcpy(dfa.init_state, nfa->init_state, sizeof(name) * nfa->nb_init_state);
      dfa.nb_final_state = 0;
      dfa.nb_iterations = 0;

      // For each state in NFA
      for (int i = 0; i < nfa->nb_nodes; i++)
      {
            // Add it to DFA
            strcpy(dfa.nodes[dfa.nb_nodes++].name, nfa->nodes[i].name);
            // If it's a final state in NFA, add it to final states in DFA
            if (isFinal(nfa->nodes[i].name, nfa))
                  strcpy(dfa.final_state[dfa.nb_final_state++].name, nfa->nodes[i].name);
            // For each symbol
            for (int j = 0; j < nfa->nb_symbols; j++)
            {
                  // Get the new state
                  char *new_state = get_new_state(nfa, nfa->nodes[i].name, nfa->symbols[j].name);
                  // If the new state is not NULL, add an iteration to DFA
                  if (new_state)
                  {
                        strcpy(dfa.iterations[dfa.nb_iterations].from, nfa->nodes[i].name);
                        strcpy(dfa.iterations[dfa.nb_iterations].symbol, nfa->symbols[j].name);
                        strcpy(dfa.iterations[dfa.nb_iterations++].dest, new_state);
                  }
            }
      }

      dfa.nb_iterations--;

      return dfa;
}