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
        mode = getchar();
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
            printf("Entree non valide. Veuillez entrer un chiffre entre 1-2.\n");
        }
    } while (!isdigit(mode));
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
