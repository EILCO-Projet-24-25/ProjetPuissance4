#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
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

char choisirPion(char *nomJoueur, char pionchoisi)
{
    if (strcmp(nomJoueur, "Ordinateur") == 0)
    {
        char randomChar;
        do
        {
            randomChar = 'A' + (rand() % 26); // Génère un caractère aléatoire (majuscule)
        } while (randomChar == pionchoisi); // Répète tant que le caractère est exclu
        printf("Pion Ordinateur : %c\n", randomChar);
        return randomChar;
    }
    char pion;
    printf(" %s choisissez votre pion (ex: A - Z) : ", nomJoueur);
    scanf(" %c", &pion);
    if (pionchoisi != '\0' && pionchoisi == pion)
    {
        afficher_erreur("Ce pion est deja choisi par l'autre joueur");
        choisirPion(nomJoueur, pionchoisi == '\0' ? '\0' : pionchoisi);
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
    // Cette fonction compte le nombre de séquences de taille "longueur" formées par "pion"
    // consécutives dans la grille. Ce n'est qu'un exemple simplifié.
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

// Fonction d'évaluation plus avancée pour le niveau 3
// On donne un score en fonction des menaces et possibilités
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
// Implémentation du minimax avec élagage alpha-bêta (Niveau 3)
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
                    break; // élagage
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
                    break; // élagage
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
        // Niveau moyen : Stratégie simple améliorée
        int meilleurCoup = -1;
        int meilleurScore = INT_MIN;

        for (int col = 0; col < colonnes; col++)
        {
            if (est_coup_valide(grille, colonnes, col))
            {
                // Simuler le coup de l'IA
                deposerPignon(grille, col, colonnes, lignes, pion2);
                // Vérifier si ce coup gagne immédiatement
                if (verifier_victoire(grille, lignes, colonnes, pion2))
                {
                    annuler_coup(grille, lignes, col, pion2);
                    deposerPignon(grille, col, colonnes, lignes, pion2);
                    return col; // On joue ce coup direct, victoire !
                }

                // Sinon, vérifier si ce coup empêche une victoire adverse imminente
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
                            // Le fait d'avoir joué dans col empêche ce coup ?
                            // Pas forcément, mais s'il y a un coup qui empêche cette victoire, on le préfère.
                            bloque = 1;
                        }
                        annuler_coup(grille, lignes, c2, pion1);
                    }
                }

                // On évalue rapidement la position après ce coup
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

        // Si aucun coup trouvé, joue aléatoirement
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
        // Niveau difficile : minimax avec élagage alpha-bêta
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
            // Si aucun coup trouvé (improbable), jouer aléatoirement
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

int jouer_tour_joueur(char **grille, int lignes, int colonnes, char pion, char *nomJoueur)
{
    afficherGrille(grille, lignes, colonnes);
    int colonne;
    int coup_valide = 0;
    while (!coup_valide)
    {
        printf("Votre tour (Joueur %s - Pion %c), choisissez une colonne (1 à %d) Ou 0 pour Sauvegarder la partie:  ", nomJoueur, pion, colonnes);
        scanf("%d", &colonne);
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
    // Initialiser le générateur de nombres aléatoires une seule fois
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL)); // Utiliser l'heure actuelle comme graine
        initialized = 1;
    }

    return (rand() % 2) + 1; // Génère un nombre entre 1 et 2
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
        while (jouer_tour_joueur(grille, lignes, colonnes, pion1, joueur1) == 1)
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
        int victoire_ordi = verifier_victoire(grille, lignes, colonnes, pion2);
        if (victoire_ordi == 1)
        {
            afficher_succes("L'Ordi a gagne\n");
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

        while (jouer_tour_joueur(grille, lignes, colonnes, *startedPion, startedJoueur) == 1)
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
        afficherGrille(grille, lignes, colonnes);

        int victoire_joueur = verifier_victoire(grille, lignes, colonnes, *startedPion);
        if (victoire_joueur == 1)
        {
            afficher_succes("Vous avez gagne lar partie\n");
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
            partie_joueur(grille, lignes, colonnes, joueur1, joueur2, difficulte, modeChoisi, choix, pion1, pion2, &startedPion, startedJoueur, &tour);
        }
        if (choix == 6)
        {
            libererGrille(grille, lignes);
            free(joueur1);
            free(joueur2);
            quitterJeu();
        }
    }
}


//Statistiques
// Fonction pour charger les statistiques depuis un fichier
int chargerStatistiques(const char *nomFichier, Statistiques stats[], int tailleMax) {
    FILE *file = fopen(nomFichier, "r");
    if (file == NULL) {
        printf("Fichier de statistiques introuvable, création d'un nouveau fichier.\n");
        return 0;
    }

    int i = 0;
    while (i < tailleMax && fscanf(file, "%s %d %d %d", stats[i].nom, &stats[i].victoires, &stats[i].defaites, &stats[i].nulles) == 4) {
        i++;
    }

    fclose(file);
    return i; // Nombre d'entrées chargées
}

// Fonction pour sauvegarder les statistiques dans un fichier
void sauvegarderStatistiques(const char *nomFichier, Statistiques stats[], int taille) {
    FILE *file = fopen(nomFichier, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de statistiques.\n");
        return;
    }

    for (int i = 0; i < taille; i++) {
        fprintf(file, "%s %d %d %d\n", stats[i].nom, stats[i].victoires, stats[i].defaites, stats[i].nulles);
    }

    fclose(file);
}

// Fonction pour mettre à jour les statistiques d'un joueur
void mettreAJourStatistiques(Statistiques stats[], int *taille, const char *nomJoueur, int victoire, int defaite, int nulle) {
    for (int i = 0; i < *taille; i++) {
        if (strcmp(stats[i].nom, nomJoueur) == 0) {
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