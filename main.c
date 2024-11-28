#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "jeu.h"
#include "parametre.h"

#define JOUEUR 'X'
#define ORDI 'O'
#define VIDE ' '

int main(int argc, char *argv[])
{
    char **grille = NULL;
    int lignes, colones, modeChoisi, difficulte;
    char choix = afficher_menu();
    if (choix == 49)
    {
        grille = definir_mode_jeu(&lignes, &colones, &modeChoisi, &difficulte);
        printf("Mode choisi %d\n", modeChoisi);
        printf("Difficulte choisi %d\n", difficulte);
        if (modeChoisi == 1)
        {
            int jeu_en_cours = 1;
            while (jeu_en_cours)
            {
                effacer_ecran();
                printf("Mode choisi %d\n", modeChoisi);
                printf("Difficulte choisi %c\n", difficulte);
                afficherGrille(grille, lignes, colones);
                // Tour du joueur humain
                jouer_tour_joueur(grille, lignes, colones);
                afficherGrille(grille, lignes, colones);

                int victoire_joueur = verifier_victoire(grille, lignes, colones, 'X');
                if (victoire_joueur == 1)
                {
                    printf("Le joueur a gagne\n");
                    jeu_en_cours = 0;
                    break;
                }
                // Tour de l'ordinateur
                printf("Tour de l'ordinateur...\n");
                int colonne_ordi = jouer_ordinateur(grille, lignes, colones, 2);
                printf("L'ordinateur a joué en colonne %d\n", colonne_ordi + 1);
                afficherGrille(grille, lignes, colones);

                // Condition de victoire ou de grille pleine ici...
                int victoire_ordi = verifier_victoire(grille, lignes, colones, 'O');
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
    return 0;
}
