#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

char **creationGrille(int lignes, int colonnes);
void libererGrille(char **grille, int lignes);
void afficherGrille(char **grille, int lignes, int colonnes);
int deposerPignon(char **grille, int colone, int colones, int lignes, char forme);
int verifier_alignement(char **grille, int lignes, int colonnes, int ligne, int col, char joueur);
int verifier_victoire(char **grille, int lignes, int colonnes, char joueur);
int evaluer_grille(char **grille, int lignes, int colonnes);
int jouer_ordinateur(char **grille, int lignes, int colonnes, int niveau);
void jouer_tour_joueur(char **grille, int lignes, int colones);
int chercher_victoire(char **grille, int lignes, int colonnes, char joueur);

#endif // JEU_H_INCLUDED
