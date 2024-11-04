#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void effacer_ecran()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void definir_mode_jeu()
{
    effacer_ecran();
    char mode;
    printf("\n=== Choisir le Mode de Jeu ===\n");
    printf("1. Jouer contre le Bot\n");
    printf("2. Joueur contre Joueur\n");
    printf("Entrez votre choix : \t");
    mode = getchar();
    if (isdigit(mode))
    {
        if (mode == '1')
        {
            printf("Mode selectionne : Jouer contre le Bot\n");
        }
        else if (mode == '2')
        {
            printf("Mode selectionne : Joueur contre Joueur\n");
        }
        else
        {
            printf("Entree non valide. Veuillez entrer un chiffre entre 1-2.\n");
            definir_mode_jeu();
        }
    }
    else
    {
        printf("Entree non valide. Veuillez entrer un chiffre entre 1-2.\n");
        definir_mode_jeu();
    }
}

void afficher_menu()
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
    choix = fgetc(stdin);
    printf("%c", choix);
    if (isdigit(choix))
    {
        switch (choix)
        {
        case '1':
            definir_mode_jeu();
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
        afficher_menu();
    }
}
