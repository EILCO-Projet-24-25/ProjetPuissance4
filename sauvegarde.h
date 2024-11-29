#ifndef SAUVEGARDE_H_INCLUDED
#define SAUVEGARDE_H_INCLUDED

int verifierFichier(const char *nomFichier);
void sauvegarderJeu(char **grille, int lignes, int colonnes, const char *nomFichier, char *joueur1, char *joueur2, int difficultes, int modejeu, int choixjeu);
char **chargerJeu(int *lignes, int *colonnes, int *difficultes, int *modejeu, int *choixjeu, const char *nomFichier, char *joueur1, char *joueur2);

#endif // SAUVEGARDE_H_INCLUDED
