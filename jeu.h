#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

char **creationGrille(int lignes, int colonnes);
void libererGrille(char **grille, int lignes);
void afficherGrille(char **grille, int lignes, int colonnes);
char choisirPion(char defaultPion);
int deposerPignon(char **grille, int colone, int colonnes, int lignes, char forme);
int verifier_alignement(char **grille, int lignes, int colonnes, int ligne, int col, char joueur);
int verifier_victoire(char **grille, int lignes, int colonnes, char joueur);
int jouer_ordinateur(char **grille, int lignes, int colonnes, int niveau, char pion1, char pion2);
int jouer_tour_joueur(char **grille, int lignes, int colonnes, char pion, char *nomJoueur);
int random_1_2();
void lancer_jeu();
void partie_ordi(char **grille, int lignes, int colonnes, char *joueur1, char *joueur2, int difficulte, int modeChoisi, int choix, char pion1, char pion2);



//Statistiques 
typedef struct {
    char nom[50];
    int victoires;
    int defaites;
    int partiesJouees;
} StatistiqueJoueur;

#endif // JEU_H_INCLUDED
