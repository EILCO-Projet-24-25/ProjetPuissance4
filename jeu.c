#include <stdio.h>
#include <stdlib.h>

char **creationGrille(int lignes, int colonnes)
{
    char **grille = (char **)malloc(lignes * sizeof(char *));
    if (grille == NULL)
    {
        printf("Erreur d'allocation de mémoire.\n");
        exit(1);
    }

    // Allocation de mémoire pour chaque ligne, etant donnee que nous sommes dans un tableau a 2 dimensions
    for (int i = 0; i < lignes; i++)
    {
        grille[i] = (char *)malloc(colonnes * sizeof(char));
        if (grille[i] == NULL)
        {
            printf("Erreur d'allocation de mémoire pour la ligne %d.\n", i);
            exit(1);
        }
    }

    // Initialisation de la grille avec des espaces vides
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            grille[i][j] = ' ';
        }
    }

    return grille;
}

void libererGrille(char **grille, int lignes)
{
    for (int i = 0; i < lignes; i++)
    {
        free(grille[i]);
    }
    free(grille);
}

void afficherGrille(char **grille, int lignes, int colonnes)
{
    for (int i = 0; i < lignes; i++)
    {
        printf("|");
        for (int j = 0; j < colonnes; j++)
        {
            printf(" %c |", grille[i][j]);
        }
        printf("\n");
        for (int k = 0; k < colonnes; k++)
        {
            printf("----");
        }
        printf("-\n");
    }
    printf("  ");
    for (int j = 0; j < colonnes; j++)
    {
        printf(" %d  ", j + 1);
    }
    printf("\n");
}
