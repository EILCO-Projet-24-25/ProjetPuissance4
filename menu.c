#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "jeu.h"
#include "parametre.h"
#include "sauvegarde.h"
#include "menu.h"
#include "temps.h"

int sauvegardeMenu()
{
    char mode;
    int firstTime = 0;
    do
    {
        effacer_ecran();
        printf("\n=== Options ===\n");
        printf("1. Quitter et sauvegarder\n");
        printf("2. Quitter sans sauvegarder\n");
        printf("3. Annuler\n");
        printf("Entrez votre choix : \t");
        // Nettoyer le tampon avant de saisir un choix
        if (firstTime == 0)
        {
            while (getchar() != '\n')
                ;
        }
        firstTime = 1;
        mode = fgetc(stdin);
        if (!isdigit(mode))
        {
            afficher_erreur("Entree non valide. Veuillez entrer un chiffre entre 1-3.");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            getchar();
            effacer_ecran();
            continue;
        }
        if (mode != '1' && mode != '2' && mode != '3')
        {
            afficher_erreur("Entree non valide. Veuillezjjjjj entrer un chiffre entre 1-3.");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            getchar();
            effacer_ecran();
        }
        else
        {
            break;
        }
    } while (1);
    return mode - '0';
}

int recommencerPartie(char **grille, int lignes, char *joueur1, char *joueur2)
{
    char mode;
    int firstTime = 0;
    do
    {
        // effacer_ecran();
        printf("\n=== Options ===\n");
        printf("1. Recommencer la partie\n");
        printf("2. Retourner au menu\n");
        printf("Entrez votre choix : \t");
        // Nettoyer le tampon avant de saisir un choix
        if (firstTime == 0)
        {
            while (getchar() != '\n')
                ;
        }
        firstTime = 1;
        mode = fgetc(stdin);
        if (!isdigit(mode))
        {
            afficher_erreur("Entree non valide. Veuillez entrer un chiffre entre 1-2.");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            getchar();
            effacer_ecran();
            continue;
        }
        if (mode != '1' && mode != '2')
        {
            afficher_erreur("Entree non valide. Veuillezjjjjj entrer un chiffre entre 1-2.");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            getchar();
            effacer_ecran();
        }
        else
        {
            break;
        }
    } while (1);
    if (mode - '0' == 2)
    {
        effacer_ecran();
        libererGrille(grille, lignes);
        free(joueur1);
        free(joueur2);
        lancer_jeu();
    }
    return mode - '0';
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
        printf("2. Joueur contre Joueur taille standard (6x7) 5sec/joueur\n");
        printf("3. Retourner au Menu Principal\n");
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
                if (*difficulte == 4)
                {
                    effacer_ecran();
                    lancer_jeu();
                    break;
                }
                printf("Mode selectionne : Jouer contre le Bot\n");
                // printf("ligne de la grille: \t");
                // scanf("%d", lignes);
                // printf("\ncolone de la grille: \t");
                // scanf("%d", colonnes);
                // Setup de la grille
                if (*difficulte == 1)
                {
                    *lignes = 6;
                    *colonnes = 7;
                }
                else if (*difficulte == 2)
                {
                    *lignes = 9;
                    *colonnes = 10;
                }
                else if (*difficulte == 3)
                {
                    *lignes = 14;
                    *colonnes = 15;
                }
                grille = creationGrille(*lignes, *colonnes);
                break;
            }
            else if (mode == '2')
            {
                printf("Mode selectionne : Joueur contre Joueur\n");
                // printf("ligne de la grille: \t");
                // scanf("%d", lignes);
                // printf("\ncolone de la grille: \t");
                // scanf("%d", colonnes);
                *lignes = 6;
                *colonnes = 7;
                grille = creationGrille(*lignes, *colonnes);
                break;
            }
            else if (mode == '3')
            {
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

    } while (mode != '1' || mode != '2' || mode != '3');
    return grille;
}

char afficher_menu()
{
    effacer_ecran();
    char choix;
    do
    {
        // int c;
        // while ((c = fgetc(stdin)) != '\n' && c != EOF)
        //     ;
        printf("\n=== Menu Principal ===\n");
        if (verifierFichier("savegarde.txt") == 1)
            printf("0. Pour continuer votre derniere partie\n");
        printf("1. Choisir le Mode de Jeu (vs Bot ou vs Joueur)\n");
        printf("2. Revoir une partie\n");
        // printf("3. Parametres \n");
        printf("3. Aide\n");
        printf("4. Statistiques\n");
        printf("5. Quitter\n");

        printf("Entrez votre choix : \t");
        scanf("%c", &choix);
        if (verifierFichier("savegarde.txt") == 1 && choix == '0')
        {
            return choix;
            break;
        }
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
            // case '3':
            //     printf("Parametres\n");
            //     break;
            case '3':
                printf("Aide\n");
                afficerAide();
                break;
            case '4':
                printf("Statistiques\n");
                 display_statistics();
                break;
            case '5':
                printf("Quitter le jeu\n");
                quitterJeu();
                break;
            default:
                afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1-6.\n");
                // printf("\nAppuyez sur la touche entree pour reessayer...\n");
                // while (getchar() != '\n')
                //     ;
                // getchar();
                afficher_menu();
                break;
            }
        }
        else
        {
            afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1-6.\n");
            // printf("\nAppuyez sur la touche entree pour reessayer...\n");
            // while (getchar() != '\n')
            //     ;
            // getchar();
            return afficher_menu();
        }
    } while (choix < '1' || choix > '6');
    return choix;
}
