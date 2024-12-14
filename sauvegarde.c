#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

int verifierFichier(const char *nomFichier)
{
    FILE *fichier = fopen(nomFichier, "r");

    // Vérifier si le fichier existe
    if (fichier == NULL)
    {
        // printf("Le fichier \"%s\" n'existe pas.\n", nomFichier);
        return 0; // Le fichier n'existe pas
    }

    // Se déplacer à la fin du fichier pour obtenir sa taille
    fseek(fichier, 0, SEEK_END);
    long taille = ftell(fichier); // Obtenir la position actuelle (taille du fichier)
    fclose(fichier);              // Fermer le fichier après vérification

    if (taille > 0)
    {
        // printf("Le fichier \"%s\" existe et contient des données.\n", nomFichier);
        return 1; // Le fichier existe et contient des données
    }
    else
    {
        // printf("Le fichier \"%s\" existe mais est vide.\n", nomFichier);
        return 2; // Le fichier existe mais est vide
    }
}

void sauvegarderJeu(char **grille, int lignes, int colonnes, const char *nomFichier, char *joueur1, char *joueur2, int difficultes, int modejeu, int choixjeu, char pion1, char pion2)
{
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    fprintf(fichier, "%d %d %d %d %d %s %s %c %c\n", lignes, colonnes, difficultes, modejeu, choixjeu, joueur1, joueur2, pion1, pion2); // Sauvegarde du nombre de lignes, colones, difficultes, mode de jeu, joueur1, joueur2

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == ' ')
            {
                fprintf(fichier, "%c ", '0');
            }
            else
            {
                fprintf(fichier, "%c ", grille[i][j]);
            }
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}

char **chargerJeu(int *lignes, int *colonnes, int *difficultes, int *modejeu, int *choixjeu, const char *nomFichier, char *joueur1, char *joueur2, char *pion1, char *pion2)
{
    printf("Charger une partie");
    char **grille = NULL;
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Lecture de la première ligne pour les métadonnées
    if (fscanf(fichier, "%d %d %d %d %d %s %s %c %c\n", lignes, colonnes, difficultes, modejeu, choixjeu, joueur1, joueur2, pion1, pion2) != 9)
    {
        fprintf(stderr, "\nErreur lors de la lecture des metadonnees du fichier.\n");
        fclose(fichier);
        exit(1);
    }

    grille = creationGrille(*lignes, *colonnes);
    // Lecture des lignes restantes pour la grille
    for (int i = 0; i < *lignes; i++)
    {
        for (int j = 0; j < *colonnes; j++)
        {
            char c;
            if (fscanf(fichier, " %c", &c) != 1)
            {
                fprintf(stderr, "Erreur lors de la lecture de la grille.\n");
                // Libération de la mémoire en cas d'erreur
                for (int k = 0; k < *lignes; k++)
                {
                    free(grille[k]);
                }
                free(grille);
                fclose(fichier);
                exit(EXIT_FAILURE);
            }
            // Remplacer '0' par ' ' et stocker dans la grille
            grille[i][j] = (c == '0') ? ' ' : c;
        }
    }

    fclose(fichier);
    return grille;
}
