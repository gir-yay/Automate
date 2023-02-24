#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*les structures*/
// on peut avoir des chaine comme alphabet ou nom d'etat

typedef struct nom
{
      char nom[100];
} nom;

/*les arcs*/

typedef struct arcs
{
      char depart[100];
      char dest[100];
      char etiquette[100];
} arcs;

typedef struct Automate
{
      nom etats[100];
      nom etiquettes[100];
      nom etat_init[100];
      nom etat_fin[100];
      arcs arc[100];
      int nb_arcs, nb_etats, nb_etiquettes, nb_etat_init, nb_etat_fin;

} Automate;

/* les fonctions*/

void GetInput(char text[], Automate *A1);
void getAutomate(Automate *A1);
int isInitial(char name[], Automate A1);
int isFinal(char name[], Automate A1);
void afficher_arcs(Automate A1);
void afficher_plus(Automate A1);
void generer_dot(Automate A1, char name[]);
int tester_automate(char *mot, Automate A1);
void tester_from_file(Automate A1, char text[]);

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

/*
cette fct lit les donnes du fichier dans ses parametres
les deux premiers linges sont pour les etas initiaux et finaux
les autres sont pour les iterations
*/
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

      char ligne[100];
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
      // recuperation des Transitions
      for (i = 0; i < (NombreLines - 2); i++)
      {
            fscanf(fichier, "%s %s %s", A1->arc[i].depart, A1->arc[i].dest, A1->arc[i].etiquette);
      }
      A1->nb_arcs = i - 1;

      // stocker les etats initiaux
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

      // recuperation de la dernière ligne (etats finaux)
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

      int etat_i = 0, etiq_i = 0, b, j, i;

      strcpy(A1->etats[0].nom, A1->arc[0].depart); // on stocke le premier etats de depart puis on commence les comparaisons

      A1->nb_etats = 1;
      etat_i = 1;
      // on veut pas stocker le meme etat plusieurs fois
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

      // stocker les etiquettes

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

// si on veut savoir si un etat est initial , elle sera utiliser plus tard
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

// si on veut savoir si un etat est final , elle sera utiliser plus tard
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

// afficher les iterations
void afficher_arcs(Automate A1)
{
      int i;
      for (i = 0; i <= A1.nb_arcs; i++)
      {
            printf("(%s) ---(%s)---> (%s)\n", A1.arc[i].depart, A1.arc[i].dest, A1.arc[i].etiquette);
      }
}

void afficher_plus(Automate A1)
{
      int j;

      getAutomate(&A1);

      printf("\n nombre des etats : %d", A1.nb_etats);

      // on utilise les isInitial et isFinal pour afficher  les etats initiaux et finaux avec des ->
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

      // affiche les etiquettes
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
