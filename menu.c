#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "jeu.h"

void effacer_ecran()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void afficher_erreur(const char *message) {
    printf("\033[1;31m");  // Code ANSI pour texte rouge
    printf("%s\n", message);
    printf("\033[0m");  // Réinitialisation de la couleur
}

char **definir_mode_jeu(int *lignes, int *colones, int *modeChoisi)
{
    char mode;
    char **grille = NULL;
    do
    {
        effacer_ecran();
        printf("\n=== Choisir le Mode de Jeu ===\n");
        printf("1. Jouer contre le Bot\n");
        printf("2. Joueur contre Joueur\n");
        printf("Entrez votre choix : \t");
        scanf("%c", &mode);
        // Nettoyer le tampon avant de saisir un choix
        while (getchar() != '\n')
            ; // Vide le tampon de saisie
        *modeChoisi = mode - '0';
        if (mode == '1')
        {
            printf("Mode selectionne : Jouer contre le Bot\n");
            printf("ligne de la grille: \t");
            scanf("%d", lignes);
            printf("\ncolone de la grille: \t");
            scanf("%d", colones);
            grille = creationGrille(*lignes, *colones);
        }
        else if (mode == '2')
        {
            printf("Mode selectionne : Joueur contre Joueur\n");
            printf("ligne de la grille: \t");
            scanf("%d", lignes);
            printf("\ncolone de la grille: \t");
            scanf("%d", colones);
            grille = creationGrille(*lignes, *colones);
        }
        else
        {
            afficher_erreur("Entree non valide. Veuillez entrer un chiffre entre 1-2.");
            // printf("Entree non valide. Veuillez entrer un chiffre entre 1-2.\n");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            getchar(); // Attendre que l'utilisateur appuie sur une touche
            effacer_ecran();
        }
    } while (!isdigit(mode) || mode != '1' || mode != '2');
    return grille;
}

char afficher_menu()
{
    effacer_ecran();
    char choix;
    printf("\n=== Menu Principal ===\n");
    printf("1. Choisir le Mode de Jeu (vs Bot ou vs Joueur)\n");
    printf("2. Revoir une partie\n");
    printf("3. Parametres\n");
    printf("4. Aide\n");
    printf("5. Statistiques\n");
    printf("6. Quitter\n");

    printf("Entrez votre choix : \t");
    scanf("%c", &choix);
    if (isdigit(choix))
    {
        switch (choix)
        {
        case '1':
            printf("Choix du mode de jeu\n");
            break;
        case '2':
            printf("Revoir une partie\n");
            break;
        case '3':
            printf("Parametres\n");
            break;
        case '4':
            printf("Aide\n");
            break;
        case '5':
            printf("Statistiques\n");
            break;
        case '6':
            printf("Quitter\n");
            break;
        default:
            printf("\nEntree non valide. Veuillez entrer un chiffre entre 1-6.\n");
            afficher_menu();
            break;
        }
    }
    else
    {
        printf("\nEntree non valide. Vrrrrrrrrreuillez entrer un chiffre entre 1-6.\n");
        return afficher_menu();
    }
    return choix;
}
