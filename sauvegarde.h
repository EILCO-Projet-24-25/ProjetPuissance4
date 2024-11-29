#ifndef SAUVEGARDE_H_INCLUDED
#define SAUVEGARDE_H_INCLUDED

void sauvegarderJeu(char **grille, int lignes, int colonnes, const char *nomFichier, char *joueur1, char *joueur2);
void chargerJeu(char **grille, int lignes, int colonnes, const char *nomFichier);


#endif // SAUVEGARDE_H_INCLUDED
