#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

char **definir_mode_jeu(int *lignes, int *colonnes, int *mode, int *difficulte);
char afficher_menu();
int sauvegardeMenu();
int recommencerPartie(char **grille, int lignes, char *joueur1, char *joueur2);

#endif // MENU_H_INCLUDED
