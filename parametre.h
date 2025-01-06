#ifndef PARAMETRE_H_INCLUDED
#define PARAMETRE_H_INCLUDED

void afficher_erreur(const char *message);
void afficher_succes(const char *message);
void effacer_ecran();
void quitterJeu();
int continuerJeu(char *message, int test);
void afficerAide();
int niveauDificulte();
void vider_tampon();
void saisieNomJoueur(char *joueur1, char *joueur2, int choix);
int replayPartie(const char *nomFichier);
#endif // PARAMETRE_H_INCLUDED
