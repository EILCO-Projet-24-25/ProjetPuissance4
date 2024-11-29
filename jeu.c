#include <stdio.h>
#include <stdlib.h>
#include "sauvegarde.h"
#include "menu.h"

char **creationGrille(int lignes, int colonnes)
{
    char **grille = (char **)malloc(lignes * sizeof(char *));
    if (grille == NULL)
    {
        printf("Erreur d'allocation de memoire.\n");
        exit(1);
    }

    // Allocation de memoire pour chaque ligne, etant donnee que nous sommes dans un tableau a 2 dimensions
    for (int i = 0; i < lignes; i++)
    {
        grille[i] = (char *)malloc(colonnes * sizeof(char));
        if (grille[i] == NULL)
        {
            printf("Erreur d'allocation de memoire pour la ligne %d.\n", i);
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
    // printf("  ");
    for (int j = 0; j < colonnes; j++)
    {

        printf("%d   ", j + 1);
    }
    printf("\n");
}

int deposerPignon(char **grille, int colone, int colonnes, int lignes, char forme)
{
    if (colone < 0 || colone >= colonnes)
        return 0;

    for (int i = lignes - 1; i >= 0; i--)
    {
        if (grille[i][colone] == ' ')
        {
            grille[i][colone] = forme;
            return 1;
        }
    }
    return 0; // Colonne pleine
}
int verifier_alignement(char **grille, int lignes, int colonnes, int ligne, int col, char joueur)
{
    int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    for (int d = 0; d < 4; d++)
    {
        int count = 0;
        for (int k = 0; k < 4; k++)
        {
            int new_row = ligne + k * directions[d][0];
            int new_col = col + k * directions[d][1];
            if (new_row >= 0 && new_row < lignes && new_col >= 0 && new_col < colonnes &&
                grille[new_row][new_col] == joueur)
            {
                count++;
            }
            else
            {
                break;
            }
        }
        if (count == 4)
            return 1;
    }
    return 0;
}

int verifier_victoire(char **grille, int lignes, int colonnes, char joueur)
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == joueur &&
                (verifier_alignement(grille, lignes, colonnes, i, j, joueur)))
            {
                return 1;
            }
        }
    }
    return 0;
}

// Fonction d'evaluation de la grille (simple)
int evaluer_grille(char **grille, int lignes, int colonnes)
{
    int score = 0;

    // Logique pour evaluer la grille : on peut ajouter des règles pour les alignements
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == 'O')
                score += 1;
            else if (grille[i][j] == 'X')
                score -= 1;
        }
    }
    return score;
}

// Fonction principale pour l'ordinateur avec niveaux de difficulte
int jouer_ordinateur(char **grille, int lignes, int colonnes, int niveau)
{
    int meilleure_colonne = -1;

    // Niveau 1 (Facile) : Choix aleatoire
    if (niveau == 1)
    {
        do
        {
            meilleure_colonne = rand() % colonnes; // Choisit une colonne aleatoire
        } while (grille[0][meilleure_colonne] != ' '); // Verifie que la colonne n'est pas pleine
        deposerPignon(grille, meilleure_colonne, colonnes, lignes, 'O'); // Depose le pion
        return meilleure_colonne;
    }

    // Niveau 2 (Moyen) : Bloque les coups gagnants du joueur
    if (niveau == 2)
    {
        for (int col = 0; col < colonnes; col++)
        {
            if (grille[0][col] == ' ')
            {
                // Simule le coup de l'ordinateur
                deposerPignon(grille, col, colonnes, lignes, 'O');

                if (evaluer_grille(grille, lignes, colonnes) < 0)
                { // Si l'ordinateur bloque la victoire
                    // Bloque le coup du joueur
                    return col;
                }

                // Enlève le pion après simulation pour tester la prochaine colonne
                for (int i = 0; i < lignes; i++)
                {
                    if (grille[i][col] == 'O')
                    {
                        grille[i][col] = ' ';
                        break;
                    }
                }
            }
        }

        // Si aucun blocage necessaire, joue aleatoirement
        do
        {
            meilleure_colonne = rand() % colonnes;
        } while (grille[0][meilleure_colonne] != ' '); // Verifie que la colonne n'est pas pleine
        deposerPignon(grille, meilleure_colonne, colonnes, lignes, 'O');
        return meilleure_colonne;
    }

    // Niveau 3 (Difficile) : Prend la meilleure decision
    if (niveau == 3)
    {
        int meilleure_valeur = INT_MIN;
        // On essaie de jouer dans chaque colonne et on evalue chaque coup
        for (int col = 0; col < colonnes; col++)
        {
            if (grille[0][col] == ' ')
            {
                // Simule le coup de l'ordinateur
                deposerPignon(grille, col, colonnes, lignes, 'O');

                // evalue la grille après avoir depose le pion
                int valeur_coup = evaluer_grille(grille, lignes, colonnes);

                // Si ce coup est le meilleur, on le garde
                if (valeur_coup > meilleure_valeur)
                {
                    meilleure_valeur = valeur_coup;
                    meilleure_colonne = col;
                }

                // Annule la simulation du coup
                for (int i = 0; i < lignes; i++)
                {
                    if (grille[i][col] == 'O')
                    {
                        grille[i][col] = ' ';
                        break;
                    }
                }
            }
        }
        // Depose finalement le pion dans la meilleure colonne trouvee
        deposerPignon(grille, meilleure_colonne, colonnes, lignes, 'O');
        return meilleure_colonne;
    }

    return -1; // Aucun coup valide n'est trouve
}

int jouer_tour_joueur(char **grille, int lignes, int colonnes)
{
    int colonne;
    int coup_valide = 0;
    while (!coup_valide)
    {
        printf("Votre tour (Joueur %c), choisissez une colonne (1 à %d) Ou 0 pour Sauvegarder la partie:  ", 'X', colonnes);
        scanf("%d", &colonne);
        if (colonne == 0)
        {
            return 1;
        }
        colonne -= 1; // Convertit en index 0-based

        if (colonne >= 0 && colonne < colonnes)
        {
            coup_valide = deposerPignon(grille, colonne, colonnes, lignes, 'X');
            if (!coup_valide)
            {
                printf("Colonne pleine. Choisissez une autre colonne.\n");
            }
        }
        else
        {
            afficher_erreur("Choix invalide. Veuillez choisir bonne colonne.\n");
        }
    }
    return 0;
}
