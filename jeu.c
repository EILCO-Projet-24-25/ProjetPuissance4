#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include "sauvegarde.h"
#include "menu.h"
#include "parametre.h"
#include "temps.h"
#include "jeu.h"

#ifdef _WIN32

#else
#include <limits.h>
#endif
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
void viderGrille(char **grille, int lignes, int colonnes)
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            grille[i][j] = ' ';
        }
    }
}

int estGrillePleine(char **grille, int lignes, int colonnes)
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == ' ')
            {
                // Il y a au moins une case vide, la grille n'est pas pleine
                return 0;
            }
        }
    }
    // Toutes les cases sont remplies
    return 1;
}
void afficherGrille(char **grille, int lignes, int colonnes)
{
    // Variables pour stocker les deux types de pions
    char pion1 = '\0';
    char pion2 = '\0';

    // Parcourir la grille pour identifier les deux symboles distincts
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            char c = grille[i][j];
            if (c != ' ' && c != pion1 && c != pion2)
            {
                if (pion1 == '\0')
                {
                    pion1 = c; // Premier symbole détecté
                }
                else if (pion2 == '\0')
                {
                    pion2 = c; // Second symbole détecté
                }
            }
            // Si les deux symboles sont déjà trouvés, arrêter la recherche
            if (pion1 != '\0' && pion2 != '\0')
                break;
        }
        if (pion1 != '\0' && pion2 != '\0')
            break;
    }

    // Si un seul symbole est présent, assigner le même symbole au deuxième pour éviter les erreurs
    if (pion2 == '\0' && pion1 != '\0')
    {
        pion2 = pion1;
    }

    // Trier les symboles pour une assignation cohérente des couleurs
    // Par exemple, pion1 < pion2 en ordre ASCII
    if (pion1 > pion2 && pion2 != '\0')
    {
        char temp = pion1;
        pion1 = pion2;
        pion2 = temp;
    }

    // Assigner des couleurs directement dans les printf
    // Premier symbole (après tri) en rouge, second en jaune
    printf("\n");
    for (int i = 0; i < lignes; i++)
    {
        printf("|");
        for (int j = 0; j < colonnes; j++)
        {
            char c = grille[i][j];
            if (c == pion1 && pion1 != '\0')
            {
                // Appliquer la première couleur (rouge)
                printf("\033[1;31m %c \033[0m|", c);
            }
            else if (c == pion2 && pion2 != '\0' && pion2 != pion1)
            {
                // Appliquer la deuxième couleur (jaune)
                printf("\033[1;33m %c \033[0m|", c);
            }
            else
            {
                // Afficher la case vide ou autre symbole sans couleur
                printf(" %c |", c);
            }
        }
        printf("\n");
        // Afficher les lignes de séparation
        for (int k = 0; k < colonnes; k++)
        {
            printf("----");
        }
        printf("-\n");
    }

    // Afficher les indices des colonnes
    for (int j = 0; j < colonnes; j++)
    {
        printf(" %d  ", j + 1);
    }
    printf("\n");
}
char choisirPion(char *nomJoueur, char pionchoisi)
{
    if (strcmp(nomJoueur, "Ordinateur") == 0)
    {
        char randomChar;
        do
        {
            randomChar = 'A' + (rand() % 26); // Genère un caractère aleatoire (majuscule)
        } while (randomChar == pionchoisi); // Repète tant que le caractère est exclu
        printf("Pion Ordinateur : %c\n", randomChar);
        return randomChar;
    }
    char pion;
    printf(" %s choisissez votre pion (ex: A - Z) : ", nomJoueur);
    scanf(" %c", &pion);
    if (pionchoisi != '\0' && pionchoisi == pion)
    {
        afficher_erreur("Ce pion est deja choisi par l'autre joueur");
        return choisirPion(nomJoueur, pionchoisi == '\0' ? '\0' : pionchoisi);
    }
    printf("Votre pion : %c\n", pion);
    return pion;
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

int compter_alignements(char **grille, int lignes, int colonnes, char pion, int longueur)
{
    // Cette fonction compte le nombre de sequences de taille "longueur" formees par "pion"
    // consecutives dans la grille. Ce n'est qu'un exemple simplifie.
    int total = 0;

    // Horizontal
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j <= colonnes - longueur; j++)
        {
            int k;
            for (k = 0; k < longueur; k++)
            {
                if (grille[i][j + k] != pion)
                    break;
            }
            if (k == longueur)
                total++;
        }
    }

    // Vertical
    for (int j = 0; j < colonnes; j++)
    {
        for (int i = 0; i <= lignes - longueur; i++)
        {
            int k;
            for (k = 0; k < longueur; k++)
            {
                if (grille[i + k][j] != pion)
                    break;
            }
            if (k == longueur)
                total++;
        }
    }

    // Diagonale descendante
    for (int i = 0; i <= lignes - longueur; i++)
    {
        for (int j = 0; j <= colonnes - longueur; j++)
        {
            int k;
            for (k = 0; k < longueur; k++)
            {
                if (grille[i + k][j + k] != pion)
                    break;
            }
            if (k == longueur)
                total++;
        }
    }

    // Diagonale montante
    for (int i = longueur - 1; i < lignes; i++)
    {
        for (int j = 0; j <= colonnes - longueur; j++)
        {
            int k;
            for (k = 0; k < longueur; k++)
            {
                if (grille[i - k][j + k] != pion)
                    break;
            }
            if (k == longueur)
                total++;
        }
    }

    return total;
}

int est_coup_valide(char **grille, int colonnes, int col)
{
    return (col >= 0 && col < colonnes && grille[0][col] == ' ');
}

void annuler_coup(char **grille, int lignes, int col, char pion)
{
    // Retire le pion de la colonne (simuler un "undo")
    for (int i = 0; i < lignes; i++)
    {
        if (grille[i][col] == pion)
        {
            grille[i][col] = ' ';
            break;
        }
    }
}

// Fonction d'evaluation plus avancee pour le niveau 3
// On donne un score en fonction des menaces et possibilites
int evaluer_position(char **grille, int lignes, int colonnes, char pionIA, char pionAdverse)
{
    // Si l'IA gagne
    if (verifier_victoire(grille, lignes, colonnes, pionIA))
        return 100000;
    // Si l'adversaire gagne
    if (verifier_victoire(grille, lignes, colonnes, pionAdverse))
        return -100000;

    // Compter les alignements 3 (quasi victoire) pour IA et adversaire
    int troisIA = compter_alignements(grille, lignes, colonnes, pionIA, 3);
    int troisAdv = compter_alignements(grille, lignes, colonnes, pionAdverse, 3);
    int deuxIA = compter_alignements(grille, lignes, colonnes, pionIA, 2);
    int deuxAdv = compter_alignements(grille, lignes, colonnes, pionAdverse, 2);

    // Score simple : plus il y a d'alignements partiels pour l'IA, mieux c'est, et inversement
    int score = 0;
    score += troisIA * 100;
    score -= troisAdv * 100;
    score += deuxIA * 10;
    score -= deuxAdv * 10;

    return score;
}

//=====================================================================
// Implementation du minimax avec elagage alpha-bêta (Niveau 3)
//=====================================================================
int minimax(char **grille, int lignes, int colonnes, int profondeur, int alpha, int beta, int maximisingPlayer, char pionIA, char pionAdverse)
{
    // Condition terminale
    int evaluation = evaluer_position(grille, lignes, colonnes, pionIA, pionAdverse);
    if (profondeur == 0 || evaluation == 100000 || evaluation == -100000)
    {
        return evaluation;
    }

    if (maximisingPlayer)
    {
        int maxEval = INT_MIN;
        for (int col = 0; col < colonnes; col++)
        {
            if (est_coup_valide(grille, colonnes, col))
            {
                deposerPignon(grille, col, colonnes, lignes, pionIA);
                int eval = minimax(grille, lignes, colonnes, profondeur - 1, alpha, beta, 0, pionIA, pionAdverse);
                annuler_coup(grille, lignes, col, pionIA);
                if (eval > maxEval)
                    maxEval = eval;
                if (eval > alpha)
                    alpha = eval;
                if (beta <= alpha)
                    break; // elagage
            }
        }
        return maxEval;
    }
    else
    {
        int minEval = INT_MAX;
        for (int col = 0; col < colonnes; col++)
        {
            if (est_coup_valide(grille, colonnes, col))
            {
                deposerPignon(grille, col, colonnes, lignes, pionAdverse);
                int eval = minimax(grille, lignes, colonnes, profondeur - 1, alpha, beta, 1, pionIA, pionAdverse);
                annuler_coup(grille, lignes, col, pionAdverse);
                if (eval < minEval)
                    minEval = eval;
                if (eval < beta)
                    beta = eval;
                if (beta <= alpha)
                    break; // elagage
            }
        }
        return minEval;
    }
}

// Fonction principale pour l'ordinateur avec niveaux de difficulte
int jouer_ordinateur(char **grille, int lignes, int colonnes, int niveau, char pion1, char pion2)
{
    printf("Ordinateur Ligne %d Colones %d Niveau %d Pion1 %c Pion2%c\n", lignes, colonnes, niveau, pion1, pion2);

    int meilleure_colonne = -1;

    // Niveau 1 (Facile) : Choix aleatoire
    if (niveau == 1)
    {
        do
        {
            meilleure_colonne = rand() % colonnes; // Choisit une colonne aleatoire
        } while (grille[0][meilleure_colonne] != ' '); // Verifie que la colonne n'est pas pleine
        deposerPignon(grille, meilleure_colonne, colonnes, lignes, pion2); // Depose le pion
        return meilleure_colonne;
    }

    // Niveau 2 (Moyen) : Bloque les coups gagnants du joueur
    if (niveau == 2)
    {
        // Niveau moyen : Strategie simple amelioree
        int meilleurCoup = -1;
        int meilleurScore = INT_MIN;

        for (int col = 0; col < colonnes; col++)
        {
            if (est_coup_valide(grille, colonnes, col))
            {
                // Simuler le coup de l'IA
                deposerPignon(grille, col, colonnes, lignes, pion2);
                // Verifier si ce coup gagne immediatement
                if (verifier_victoire(grille, lignes, colonnes, pion2))
                {
                    annuler_coup(grille, lignes, col, pion2);
                    deposerPignon(grille, col, colonnes, lignes, pion2);
                    return col; // On joue ce coup direct, victoire !
                }

                // Sinon, verifier si ce coup empêche une victoire adverse imminente
                // Simuler le coup adverse suivant et voir s'il aurait pu gagner
                int bloque = 0;
                for (int c2 = 0; c2 < colonnes; c2++)
                {
                    if (est_coup_valide(grille, colonnes, c2))
                    {
                        deposerPignon(grille, c2, colonnes, lignes, pion1);
                        if (verifier_victoire(grille, lignes, colonnes, pion1))
                        {
                            // Le joueur aurait pu gagner s'il jouait ici au prochain coup
                            // Le fait d'avoir joue dans col empêche ce coup ?
                            // Pas forcement, mais s'il y a un coup qui empêche cette victoire, on le prefère.
                            bloque = 1;
                        }
                        annuler_coup(grille, lignes, c2, pion1);
                    }
                }

                // On evalue rapidement la position après ce coup
                int score = 0;
                if (bloque)
                    score += 500; // On donne un bonus si on bloque une menace
                // Ajout d'une petite heuristique
                score += evaluer_position(grille, lignes, colonnes, pion2, pion1);

                annuler_coup(grille, lignes, col, pion2);

                if (score > meilleurScore)
                {
                    meilleurScore = score;
                    meilleurCoup = col;
                }
            }
        }

        // Si aucun coup trouve, joue aleatoirement
        if (meilleurCoup == -1)
        {
            int col;
            do
            {
                col = rand() % colonnes;
            } while (!est_coup_valide(grille, colonnes, col));
            deposerPignon(grille, col, colonnes, lignes, pion2);
            return col;
        }
        else
        {
            deposerPignon(grille, meilleurCoup, colonnes, lignes, pion2);
            return meilleurCoup;
        }
    }

    if (niveau == 3)
    {
        // Niveau difficile : minimax avec elagage alpha-bêta
        int profondeur = 4; // Vous pouvez ajuster la profondeur
        int meilleurCoup = -1;
        int meilleurScore = INT_MIN;

        for (int col = 0; col < colonnes; col++)
        {
            if (est_coup_valide(grille, colonnes, col))
            {
                deposerPignon(grille, col, colonnes, lignes, pion2);
                int score = minimax(grille, lignes, colonnes, profondeur - 1, INT_MIN, INT_MAX, 0, pion2, pion1);
                annuler_coup(grille, lignes, col, pion2);

                if (score > meilleurScore)
                {
                    meilleurScore = score;
                    meilleurCoup = col;
                }
            }
        }

        if (meilleurCoup == -1)
        {
            // Si aucun coup trouve (improbable), jouer aleatoirement
            int col;
            do
            {
                col = rand() % colonnes;
            } while (!est_coup_valide(grille, colonnes, col));
            deposerPignon(grille, col, colonnes, lignes, pion2);
            return col;
        }
        else
        {
            deposerPignon(grille, meilleurCoup, colonnes, lignes, pion2);
            return meilleurCoup;
        }
    }

    return -1; // Aucun coup valide n'est trouve
}

int jouer_tour_joueur(char **grille, int lignes, int colonnes, char pion, char *nomJoueur, int difficulte)
{
    afficherGrille(grille, lignes, colonnes);
    int colonne;
    int coup_valide = 0;

    int timeout;
    if (difficulte == 0)
    {
        timeout = 5000;
    }
    else if (difficulte == 1)
    {
        timeout = 10000;
    }
    else if (difficulte == 2)
    {
        timeout = 6000;
    }
    else if (difficulte == 3)
    {
        timeout = 3000;
    }
    // fflush(stdout);

    while (!coup_valide)
    {
        printf("Temps de saisie %d secondes\n", timeout / 1000);
        printf("Votre tour (Joueur %s - Pion %c) choisissez une colonne (1 à %d) Ou 0 pour Sauvegarder la partie:  ", nomJoueur, pion, colonnes);
        // scanf("%d", &colonne);
        int saisi = saisir_entier_avec_gestion_temps(timeout, &colonne);
        if (!saisi)
        {
            return 0; // Retourne 0 pour indiquer un echec
        }
        if (colonne == 0)
        {
            return 1;
        }
        colonne -= 1; // Convertit en index 0-based

        if (colonne >= 0 && colonne < colonnes)
        {
            coup_valide = deposerPignon(grille, colonne, colonnes, lignes, pion);
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

int random_1_2()
{
    // Initialiser le generateur de nombres aleatoires une seule fois
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL)); // Utiliser l'heure actuelle comme graine
        initialized = 1;
    }

    return (rand() % 2) + 1; // Genère un nombre entre 1 et 2
}

void partie_ordi(char **grille, int lignes, int colonnes, char *joueur1, char *joueur2, int difficulte, int modeChoisi, int choix, char pion1, char pion2)
{
    int jeu_en_cours = 1;
    while (jeu_en_cours)
    {
        // Tour du joueur humain
        //;
        effacer_ecran();
        int quitter = 0;
        while (jouer_tour_joueur(grille, lignes, colonnes, pion1, joueur1, difficulte) == 1)
        {
            // effacer_ecran();
            int tmp = sauvegardeMenu();
            if (tmp == 1)
            {
                // Quitter et sauvegarder
                sauvegarderJeu(grille, lignes, colonnes, "savegarde.txt", joueur1, joueur2, difficulte, modeChoisi, choix, pion1, pion2);
                // effacer_ecran();
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

        int victoire_joueur = verifier_victoire(grille, lignes, colonnes, pion1);
        if (victoire_joueur == 1)
        {
            afficher_succes("Le joueur a gagne\n");
            if (difficulte == 1)
            {
                update_statistics(joueur1, 1, 50);
                update_statistics(joueur2, 0, 10);
            }
            else if (difficulte == 2)
            {
                update_statistics(joueur1, 1, 100);
                update_statistics(joueur2, 0, 10);
            }
            else if (difficulte == 3)
            {
                update_statistics(joueur1, 1, 150);
                update_statistics(joueur2, 0, 10);
            }
            jeu_en_cours = 0;
            break;
        }
        // Tour de l'ordinateur
        printf("Tour de l'ordinateur...\n");
        // printf("difficultes: %d\n", difficulte-'0');
        // exit(1);
        int colonne_ordi = jouer_ordinateur(grille, lignes, colonnes, difficulte, pion1, pion2);
        printf("L'ordinateur a joue en colonne %d\n", colonne_ordi + 1);
        afficherGrille(grille, lignes, colonnes);

        // Condition de victoire ou de grille pleine ici...
        if (estGrillePleine(grille, lignes, colonnes) == 1)
        {
            afficher_erreur("La grille est pleine. Match nul.\n");
            jeu_en_cours = 0;
            break;
        }
        int victoire_ordi = verifier_victoire(grille, lignes, colonnes, pion2);
        if (victoire_ordi == 1)
        {
            afficher_succes("L'Ordi a gagne\n");

            if (difficulte == 1)
            {
                update_statistics(joueur2, 1, 50);
                update_statistics(joueur1, 0, 10);
            }
            else if (difficulte == 2)
            {
                update_statistics(joueur2, 1, 100);
                update_statistics(joueur1, 0, 10);
            }
            else if (difficulte == 3)
            {
                update_statistics(joueur2, 1, 150);
                update_statistics(joueur1, 0, 10);
            }
            jeu_en_cours = 0;
            break;
        }
    }

    int choix_fin_partie = recommencerPartie(grille, lignes, joueur1, joueur2);
    if (choix_fin_partie == 1)
    {
        effacer_ecran();
        viderGrille(grille, lignes, colonnes);
        partie_ordi(grille, lignes, colonnes, joueur1, joueur2, difficulte, modeChoisi, choix, pion1, pion2);
    }
}
void partie_joueur(char **grille, int lignes, int colonnes, char *joueur1, char *joueur2, int difficulte, int modeChoisi, int choix, char pion1, char pion2, char *startedPion, char *startedJoueur, int *tour)
{
    int jeu_en_cours = 1;

    while (jeu_en_cours)
    {
        // Tour du joueur humain
        //;
        effacer_ecran();
        int quitter = 0;
        if (*tour == 1)
        {
            strcpy(startedJoueur, joueur1);
            *startedPion = pion1;
        }
        else
        {
            strcpy(startedJoueur, joueur2);
            *startedPion = pion2;
        }

        while (jouer_tour_joueur(grille, lignes, colonnes, *startedPion, startedJoueur, difficulte) == 1)
        {
            // effacer_ecran();
            int tmp = sauvegardeMenu();
            if (tmp == 1)
            {
                // Quitter et sauvegarder
                sauvegarderJeu(grille, lignes, colonnes, "savegarde.txt", joueur1, joueur2, *tour, modeChoisi, choix, pion1, pion2);
                // effacer_ecran();
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
        printf("\n");
        afficherGrille(grille, lignes, colonnes);
        if (estGrillePleine(grille, lignes, colonnes) == 1)
        {
            afficher_erreur("La grille est pleine. Match nul.\n");
            jeu_en_cours = 0;
            break;
        }
        int victoire_joueur = verifier_victoire(grille, lignes, colonnes, *startedPion);
        if (victoire_joueur == 1)
        {
            afficher_succes("Vous avez gagne la partie\n");
            // printf("diffucultes %d\n", difficulte);

            if (strcmp(startedJoueur, joueur1) == 0)
            {
                update_statistics(startedJoueur, 1, 50);
                update_statistics(joueur2, 0, 10);
            }
            else if (strcmp(startedJoueur, joueur2) == 0)
            {
                {
                    update_statistics(startedJoueur, 1, 50);
                    update_statistics(joueur1, 0, 10);
                }
            }

            jeu_en_cours = 0;
            break;
        }
        *tour = (*tour % 2) + 1;
    }

    int choix_fin_partie = recommencerPartie(grille, lignes, joueur1, joueur2);
    if (choix_fin_partie == 1)
    {
        effacer_ecran();
        viderGrille(grille, lignes, colonnes);
        *tour = (*tour % 2) + 1;
        partie_joueur(grille, lignes, colonnes, joueur1, joueur2, difficulte, modeChoisi, choix, pion1, pion2, startedPion, startedJoueur, tour);
    }
}

void lancer_jeu()
{
    char **grille = NULL;
    int lignes, colonnes, modeChoisi, difficulte;
    char *joueur1 = (char *)malloc((12) * sizeof(char));
    char *joueur2 = (char *)malloc((12) * sizeof(char));
    int choix = afficher_menu() - '0';
    char pion1, pion2;
    int tour = random_1_2();
    char *startedJoueur = (char *)malloc(12 * sizeof(char));
    char startedPion;

    // printf("Choix %d\n", choix);
    int oldChoice = 1;
    if (choix == 0)
    {
        oldChoice = choix;
        grille = chargerJeu(&lignes, &colonnes, &difficulte, &modeChoisi, &choix, "savegarde.txt", joueur1, joueur2, &pion1, &pion2);
        remove("savegarde.txt");
        afficherGrille(grille, lignes, colonnes);
        if (modeChoisi == 2)
        {
            tour = difficulte;
        }
        // printf("Lignes %d Colones %d difficultes %d modejeu %d choixjeu %d joueur1 %s joueur2%s pion1 %c pion2 %c\n", lignes, colonnes, difficulte, modeChoisi, choix, joueur1, joueur2, pion1, pion2);
        // printf("Old Choix %d", oldChoice);
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
                // test affichage
                //  printf("Joueur 1 %s\n", joueur1);
                //  printf("Joueur 2 %s\n", joueur2);
                pion1 = choisirPion(joueur1, '\0');
                pion2 = choisirPion(joueur2, pion1);
                printf("Le pion du joueur1 %s est %c et le pion du joueur2 est %s et le pion %c\n", joueur1, pion1, joueur2, pion2);
                // exit(1);
            }
            else if (modeChoisi == 2)
            {
                saisieNomJoueur(joueur1, joueur2, modeChoisi);
                printf("Joueur 1 %s\n", joueur1);
                printf("Joueur 2 %s\n", joueur2);
                printf("Choix des pions, le joueur %s commencera la partie en 1ere\n", tour == 1 ? joueur1 : joueur2);
                if (tour == 1)
                {
                    pion1 = choisirPion(joueur1, '\0');
                    pion2 = choisirPion(joueur2, pion1);
                }
                else
                {
                    pion2 = choisirPion(joueur2, '\0');
                    pion1 = choisirPion(joueur1, pion2);
                }
            }
        }
        if (modeChoisi == 1)
        {
            partie_ordi(grille, lignes, colonnes, joueur1, joueur2, difficulte, modeChoisi, choix, pion1, pion2);
        }

        else if (modeChoisi == 2)
        {
            // Jeu contre un autre joueur
            difficulte = 0;
            partie_joueur(grille, lignes, colonnes, joueur1, joueur2, difficulte, modeChoisi, choix, pion1, pion2, &startedPion, startedJoueur, &tour);
        }
        if (choix == 5)
        {
            libererGrille(grille, lignes);
            free(joueur1);
            free(joueur2);
            quitterJeu();
        }
    }
}

// Statistiques
//  Fonction pour charger les statistiques depuis un fichier
int chargerStatistiques(const char *nomFichier, StatistiqueJoueur stats[], int tailleMax)
{
    FILE *file = fopen(nomFichier, "r");
    if (file == NULL)
    {
        printf("Fichier de statistiques introuvable, creation d'un nouveau fichier.\n");
        return 0;
    }

    int i = 0;
    while (i < tailleMax && fscanf(file, "%s %d %d %d", stats[i].nom, &stats[i].victoires, &stats[i].defaites, &stats[i].nulles) == 4)
    {
        i++;
    }

    fclose(file);
    return i; // Nombre d'entrees chargees
}

// Fonction pour sauvegarder les statistiques dans un fichier
void sauvegarderStatistiques(const char *nomFichier, StatistiqueJoueur stats[], int taille)
{
    FILE *file = fopen(nomFichier, "w");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier de statistiques.\n");
        return;
    }

    for (int i = 0; i < taille; i++)
    {
        fprintf(file, "%s %d %d %d\n", stats[i].nom, stats[i].victoires, stats[i].defaites, stats[i].nulles);
    }

    fclose(file);
}

// Fonction pour mettre à jour les statistiques d'un joueur
void mettreAJourStatistiques(StatistiqueJoueur stats[], int *taille, const char *nomJoueur, int victoire, int defaite, int nulle)
{
    for (int i = 0; i < *taille; i++)
    {
        if (strcmp(stats[i].nom, nomJoueur) == 0)
        {
            stats[i].victoires += victoire;
            stats[i].defaites += defaite;
            stats[i].nulles += nulle;
            return;
        }
    }

    // Si le joueur n'existe pas, on l'ajoute
    strcpy(stats[*taille].nom, nomJoueur);
    stats[*taille].victoires = victoire;
    stats[*taille].defaites = defaite;
    stats[*taille].nulles = nulle;
    (*taille)++;
}