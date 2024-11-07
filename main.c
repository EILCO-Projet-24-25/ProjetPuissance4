#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "jeu.h"

int main(int argc, char *argv[])
{
    char **grille = NULL;
    int lignes, colones, modeChoisi;
    char choix = afficher_menu();
    if (choix == 49)
    {
        grille = definir_mode_jeu(&lignes, &colones, &modeChoisi);
        afficherGrille(grille, lignes, colones);
        printf("Mode choisi %d",modeChoisi);
    }
    return 0;
}
