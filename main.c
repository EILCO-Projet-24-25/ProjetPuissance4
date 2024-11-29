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
    char choix = afficher_menu();
    if (choix == 49)
    {
        // Mode contre Ordinateur / Joueur
        // Utilisation du code ASCII pour les choix 49 Correspond au choix 1
        grille = definir_mode_jeu(&lignes, &colonnes, &modeChoisi, &difficulte);
        // printf("Mode de jeu %d\n", modeChoisi);
        saisieNomJoueur(joueur1, joueur2, modeChoisi);
        printf("Joueur 1 %s\n", joueur1);
        printf("Joueur 2 %s\n", joueur2);
        // free(joueur1);
        // free(joueur2);
        // free(grille);
        // exit(1);
        if (modeChoisi == 1)
        {
            int jeu_en_cours = 1;
            while (jeu_en_cours)
            {
                effacer_ecran();
                afficherGrille(grille, lignes, colonnes);

                // Tour du joueur humain
                int functionReturn = jouer_tour_joueur(grille, lignes, colonnes);
                if (functionReturn == 1)
                {
                    sauvegarderJeu(grille, lignes, colonnes, "savegarde.txt", joueur1, joueur2);
                    effacer_ecran();
                    free(grille);
                    free(joueur1);
                    free(joueur2);
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
                int colonne_ordi = jouer_ordinateur(grille, lignes, colonnes, 2);
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
