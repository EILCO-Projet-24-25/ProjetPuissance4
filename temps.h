#ifndef TEMPS_H_INCLUDED
#define TEMPS_H_INCLUDED

int saisir_entier_avec_gestion_temps(int timeout_ms, int *result);
void mise_a_jour_statistique(const char *player_name, int victory, int points, int duree);
void display_statistics();
#endif // TEMPS_H_INCLUDED
