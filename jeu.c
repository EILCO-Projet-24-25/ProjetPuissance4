#include <stdio.h>
#include <stdlib.h>
#include "sauvegarde.h"
#include "menu.h"
#include "parametre.h"

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
    afficherGrille(grille, lignes, colonnes);
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

void lancer_jeu()
{
    char **grille = NULL;
    int lignes, colonnes, modeChoisi, difficulte;
    char *joueur1 = (char *)malloc((12) * sizeof(char));
    char *joueur2 = (char *)malloc((12) * sizeof(char));
    int choix = afficher_menu() - '0';
    // printf("Choix %d\n", choix);
    int oldChoice = 1;
    if (choix == 0)
    {
        oldChoice = choix;
        grille = chargerJeu(&lignes, &colonnes, &difficulte, &modeChoisi, &choix, "savegarde.txt", joueur1, joueur2);
        afficherGrille(grille, lignes, colonnes);

        printf("Lignes %d Colones %d difficultes %d modejeu %d choixjeu %d joueur1 %s joueur2%s\n", lignes, colonnes, difficulte, modeChoisi, choix, joueur1, joueur2);
        printf("Old Choix %d", oldChoice);
        // exit(1);
    }
    if (choix == 1)
    {
        // Mode contre Ordinateur / Joueur
        if (oldChoice != 0)
        {
            grille = definir_mode_jeu(&lignes, &colonnes, &modeChoisi, &difficulte);
            if (modeChoisi == 3)
            {
                lancer_jeu();
            }
            else if (modeChoisi == 1)
            {
                // printf("Mode de jeu %d\n", modeChoisi);
                saisieNomJoueur(joueur1, joueur2, modeChoisi);
                printf("Joueur 1 %s\n", joueur1);
                printf("Joueur 2 %s\n", joueur2);
            }
        }
        if (modeChoisi == 1)
        {
            int jeu_en_cours = 1;
            while (jeu_en_cours)
            {
                // Tour du joueur humain
                //;
                effacer_ecran();
                int quitter = 0;
                while (jouer_tour_joueur(grille, lignes, colonnes) == 1)
                {
                    effacer_ecran();
                    int tmp = sauvegardeMenu() - '0';
                    if (tmp == 1)
                    {
                        // Quitter et sauvegarder
                        sauvegarderJeu(grille, lignes, colonnes, "savegarde.txt", joueur1, joueur2, difficulte - '0', modeChoisi, choix);
                        effacer_ecran();
                        free(grille);
                        free(joueur1);
                        free(joueur2);
                        quitter = 1;
                        break;
                    }
                    else if (tmp == 2)
                    {
                        // Quitter sans sauvegarder
                        effacer_ecran();
                        free(grille);
                        free(joueur1);
                        free(joueur2);
                        quitter = 1;
                        break;
                    }
                }
                if (quitter == 1)
                {
                    lancer_jeu();
                    break;
                }
                afficherGrille(grille, lignes, colonnes);

                int victoire_joueur = verifier_victoire(grille, lignes, colonnes, 'X');
                if (victoire_joueur == 1)
                {
                    printf("Le joueur a gagne\n");
                    jeu_en_cours = 0;
                    break;
                }
                // Tour de l'ordinateur
                printf("Tour de l'ordinateur...\n");
                // printf("difficultes: %d\n", difficulte-'0');
                // exit(1);
                int colonne_ordi = jouer_ordinateur(grille, lignes, colonnes, difficulte - '0');
                printf("L'ordinateur a joue en colonne %d\n", colonne_ordi + 1);
                afficherGrille(grille, lignes, colonnes);

                // Condition de victoire ou de grille pleine ici...
                int victoire_ordi = verifier_victoire(grille, lignes, colonnes, 'O');
                if (victoire_ordi == 1)
                {
                    printf("L'Ordi a gagne\n");
                    jeu_en_cours = 0;
                    break;
                }
            }
            libererGrille(grille, lignes);
        }
    }
    else if (modeChoisi == 2)
    {
        // Jeu contre un autre joueur
        saisieNomJoueur(joueur1, joueur2, modeChoisi);
        printf("Joueur 1 %s\n", joueur1);
        printf("Joueur 2 %s\n", joueur2);
        free(joueur1);
        free(joueur2);
        free(grille);
        exit(1);
    }
}
