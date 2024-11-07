#ifndef PUISSANCE_H_INCLUDED
#define PUISSANCE_H_INCLUDED


extern char **grid; // Grille de jeu
extern int rows, cols; // Dimensions de la grille choisies par le joueur

void initialisionGrille(int rows, int cols); // Initialise la grille
void affichageGrille();                      // Affiche la grille
int deposerPignon(int col, char disc);       // Dépose un jeton dans une colonne
int checkWin(char disc);                     // Vérifie si un joueur a gagné
void libererGrille();                        // Libère la mémoire allouée pour la grille
int choisirModeJeu(); // mode une personne ou mode deux personnes
char choisirPion(char defaultPion); //choisir son style de pion



#endif // PUISSANCE_H_INCLUDED
