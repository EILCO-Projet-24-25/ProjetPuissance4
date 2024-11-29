#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "jeu.h"
#include "parametre.h"

void effacer_ecran()
{
    printf("Effacer Ecran\n");
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void afficher_erreur(const char *message)
{
    printf("\033[1;31m"); // Code ANSI pour texte rouge
    printf("%s\n", message);
    printf("\033[0m"); // Reinitialisation de la couleur
}

char **definir_mode_jeu(int *lignes, int *colonnes, int *modeChoisi, int *difficulte)
{
    char mode;
    char **grille = NULL;
    int firstTime = 0;
    do
    {
        effacer_ecran();
        printf("\n=== Choisir le Mode de Jeu ===\n");
        printf("1. Jouer contre le Bot\n");
        printf("2. Joueur contre Joueur\n");
        printf("Entrez votre choix : \t");
        // Nettoyer le tampon avant de saisir un choix
        if (firstTime == 0)
        {
            while (getchar() != '\n')
                ;
        }
        firstTime = 1;
        // scanf("%c", &mode);
        mode = fgetc(stdin);
        *modeChoisi = mode - '0';
        if (isdigit(mode))
        {
            if (mode == '1')
            {
                *difficulte = niveauDificulte();
                printf("Mode selectionne : Jouer contre le Bot\n");
                printf("ligne de la grille: \t");
                scanf("%d", lignes);
                printf("\ncolone de la grille: \t");
                scanf("%d", colonnes);
                grille = creationGrille(*lignes, *colonnes);
                break;
            }
            else if (mode == '2')
            {
                printf("Mode selectionne : Joueur contre Joueur\n");
                printf("ligne de la grille: \t");
                scanf("%d", lignes);
                printf("\ncolone de la grille: \t");
                scanf("%d", colonnes);
                grille = creationGrille(*lignes, *colonnes);
                break;
            }
            else
            {
                afficher_erreur("Entree non valide. Veuillez entrer un chiffre entre 1-2.");
                printf("\nAppuyez sur la touche entree pour reessayer...\n");
                while (getchar() != '\n')
                    ;
                getchar();
                effacer_ecran();
            }
        }
        else
        {
            afficher_erreur("Entree non valide. Veuillez entrer un chiffre entre 1-2.");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            getchar();
            effacer_ecran();
        }

    } while (mode != '1' || mode != '2');
    return grille;
}

char afficher_menu()
{
    // effacer_ecran();
    char choix;
    do
    {
        vider_tampon();
        effacer_ecran();
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
                afficerAide();
                break;
            case '5':
                printf("Statistiques\n");
                break;
            case '6':
                quitterJeu();
                break;
            default:
                afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1-6.\n");
                printf("\nAppuyez sur la touche entree pour reessayer...\n");
                while (getchar() != '\n')
                    ;
                getchar();
                afficher_menu();
                break;
            }
        }
        else
        {
            afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1-6.\n");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            getchar();
            return afficher_menu();
        }
    } while (choix < '1' || choix > '6');
    return choix;
}
