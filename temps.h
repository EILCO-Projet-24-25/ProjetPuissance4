#ifndef TEMPS_H_INCLUDED
#define TEMPS_H_INCLUDED

int saisir_entier_avec_gestion_temps(int timeout_ms, int *result);
void update_statistics(const char* player_name, int victory, int points);
void display_statistics();
#endif // TEMPS_H_INCLUDED
