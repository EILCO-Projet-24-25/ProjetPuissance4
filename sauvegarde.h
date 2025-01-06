#ifndef SAUVEGARDE_H_INCLUDED
#define SAUVEGARDE_H_INCLUDED

int verifierFichier(const char *nomFichier);
void sauvegarderJeu(char **grille, int lignes, int colonnes, const char *nomFichier, char *joueur1, char *joueur2, int difficultes, int modejeu, int choixjeu, char pion1, char pion2, char *nomPartie);
char **chargerJeu(int *lignes, int *colonnes, int *difficultes, int *modejeu, int *choixjeu, const char *nomFichier, char *joueur1, char *joueur2, char *pion1, char *pion2, char *nomPartie);
char *creerPartie(const char *joueur1, const char *joueur2);
int sauvegarde(const char *nomFichierGrille, char **grille, int lignes, int colonnes, char *joueur1, char *joueur2, char pionjoueur1, char pionjoueur2);
#endif // SAUVEGARDE_H_INCLUDED
