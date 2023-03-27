#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

/*----------------------les structures-------------------------*/

/* on peut avoir des chaine comme  nom d'etat */

typedef struct
{
      char nom[MAX];
} nom;

/*les arcs*/

typedef struct
{
      char depart[MAX];
      char dest[MAX];
      char etiquette[MAX];
} arcs;

/* l'automate */
typedef struct
{
      arcs arc[MAX];
      nom etat_init[MAX];
      nom etat_fin[MAX];
      nom etats[MAX];
      nom etiquettes[MAX];
      int nb_arcs;
      int nb_etat_init;
      int nb_etat_fin;
      int nb_etats;
      int nb_etiquettes;

} Automate;

typedef struct
{
      char first[MAX];
      char second[MAX];
} Pair;

typedef struct
{
      arcs first;
      arcs second;
} arcPair;

typedef struct
{
      arcs arc[20];
      int nb_arcs;
      char depart[MAX];
      char dest[MAX];
      char etiquette[MAX];
      int initial;
      int final;
} Group;
/*-------------------------les fonctions-------------------------*/
/*Part 1*/

void GetInput(char text[], Automate *A1); /* lire l'automate à partir du fichier .txt */
void getAutomate(Automate *A1);
void init(Automate *A1); /*initialisation*/
int menu();

void afficher_arcs(Automate *A1);            /* print arcs comme (depart)--(etiquette)-->(destination) */
void generer_dot(Automate *A1, char name[]); /* genere le fichier .dot */

/*part 2*/
int isInitial(char name[], Automate *A1);         /* si return 1 c'est un etat initial*/
int isFinal(char name[], Automate *A1);           /* si return 1 c'est un etat finale*/
int tester(char *mot, Automate *A1);              /*on test si le mot en parametre est accepté par l'automate*/
void tester_from_file(Automate *A1, char text[]); /*On ouvre la liste des mots et on affiche que ceux qui sont acceptés par l'automate en parametre*/

/*Part 3*/
void Automate_Union(Automate *A, Automate *AA, Automate *Union);
void Automate_Etoile(Automate *A, Automate *Etoile);
void Automate_Produit(Automate *A, Automate *AA, Automate *Produit);

/*Part 4*/
int isatt(char *etat, Automate *A);
Automate Atteinte(Automate *A);
Automate supprimer_epsilon(Automate *A);
Automate determiniser(Automate *A);

/* ---------------------- the main function ---------------*/

int main(int argc, char *argv[])
{

      Automate *A = malloc(sizeof(Automate));
      init(A);
      char text1[] = "automate1.txt";
      GetInput(text1, A);
      getAutomate(A);

      Automate *AA = malloc(sizeof(Automate));
      init(AA);
      char text2[] = "automate2.txt";
      GetInput(text2, AA);
      getAutomate(AA);

      Automate *Union = malloc(sizeof(Automate));
      init(Union);

      Automate *Etoile = malloc(sizeof(Automate));
      init(Etoile);

      Automate *Etoile2 = malloc(sizeof(Automate));
      init(Etoile2);

      Automate *Produit = malloc(sizeof(Automate));
      init(Produit);

      char liste[] = "liste.txt";
      char liste2[] = "liste2.txt";

      int choix;
      do
      {

            choix = menu();
            fflush(stdin);

            switch (choix)
            {
            case 1:

                  char name[] = "output1.dot";

                  printf("\n Automate 1:\n");
                  generer_dot(A, name);
                  afficher_arcs(A);

                  printf("\n Automate 2:\n");

                  char name2[] = "output2.dot";

                  afficher_arcs(AA);
                  generer_dot(AA, name2);

                  break;

            case 2:
                  int c = 0;
                  int resultat = 0;
                  char mot[20];

                  printf("Tester sur l'automate 1 ou 2 ?\t");
                  scanf("%d", &c);
                  fflush(stdin);

                  puts("Entrer le mot a tester!");
                  scanf("%s", mot);

                  printf("\nTestons le mot %s :\n", mot);
                  if (c == 1)
                  {
                        resultat = tester(mot, A);
                        if (resultat == 1)
                        {
                              puts("ce mot est accepte!");
                        }
                        else
                        {
                              puts("ce mot n'est pas accepte");
                        }

                        printf("\nTestons les mots du fichier %s, on ecrit seulement les mots qui sont acceptes!\n\n", liste);
                        tester_from_file(A, liste);
                  }

                  if (c == 2)
                  {
                        resultat = tester(mot, AA);
                        if (resultat == 1)
                        {
                              puts("ce mot est accepte!");
                        }
                        else
                        {
                              puts("ce mot n'est pas accepte");
                        }

                        printf("\nTestons les mots du fichier %s, on ecrit seulement les mots qui sont acceptes!\n\n", liste2);
                        tester_from_file(AA, liste2);
                  }

                  break;
            case 3:
                  /*-----------------------------Automate Union----------------*/

                  Automate_Union(A, AA, Union);
                  getAutomate(Union);
                  generer_dot(Union, "union.dot");

                  break;
            case 4:
                  /*-------------------------ETOILE d'un automate---------------*/

                  Automate_Etoile(A, Etoile);
                  getAutomate(Etoile);
                  generer_dot(Etoile, "etoile1.dot");

                  Automate_Etoile(AA, Etoile2);
                  getAutomate(Etoile2);
                  generer_dot(Etoile2, "etoile2.dot");

                  break;
            case 5:
                  /*----------------------Produit de deux automates------------------*/

                  Automate_Produit(A, AA, Produit);
                  Automate P = Atteinte(Produit);
                  getAutomate(&P);
                  generer_dot(&P, "produit.dot");

                  break;

            case 6:
                  Automate u_sanseps = supprimer_epsilon(Union);
                  getAutomate(&u_sanseps);
                  afficher_arcs(&u_sanseps);
                  generer_dot(&u_sanseps, "sanseps_union.dot");

                  break;
            case 7:
                  printf("Etoile du premier Automate\n");
                  Automate e_sanseps1 = supprimer_epsilon(Etoile);
                  getAutomate(&e_sanseps1);
                  afficher_arcs(&e_sanseps1);
                  generer_dot(&e_sanseps1, "sanseps_etoile1.dot");

                  printf("Etoile du deuxieme Automate\n");
                  Automate e_sanseps2 = supprimer_epsilon(Etoile2);
                  getAutomate(&e_sanseps2);
                  afficher_arcs(&e_sanseps2);
                  generer_dot(&e_sanseps2, "sanseps_etoile2.dot");

                  break;
            case 8:

                  /*Free Memory*/

                  free(A);
                  free(AA);
                  free(Union);
                  free(Etoile);
                  free(Etoile2);
                  free(Produit);

                  exit(1);
            }

      } while (choix != 8);

      return 0;
}

/*----------------------------Part 1--------------------------*/
void init(Automate *A1)
{
      A1->nb_etat_fin = 0;
      A1->nb_etat_init = 0;
      A1->nb_arcs = 0;
      A1->nb_etats = 0;
      A1->nb_etiquettes = 0;
}

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
      /* recuperation des Transitions */
      for (i = 0; i < (NombreLines - 2); i++)
      {
            fscanf(fichier, "%s %s %s", A1->arc[i].depart, A1->arc[i].dest, A1->arc[i].etiquette);
      }
      A1->nb_arcs = i - 1;

      /* stocker les etats initiaux */
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

      /* recuperation de la dernière ligne (etats finaux)*/
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

int menu()
{
      int choix;
      printf("\n--------------------------------Menu--------------------------\n");

      printf("1-Afficher les deux automates et generer leur .dot\n");
      printf("2-tester un mot et une liste de mot d'un des automates\n");
      printf("3-Union   : generer .dot\n");
      printf("4-Etoile des deux automates : generer .dot\n");
      printf("5-Produit : generer .dot\n");
      printf("6-Supprimer les epsilons transitions de l'union\n");
      printf("7-supprimer les epsilons transitions de l'etoile\n");
      printf("8-Determiniser un automate\n");
      printf("9-Minimiser un automate\n");
      printf("10-Quitter...\n");
      printf("votre choix est :  ");

      scanf("%d", &choix);
      printf("\n");
      return choix;
}

/* afficher les iterations*/
void afficher_arcs(Automate *A1)
{
      int i;
      for (i = 0; i <= A1->nb_arcs; i++)
      {
            printf("(%s) ---(%s)---> (%s)\n", A1->arc[i].depart, A1->arc[i].etiquette, A1->arc[i].dest);
      }
}

void generer_dot(Automate *A1, char name[])
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

            for (i = 0; i < A1->nb_etat_init; i++)
            {

                  fprintf(file, "\" \"->%s;\n", A1->etat_init[i].nom);
            }

            for (i = 0; i < A1->nb_etat_fin; i++)
            {
                  fprintf(file, "\n%s[shape =doublecircle;];", A1->etat_fin[i].nom);
            }

            fprintf(file, "\nnode [shape = circle;];\n");

            for (i = 0; i <= A1->nb_arcs; i++)
            {

                  fprintf(file, " %s -> %s [label = \"%s\";];\n", A1->arc[i].depart, A1->arc[i].dest, A1->arc[i].etiquette);
            }

            fprintf(file, "}\n");
      }

      fclose(file);
}

/*-----------------------------------Part 2-------------------------------------*/

/* On teste si un mot est accepté par l'automate */
int tester(char *mot, Automate *A1)
{
      int i, j, k;
      char etat_actuel[100];
      int longueur_mot = strlen(mot);

      char temp[2];

      /* On vérifie que le mot est non vide */
      if (longueur_mot == 0)
      {
            return 0;
      }

      /* On initialise l'état actuel à l'un des états initiaux */
      for (i = 0; i < A1->nb_etat_init; i++)
      {
            strcpy(etat_actuel, A1->etat_init[i].nom);

            /* On parcourt chaque caractère du mot */
            for (j = 0; j < longueur_mot; j++)
            {
                  temp[0] = mot[j];
                  temp[1] = '\0';
                  /* On cherche une transition à partir de l'état actuel avec le symbole courant*/
                  int arc_trouve = 0;
                  for (k = 0; k <= A1->nb_arcs; k++)
                  {
                        if ((strcmp(A1->arc[k].depart, etat_actuel) == 0) && (strcmp(A1->arc[k].etiquette, temp) == 0))
                        {
                              strcpy(etat_actuel, A1->arc[k].dest);
                              arc_trouve = 1;
                              break;
                        }
                  }
                  if (arc_trouve == 0)
                  {
                        /* Si aucune transition n'a été trouvée, le mot n'est pas accepté par  l'automate */
                        return 0;
                  }
            }
      }

      /* On vérifie si l'état actuel est un état final */
      return isFinal(etat_actuel, A1);
}

/* tester les mots d'un fichier*/
void tester_from_file(Automate *A1, char text[])
{
      int i;
      int res;

      FILE *liste;
      liste = fopen(text, "r");

      if (liste == NULL)
      {
            printf("ERROR!\n");
            exit(1);
      }

      char chaine[100];
      rewind(liste);
      while (!feof(liste))
      {
            fgets(chaine, 50, liste);
            chaine[strlen(chaine) - 1] = '\0';
            res = tester(chaine, A1);
            if (res == 1)
            {
                  printf("%s\n", chaine);
            }
      }

      fclose(liste);
}

int isInitial(char name[], Automate *A1)
{
      int i;
      for (i = 0; i < A1->nb_etat_init; i++)
      {
            if (strcmp(name, A1->etat_init[i].nom) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

int isFinal(char name[], Automate *A1)
{
      int i;

      for (i = 0; i < A1->nb_etat_fin; i++)
      {
            if (strcmp(name, A1->etat_fin[i].nom) == 0)
            {
                  return 1;
            }
      }
      return 0;
}

/*-------------------------Part 3-----------------------*/

void Automate_Union(Automate *A, Automate *AA, Automate *Union)
{
      int i, k;
      k = 0;
      Union->nb_etat_init = 1;
      Union->nb_etat_fin = 1;
      strcpy(Union->etat_init[0].nom, "start");
      strcpy(Union->etat_fin[0].nom, "end");

      for (i = 0; i <= A->nb_arcs; i++)
      {
            strcpy(Union->arc[i].depart, A->arc[i].depart);
            strcpy(Union->arc[i].dest, A->arc[i].dest);
            strcpy(Union->arc[i].etiquette, A->arc[i].etiquette);
      }
      k = i - 1;

      for (i = 0; i <= AA->nb_arcs; i++)
      {
            k++;
            itoa((atoi(AA->arc[i].depart) + A->nb_etats), Union->arc[k].depart, 10);
            itoa((atoi(AA->arc[i].dest) + A->nb_etats), Union->arc[k].dest, 10);
            strcpy(Union->arc[k].etiquette, AA->arc[i].etiquette);
      }
      for (i = 0; i < A->nb_etat_init; i++)
      {
            k++;
            strcpy(Union->arc[k].depart, "start");
            strcpy(Union->arc[k].dest, A->etat_init[i].nom);
            strcpy(Union->arc[k].etiquette, "epsilon");
      }
      for (i = 0; i < A->nb_etat_fin; i++)
      {
            k++;
            strcpy(Union->arc[k].depart, A->etat_fin[i].nom);
            strcpy(Union->arc[k].dest, "end");
            strcpy(Union->arc[k].etiquette, "epsilon");
      }

      for (i = 0; i < AA->nb_etat_init; i++)
      {
            k++;
            strcpy(Union->arc[k].depart, "start");
            itoa((atoi(AA->etat_init[i].nom) + A->nb_etats), Union->arc[k].dest, 10);
            strcpy(Union->arc[k].etiquette, "epsilon");
      }
      for (i = 0; i < AA->nb_etat_fin; i++)
      {
            k++;
            itoa((atoi(AA->etat_fin[i].nom) + A->nb_etats), Union->arc[k].depart, 10);
            strcpy(Union->arc[k].dest, "end");
            strcpy(Union->arc[k].etiquette, "epsilon");
      }
      Union->nb_arcs = k;
}

void Automate_Etoile(Automate *A, Automate *Etoile)
{
      int i, j, k;
      Etoile->nb_etat_init = 1;
      Etoile->nb_etat_fin = 1;
      strcpy(Etoile->etat_init[0].nom, "start");
      strcpy(Etoile->etat_fin[0].nom, "end");

      for (i = 0; i <= A->nb_arcs; i++)
      {
            strcpy(Etoile->arc[i].depart, A->arc[i].depart);
            strcpy(Etoile->arc[i].dest, A->arc[i].dest);
            strcpy(Etoile->arc[i].etiquette, A->arc[i].etiquette);
      }
      k = i - 1;

      for (i = 0; i < A->nb_etat_init; i++)
      {
            k++;
            strcpy(Etoile->arc[k].depart, "start");
            strcpy(Etoile->arc[k].dest, A->etat_init[i].nom);
            strcpy(Etoile->arc[k].etiquette, "epsilon");
      }
      for (i = 0; i < A->nb_etat_fin; i++)
      {
            k++;
            strcpy(Etoile->arc[k].depart, A->etat_fin[i].nom);
            strcpy(Etoile->arc[k].dest, "end");
            strcpy(Etoile->arc[k].etiquette, "epsilon");
      }
      k++;
      strcpy(Etoile->arc[k].depart, "start");
      strcpy(Etoile->arc[k].dest, "end");
      strcpy(Etoile->arc[k].etiquette, "epsilon");

      for (i = 0; i < A->nb_etat_fin; i++)
      {
            for (j = 0; j < A->nb_etat_init; j++)
            {
                  k++;
                  strcpy(Etoile->arc[k].depart, A->etat_fin[i].nom);
                  strcpy(Etoile->arc[k].dest, A->etat_init[j].nom);
                  strcpy(Etoile->arc[k].etiquette, "epsilon");
            }
      }

      Etoile->nb_arcs = k;
}

void Automate_Produit(Automate *A, Automate *AA, Automate *Produit)
{
      Pair init_states[A->nb_etat_init * AA->nb_etat_init];
      Pair final_states[A->nb_etat_fin * AA->nb_etat_fin];
      Pair states[A->nb_etats * AA->nb_etats];

      arcPair ap[MAX];
      int nb_ap = 0;
      int count = 0;

      int i, j, k, l;

      int nb_init_states = 0;
      int nb_final_states = 0;
      int nb_states = A->nb_etats * AA->nb_etats;

      k = 0;

      for (i = 0; i < A->nb_etats; i++)
      {
            for (j = 0; j < AA->nb_etats; j++)
            {
                  strcpy(states[k].first, A->etats[i].nom);
                  strcpy(states[k].second, AA->etats[j].nom);
                  k++;
            }
      }
      k = 0;
      for (i = 0; i < A->nb_etat_init; i++)
      {
            for (j = 0; j < AA->nb_etat_init; j++)
            {
                  strcpy(init_states[k].first, A->etat_init[i].nom);
                  strcpy(init_states[k].second, AA->etat_init[j].nom);
                  strcpy(Produit->etat_init[k].nom, strcat(init_states[k].first, init_states[k].second));
                  k++;
            }
      }

      nb_init_states = k;
      Produit->nb_etat_init = nb_init_states;

      k = 0;
      for (i = 0; i < A->nb_etat_fin; i++)
      {
            for (j = 0; j < AA->nb_etat_fin; j++)
            {
                  strcpy(final_states[k].first, A->etat_fin[i].nom);
                  strcpy(final_states[k].second, AA->etat_fin[j].nom);
                  strcpy(Produit->etat_fin[k].nom, strcat(final_states[k].first, final_states[k].second));

                  k++;
            }
      }

      nb_final_states = k - 1;
      Produit->nb_etat_fin = nb_final_states;

      for (k = 0; k < nb_states; k++)
      {
            for (i = 0; i <= A->nb_arcs; i++)
            {
                  for (j = 0; j <= AA->nb_arcs; j++)
                  {
                        if (strcmp(A->arc[i].depart, states[k].first) == 0 && strcmp(AA->arc[j].depart, states[k].second) == 0)
                        {
                              for (l = 0; l < nb_states; l++)
                              {
                                    if (strcmp(A->arc[i].dest, states[l].first) == 0 && strcmp(AA->arc[j].dest, states[l].second) == 0 && strcmp(A->arc[i].etiquette, AA->arc[j].etiquette) == 0)
                                    {
                                          strcpy(ap[k].first.depart, states[k].first);
                                          strcpy(ap[k].first.dest, states[l].first);
                                          strcpy(ap[k].first.etiquette, A->arc[i].etiquette);

                                          strcpy(ap[k].second.depart, states[k].second);
                                          strcpy(ap[k].second.dest, states[l].second);
                                          strcpy(ap[k].second.etiquette, AA->arc[j].etiquette);

                                          strcpy(Produit->arc[count].depart, strcat(ap[k].first.depart, ap[k].second.depart));
                                          strcpy(Produit->arc[count].dest, strcat(ap[k].first.dest, ap[k].second.dest));
                                          strcpy(Produit->arc[count].etiquette, ap[k].first.etiquette);
                                          count++;
                                    }
                              }
                        }
                  }
            }
      }

      Produit->nb_arcs = count - 1;
}

/*Part 4*/
int isatt(char *etat, Automate *A)
{
      int i, j;
      int count = 0;

      if ((isInitial(etat, A) == 1) || (isFinal(etat, A) == 1))
      {
            return 1;
      }
      else
      {

            for (i = 0; i <= A->nb_arcs; i++)
            {
                  if (strcmp(etat, A->arc[i].dest) == 0)
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

Automate Atteinte(Automate *A)
{
      int i, j;
      Automate att;
      att.nb_etat_init = A->nb_etat_init;
      att.nb_etat_fin = A->nb_etat_fin;

      for (i = 0; i < A->nb_etat_init; i++)
      {

            strcpy(att.etat_init[i].nom, A->etat_init[i].nom);
      }

      for (i = 0; i < A->nb_etat_fin; i++)
      {

            strcpy(att.etat_fin[i].nom, A->etat_fin[i].nom);
      }
      int k = 0;
      for (j = 0; j <= A->nb_arcs; j++)
      {
            if (isatt(A->arc[j].depart, A) == 1)
            {
                  strcpy(att.arc[k].depart, A->arc[j].depart);
                  strcpy(att.arc[k].dest, A->arc[j].dest);
                  strcpy(att.arc[k].etiquette, A->arc[j].etiquette);
                  k++;
            }
      }
      att.nb_arcs = k - 1;
      return att;
}

Automate supprimer_epsilon(Automate *A)
{

      Automate sanseps;

      int i, m, n;
      int j = 0;
      int k = 0;
      int count = 0;

      int fin = 0;

      for (i = 0; i < A->nb_etat_init; i++)
      {
            strcpy(sanseps.etat_init[i].nom, A->etat_init[i].nom);
      }
      sanseps.nb_etat_init = A->nb_etat_init;

      arcs eps_trans[20];
      int nb_eps = 0;
      for (i = 0; i <= A->nb_arcs; i++)
      {
            if (strcmp(A->arc[i].etiquette, "epsilon") == 0)
            {

                  if (isFinal(A->arc[i].dest, A) == 0)
                  {

                        strcpy(eps_trans[j].etiquette, A->arc[i].etiquette);
                        strcpy(eps_trans[j].depart, A->arc[i].depart);
                        strcpy(eps_trans[j].dest, A->arc[i].dest);
                        j++;
                  }
                  else
                  {
                        strcpy(sanseps.etat_fin[fin].nom, A->arc[i].depart);
                        fin++;
                  }
            }
            else
            {

                  strcpy(sanseps.arc[k].etiquette, A->arc[i].etiquette);
                  strcpy(sanseps.arc[k].depart, A->arc[i].depart);
                  strcpy(sanseps.arc[k].dest, A->arc[i].dest);
                  k++;
            }
      }

      sanseps.nb_etat_fin = fin;

      nb_eps = j;

      for (n = 0; n < nb_eps; n++)
      {
            for (m = 0; m <= A->nb_arcs; m++)
            {

                  if (strcmp(eps_trans[n].dest, A->arc[m].depart) == 0)
                  {

                        strcpy(sanseps.arc[k].etiquette, A->arc[m].etiquette);
                        strcpy(sanseps.arc[k].depart, eps_trans[n].depart);
                        strcpy(sanseps.arc[k].dest, A->arc[m].dest);

                        k++;
                  }
            }
      }

      sanseps.nb_arcs = k - 1;

      for (i = 0; i <= sanseps.nb_arcs; i++)
      {
            if (strcmp(sanseps.arc[i].etiquette, "epsilon") == 0)
            {
                  count++;
            }
      }
      if (count == 0)
      {
            return Atteinte(&sanseps);
      }
      else
      {
            return supprimer_epsilon(&sanseps);
      }
}
