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

/*des variables globales*/

int nb_arcs = 0, nb_etats = 0, nb_etiquettes = 0, nb_etat_init = 0, nb_etat_fin = 0;

/*des structures globales*/

arcs arc[100];
nom etats[100];
nom etiquettes[100];
nom etat_init[100];
nom etat_fin[100];

/* les fonctions*/

void GetInput(FILE *fichier);
void getAutomate();
int isInitial(char name[]);
int isFinal(char name[]);
void afficher_arcs();
void afficher_plus();
void generer_dot();

/* the main function*/

int main(int argc, char *argv[])
{
      FILE *fichier;
      fichier = fopen("input.txt", "r");

      if (fichier == NULL)
      {
            printf("ERROR!\n");
            exit(1);
      }

      GetInput(fichier);
      afficher_arcs();

      afficher_plus();

      fclose(fichier);

      FILE *file = NULL;
      file = fopen("input.dot", "w");

      generer_dot(file);

      fclose(file);

      return 0;
}

/*
cette fct lit les donnes du fichier dans ses parametres
les deux premiers linges sont pour les etas initiaux et finaux
les autres sont pour les iterations
*/
void GetInput(FILE *fichier)
{
      int i, j;
      char c;

      char ligne[100];
      // char *token;
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
            fscanf(fichier, "%s %s %s", arc[i].depart, arc[i].dest, arc[i].etiquette);
      }
      nb_arcs = i - 1;

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
                        strcpy(etat_init[j].nom, temp);
                        j++;
                        nb_etat_init++;
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
                        strcpy(etat_fin[j].nom, temp);
                        j++;
                        nb_etat_fin++;
                  }
            }
      }
}

void getAutomate()
{

      int etat_i = 0, etiq_i = 0, b, j, i;

      strcpy(etats[0].nom, arc[0].depart); // on stocke le premier etats de depart puis on commence les comparaisons

      nb_etats = 1;
      etat_i = 1;
      // on veut pas stocker le meme etat plusieurs fois
      for (i = 0; i <= nb_arcs; i++)
      {

            b = 0;
            for (j = 0; j < nb_etats; j++)
            {
                  if (strcmp(arc[i].depart, etats[j].nom) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {

                  strcpy(etats[etat_i].nom, arc[i].depart);
                  etat_i++;
                  nb_etats++;
            }

            b = 0;
            for (j = 0; j < nb_etats; j++)
            {
                  if (strcmp(arc[i].dest, etats[j].nom) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {

                  strcpy(etats[etat_i].nom, arc[i].dest);
                  etat_i++;
                  nb_etats++;
            }
      }

      // stocker les etiquettes

      strcpy(etiquettes[0].nom, arc[0].etiquette);
      nb_etiquettes = 1;
      etiq_i = 1;

      for (i = 0; i <= nb_arcs; i++)
      {
            b = 0;
            for (j = 0; j < nb_etiquettes; j++)
            {
                  if (strcmp(arc[i].etiquette, etiquettes[j].nom) == 0)
                  {
                        b = 1;
                  }
            }
            if (b == 0)
            {
                  strcpy(etiquettes[etiq_i].nom, arc[i].etiquette);
                  etiq_i++;
                  nb_etiquettes++;
            }
      }
}

// si on veut savoir si un etat est initial , elle sera utiliser plus tard
int isInitial(char name[])
{
      int i;
      for (i = 0; i < nb_etat_init; i++)
      {
            if (strcmp(name, etat_init[i].nom) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

// si on veut savoir si un etat est final , elle sera utiliser plus tard
int isFinal(char name[])
{
      int i;

      for (i = 0; i < nb_etats; i++)
      {
            if (strcmp(name, etat_fin[i].nom) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

// afficher les iterations
void afficher_arcs()
{
      int i;
      for (i = 0; i <= nb_arcs; i++)
      {
            printf("(%s) ---(%s)---> (%s)\n", arc[i].depart, arc[i].dest, arc[i].etiquette);
      }
}

void afficher_plus()
{
      int j;

      getAutomate();

      printf("\n nombre des etats : %d", nb_etats);

      // on utilise les isInitial et isFinal pour afficher  les etats initiaux et finaux avec des ->
      for (j = 0; j < nb_etats; j++)
      {
            if (isInitial(etats[j].nom) == 1 && isFinal(etats[j].nom) == 1)
            {
                  printf("\n[<-%s->]", etats[j].nom);
            }
            else
            {
                  if (isInitial(etats[j].nom) == 1)
                  {
                        printf("\n[->%s]", etats[j].nom);
                  }
                  else
                  {
                        if (isFinal(etats[j].nom) == 1)
                        {
                              printf("\n[%s->]", etats[j].nom);
                        }
                        else
                        {
                              printf("\n[%s]", etats[j].nom);
                        }
                  }
            }
      }

      printf("\n nombre des etiquettes : %d", nb_etiquettes);

      // affiche les etiquettes
      for (j = 0; j < nb_etiquettes; j++)
      {
            printf("\n|%s|", etiquettes[j].nom);
      }
}

void generer_dot(FILE *file)
{

      int i;
      fprintf(file, "digraph automate {\n");
      fprintf(file, "fontname=\"Helvetica,Arial,sans-serif\"\n");
      fprintf(file, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
      fprintf(file, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
      fprintf(file, "rankdir=LR;\n");
      fprintf(file, "node [shape = doublecircle ;];\n");

      for (i = 0; i < nb_etat_fin; i++)
      {

            fprintf(file, "%s;\t", etat_fin[i].nom);
      }
      fprintf(file, "node [shape = square ;];\n");

      for (i = 0; i < nb_etat_init; i++)
      {

            fprintf(file, "%s;\t", etat_init[i].nom);
      }

      fprintf(file, "\nnode [shape = circle];\n");

      for (i = 0; i <= nb_arcs; i++)
      {

            fprintf(file, " %s -> %s [label = \"%s\";];\n", arc[i].depart, arc[i].dest, arc[i].etiquette);
      }

      fprintf(file, "}\n");
}
