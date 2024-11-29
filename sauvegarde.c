#include <stdio.h>

void sauvegarderJeu(char **grille, int lignes, int colonnes, const char *nomFichier, char *joueur1, char *joueur2)
{
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    fprintf(fichier, "%d %d\n", lignes, colonnes); // Sauvegarde des dimensions

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

void chargerJeu(char **grille, int lignes, int colonnes, const char *nomFichier)
{
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    int l, c;
    fscanf(fichier, "%d %d", &l, &c);
    if (l != lignes || c != colonnes)
    {
        printf("Les dimensions du fichier ne correspondent pas � la grille\n");
        fclose(fichier);
        return;
    }

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            fscanf(fichier, " %c", &grille[i][j]);
        }
    }

    fclose(fichier);
}
