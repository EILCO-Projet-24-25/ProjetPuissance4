#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "menu.h"

void quitterJeu()
{
    printf("Aurevoir....... :)\n");
    exit(1);
}

int continuerJeu(char *message)
{
    char choix;
    printf("%s\n", message);
    while (getchar() != '\n')
        ;
    scanf("%c", &choix);
    if (isdigit(choix))
    {
        switch (choix)
        {
        case '1':
            break;
        case '0':
            break;
        default:
            afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 et 0.\n");
            continuerJeu(message);
            break;
        }
    }
    else
    {
        afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 et 0.\n");
        return continuerJeu(message);
    }
    return choix;
}

void afficerAide()
{
    effacer_ecran();
    printf("\n=== Aide au jeu Puissance 4 ===\n");
    printf("\nObjectif :\n");
    printf("Le but du jeu est d'aligner 4 de vos pions horizontalement, verticalement ou diagonalement avant votre adversaire.\n");

    printf("\nComment jouer :\n");
    printf("1. Vous jouez a tour de role avec votre adversaire (un autre joueur ou l'ordinateur).\n");
    printf("2. a chaque tour, choisissez une colonne pour y deposer votre pion.\n");
    printf("3. Le pion tombera dans la position la plus basse disponible de cette colonne.\n");

    printf("\nNiveaux de difficulte :\n");
    printf("1. Niveau Facile :\n");
    printf("   - L'ordinateur joue aleatoirement, sans strategie particuliere.\n");
    printf("2. Niveau Moyen :\n");
    printf("   - L'ordinateur tente de bloquer vos coups gagnants.\n");
    printf("3. Niveau Difficile :\n");
    printf("   - L'ordinateur analyse la grille pour faire le meilleur choix possible et essayer de gagner a tout prix.\n");

    printf("\nConseils :\n");
    printf("- Essayez de planifier vos coups a l'avance pour creer plusieurs alignements possibles.\n");
    printf("- Bloquez les alignements adverses des que possible.\n");
    printf("- Ne negligez pas les diagonales, elles sont souvent moins visibles !\n");

    printf("\nCommandes :\n");
    printf("- Au menu principal, selectionnez les options a l'aide des chiffres (1 - 6).\n");
    printf("- Pendant la partie, entrez le numero de la colonne ou vous voulez deposer votre pion.\n");

    printf("\nAmusez-vous bien et bonne chance !\n\n");

    if (continuerJeu("Retourner sur le menu? Saisir 1 Pour Oui 0 Pour quitter le jeu....") == 49)
    {
        effacer_ecran();
        afficher_menu();
    }
    else
    {
        quitterJeu();
    }
}

int niveauDificulte()
{
    effacer_ecran();
    char choix;
    printf("\n=== Niveau de diffucultes ===\n");
    printf("1. Facile\n");
    printf("2. Intermediaire\n");
    printf("3. Difficile\n");

    printf("Entrez votre choix : \t");
    while (getchar() != '\n')
        ;
    choix = fgetc(stdin);
    printf("Choix %c\n", choix);
    if (isdigit(choix))
    {
        switch (choix)
        {
        case '1':
            return choix;
            break;
        case '2':
            return choix;
            break;
        case '3':
            return choix;
            break;
        default:
            afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 - 3.\n");
            niveauDificulte();
            break;
        }
    }
    else
    {
        afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 - 3.\n");
        return niveauDificulte();
    }
    return choix;
}
