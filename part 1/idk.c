#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nom // name
{
      char nom[100];
} nom;

/*transitions*/

typedef struct arcs
{
      char depart[100];    // from
      char dest[100];      // to or destination
      char etiquette[100]; // symbol or alphabet
} arcs;

typedef struct Automate
{
      nom etats[100];                                                  // state
      nom etiquettes[100];                                             // symbol or alphabet
      nom etat_init[100];                                              // initial states
      nom etat_fin[100];                                               // final states
      arcs arc[100];                                                   // iterations
      int nb_arcs, nb_etats, nb_etiquettes, nb_etat_init, nb_etat_fin; // nb for number

} Automate;

/* les fonctions*/

void GetInput(char text[], Automate *A1);
void getAutomate(Automate *A1);
void GetInput(char text[], Automate *A1); // read the automaton from the file
void getAutomate(Automate *A1);           // just to store all the distinct states and symbols of the automaton
int isInitial(char name[], Automate A1);
int isFinal(char name[], Automate A1);
void afficher_arcs(Automate A1);                 // display transitions
void afficher_plus(Automate A1);                 // display more basically the information that we got thanks to getAutomate()
void generer_dot(Automate A1, char name[]);      // generate a .dot file that describe the graph of the automaton
int tester_automate(char *mot, Automate A1);     // test if a word is of that automaton
void tester_from_file(Automate A1, char text[]); // use a file to test many words

/* the main function*/
int main(int argc, char *argv[])
{
      Automate A;

      char text1[] = "input.txt";
      GetInput(text1, &A);
      getAutomate(&A);

      afficher_arcs(A);
      afficher_plus(A);
      char name[] = "output.dot";
      generer_dot(A, name);

      return 0;
}

// this function is not really necessary !!!

void GetInput(char text[], Automate *A1)
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

      char ligne[100];     // line
      int NombreLines = 1; // number of lines of the file
      char temp[2];        // just to transform each letter of the line to a string

      while ((c = fgetc(fichier)) != EOF)
      {
            if (c == '\n')
            {
                  NombreLines++;
            }
      }

      rewind(fichier);
      // get all Transitions
      for (i = 0; i < (NombreLines - 2); i++)
      {
            fscanf(fichier, "%s %s %s", A1->arc[i].depart, A1->arc[i].dest, A1->arc[i].etiquette);
      }
      A1->nb_arcs = i - 1;

      // stocking the initial states
      j = 0;
      if (fscanf(fichier, "%s", ligne) == 1)
      {

            for (i = 0; i < strlen(ligne); i++)
            {
                  if (ligne[i] != ',')
                  {
                        temp[0] = ligne[i];
                        temp[1] = '\0';
                        strcpy(A1->etat_init[j].nom, temp);
                        j++;
                        A1->nb_etat_init++;
                  }
            }
      }

      // stocking the final states
      j = 0;
      if (fscanf(fichier, "%s", ligne) == 1)
      {

            for (i = 0; i < strlen(ligne); i++)
            {
                  if (ligne[i] != ',')
                  {
                        temp[0] = ligne[i];
                        temp[1] = '\0';
                        strcpy(A1->etat_fin[j].nom, temp);
                        j++;
                        A1->nb_etat_fin++;
                  }
            }
      }
      fclose(fichier);
}

void getAutomate(Automate *A1)
{

      int etat_i = 0, etiq_i = 0, b, j, i; // etat == state

      strcpy(A1->etats[0].nom, A1->arc[0].depart);

      A1->nb_etats = 1;
      etat_i = 1;

      for (i = 0; i <= A1->nb_arcs; i++)
      {

            b = 0;
            for (j = 0; j < A1->nb_etats; j++)
            {
                  if (strcmp(A1->arc[i].depart, A1->etats[j].nom) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {

                  strcpy(A1->etats[etat_i].nom, A1->arc[i].depart);
                  etat_i++;
                  A1->nb_etats++;
            }

            b = 0;
            for (j = 0; j < A1->nb_etats; j++)
            {
                  if (strcmp(A1->arc[i].dest, A1->etats[j].nom) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {

                  strcpy(A1->etats[etat_i].nom, A1->arc[i].dest);
                  etat_i++;
                  A1->nb_etats++;
            }
      }

      strcpy(A1->etiquettes[0].nom, A1->arc[0].etiquette);
      A1->nb_etiquettes = 1;
      etiq_i = 1;

      for (i = 0; i <= A1->nb_arcs; i++)
      {
            b = 0;
            for (j = 0; j < A1->nb_etiquettes; j++)
            {
                  if (strcmp(A1->arc[i].etiquette, A1->etiquettes[j].nom) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {
                  strcpy(A1->etiquettes[etiq_i].nom, A1->arc[i].etiquette);
                  etiq_i++;
                  A1->nb_etiquettes++;
            }
      }
}

int isInitial(char name[], Automate A1)
{
      int i;
      for (i = 0; i < A1.nb_etat_init; i++)
      {
            if (strcmp(name, A1.etat_init[i].nom) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

int isFinal(char name[], Automate A1)
{
      int i;

      for (i = 0; i < A1.nb_etat_fin; i++)
      {
            if (strcmp(name, A1.etat_fin[i].nom) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

// show iterations
void afficher_arcs(Automate A1)
{
      int i;
      for (i = 0; i <= A1.nb_arcs; i++)
      {
            printf("(%s) ---(%s)---> (%s)\n", A1.arc[i].depart, A1.arc[i].dest, A1.arc[i].etiquette);
      }
}

// show more
// this function is not necessary too!!!
void afficher_plus(Automate A1)
{
      int j;

      getAutomate(&A1);

      printf("\n nombre des etats : %d", A1.nb_etats);

      for (j = 0; j < A1.nb_etats; j++)
      {
            if (isInitial(A1.etats[j].nom, A1) == 1 && isFinal(A1.etats[j].nom, A1) == 1)
            {
                  printf("\n[<-%s->]", A1.etats[j].nom);
            }
            else
            {
                  if (isInitial(A1.etats[j].nom, A1) == 1)
                  {
                        printf("\n[->%s]", A1.etats[j].nom);
                  }
                  else
                  {
                        if (isFinal(A1.etats[j].nom, A1) == 1)
                        {
                              printf("\n[%s->]", A1.etats[j].nom);
                        }
                        else
                        {
                              printf("\n[%s]", A1.etats[j].nom);
                        }
                  }
            }
      }

      printf("\n nombre des etiquettes : %d", A1.nb_etiquettes);

      // show all the distinct symbols
      for (j = 0; j < A1.nb_etiquettes; j++)
      {
            printf("\n|%s|", A1.etiquettes[j].nom);
      }
}

void generer_dot(Automate A1, char name[])
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

      for (i = 0; i < A1.nb_etat_fin; i++)
      {

            fprintf(file, ";%s\t", A1.etat_fin[i].nom);
      }
      fprintf(file, "\nnode [shape = square ;];\n");

      for (i = 0; i < A1.nb_etat_init; i++)
      {

            fprintf(file, "%s;\t", A1.etat_init[i].nom);
      }

      fprintf(file, "\nnode [shape = circle];\n");

      for (i = 0; i <= A1.nb_arcs; i++)
      {

            fprintf(file, " %s -> %s [label = \"%s\";];\n", A1.arc[i].depart, A1.arc[i].dest, A1.arc[i].etiquette);
      }

      fprintf(file, "}\n");

      fclose(file);
}
