#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

void afficher_erreur();
void afficher_succes(const char *message);
void effacer_ecran();
char **definir_mode_jeu(int *lignes, int *colonnes, int *mode, int *difficulte);
char afficher_menu();
int sauvegardeMenu();

#endif // MENU_H_INCLUDED
