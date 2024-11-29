#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "jeu.h"
#include "parametre.h"
#include "sauvegarde.h"

#define JOUEUR 'X'
#define ORDI 'O'
#define VIDE ' '

int main(int argc, char *argv[])
{
    char **grille = NULL;
    int lignes, colonnes, modeChoisi, difficulte;
    char *joueur1 = (char *)malloc((12) * sizeof(char));
    char *joueur2 = (char *)malloc((12) * sizeof(char));
    int choix = afficher_menu() - '0';
    // printf("Choix %d\n", choix);
    int oldChoice;
    if (choix == 0)
    {
        oldChoice = choix;
        grille = chargerJeu(&lignes, &colonnes, &difficulte, &modeChoisi, &choix, "savegarde.txt", joueur1, joueur2);
        afficherGrille(grille, lignes, colonnes);
        
        printf("%d %d %d %d %d %s %s\n", lignes, colonnes, difficulte, modeChoisi, choix, joueur1, joueur2);
        exit(1);
    }
    if (choix == 1)
    {
        // Mode contre Ordinateur / Joueur
        // if (oldChoice != 0)
        // {
        grille = definir_mode_jeu(&lignes, &colonnes, &modeChoisi, &difficulte);
        if(modeChoisi == 3)
        // printf("Mode de jeu %d\n", modeChoisi);
        saisieNomJoueur(joueur1, joueur2, modeChoisi);
        printf("Joueur 1 %s\n", joueur1);
        printf("Joueur 2 %s\n", joueur2);
        // }
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
                    afficher_menu();
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

    return 0;
}
