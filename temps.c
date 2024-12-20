#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "parametre.h"
#include "jeu.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

// Fonction pour initialiser le générateur de nombres aléatoires
void init_random()
{
    srand((unsigned)time(NULL));
}

#ifndef _WIN32
// Fonction pour configurer le terminal en mode non bloquant (sous Linux/Unix)
void set_terminal_mode(int enable)
{
    static struct termios oldt, newt;
    if (enable)
    {
        // Sauvegarder les paramètres actuels du terminal
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        // Désactiver le mode canonique et l'écho
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        // Mettre stdin en mode non-bloquant
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    }
    else
    {
        // Restaurer les paramètres du terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}
#endif

// Fonction pour valider que la chaîne saisie représente un entier valide
int validation_saisie(const char *input, int *result)
{
    if (input == NULL || *input == '\0')
    {
        return 0; // Entrée vide
    }

    char *endptr;
    long val = strtol(input, &endptr, 10);

    // Vérifier s'il y a des caractères non numériques
    if (*endptr != '\0')
    {
        return 0; // Entrée invalide
    }

    // Vérifier que la valeur est dans les limites d'un int
    if (val < INT_MIN || val > INT_MAX)
    {
        return 0; // Dépassement de capacité
    }

    *result = (int)val;
    return 1; // Entrée valide
}

#ifdef _WIN32
// Fonction pour lire un entier avec un timeout sous Windows
// Retourne 1 si un entier valide est saisi, 0 en cas de timeout ou d'erreur
int saisir_entier_avec_gestion_temps(int timeout_ms, int *result)
{
    int elapsed = 0;
    const int step = 100; // Intervalle de vérification en ms
    char buffer[20];
    int index = 0;

    while (elapsed < timeout_ms)
    {
        if (_kbhit())
        {
            int c = _getch();
            if (c == '\r')
            { // Touche Entrée
                buffer[index] = '\0';
                if (validation_saisie(buffer, result))
                {
                    return 1; // Saisie valide
                }
                else
                {
                    printf("\nEntrée invalide. Veuillez saisir un entier valide.\n");
                    return 0; // Saisie invalide
                }
            }
            else if (c == '\b')
            { // Touche Backspace
                if (index > 0)
                {
                    index--;
                    printf("\b \b"); // Supprimer le caractère affiché
                }
            }
            // Acceptation des signes '+' et '-' en première position
            else if ((c == '-' || c == '+') && index == 0 && index < (int)(sizeof(buffer) - 1))
            {
                buffer[index++] = c;
                printf("%c", c);
            }
            // Acceptation des chiffres
            else if (isdigit(c) && index < (int)(sizeof(buffer) - 1))
            {
                buffer[index++] = c;
                printf("%c", c);
            }
        }
        Sleep(step);
        elapsed += step;
    }
    printf("\nTemps écoulé !\n");
    return 0; // Timeout
}
#else
// Fonction pour lire un entier avec un timeout sous Linux/Unix
// Retourne 1 si un entier valide est saisi, 0 en cas de timeout ou d'erreur
int saisir_entier_avec_gestion_temps(int timeout_ms, int *result)
{
    set_terminal_mode(1); // Activer le mode non bloquant
    struct timeval start, current;
    gettimeofday(&start, NULL);
    int time_left = timeout_ms;

    char buffer[20];
    int index = 0;

    while (time_left > 0)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval tv;
        tv.tv_sec = time_left / 1000;
        tv.tv_usec = (time_left % 1000) * 1000;

        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds))
        {
            char c = getchar();
            if (c == '\n')
            { // Touche Entrée
                buffer[index] = '\0';
                set_terminal_mode(0); // Restaurer le terminal
                if (validation_saisie(buffer, result))
                {
                    return 1; // Saisie valide
                }
                else
                {
                    printf("\nEntrée invalide. Veuillez saisir un entier valide.\n");
                    return 0; // Saisie invalide
                }
            }
            else if (c == 127 || c == '\b')
            { // Touche Backspace
                if (index > 0)
                {
                    index--;
                    printf("\b \b"); // Supprimer le caractère affiché
                }
            }
            // Acceptation des signes '+' et '-' en première position
            else if ((c == '-' || c == '+') && index == 0 && index < (int)(sizeof(buffer) - 1))
            {
                buffer[index++] = c;
                printf("%c", c);
            }
            // Acceptation des chiffres
            else if (isdigit(c) && index < (int)(sizeof(buffer) - 1))
            {
                buffer[index++] = c;
                printf("%c", c);
            }
        }

        // Calculer le temps écoulé
        gettimeofday(&current, NULL);
        int elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec - start.tv_usec) / 1000;
        time_left = timeout_ms - elapsed;
    }

    set_terminal_mode(0); // Restaurer le terminal
    printf("\nTemps écoulé !\n");
    return 0; // Timeout
}
#endif

void update_statistics(char *player_name, int victory, int points)
{
    // printf("Début de la mise à jour des statistiques pour %s\n", player_name);
    fflush(stdout); // S'assurer que la sortie est affichée immédiatement

    // Vérifier les paramètres
    if (player_name == NULL)
    {
        fprintf(stderr, "Erreur : Le nom du joueur ne peut pas être NULL.\n");
        return;
    }
    if (victory != 0 && victory != 1)
    {
        fprintf(stderr, "Erreur : Le paramètre 'victory' doit être 0 ou 1.\n");
        return;
    }

    // Copier et nettoyer le nom du joueur
    char nom_joueur[100];
    strncpy(nom_joueur, player_name, sizeof(nom_joueur) - 1);
    nom_joueur[sizeof(nom_joueur) - 1] = '\0';

    // Supprimer les espaces en début
    char *start = nom_joueur;
    while (isspace((unsigned char)*start))
        start++;
    memmove(nom_joueur, start, strlen(start) + 1);

    // Supprimer les espaces en fin
    char *end = nom_joueur + strlen(nom_joueur) - 1;
    while (end > nom_joueur && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';

    // Remplacer les espaces internes par des underscores
    for (int i = 0; nom_joueur[i] != '\0'; i++)
    {
        if (isspace((unsigned char)nom_joueur[i]))
        {
            nom_joueur[i] = '_';
        }
    }
    // printf("Nom du joueur nettoyé : %s\n", nom_joueur);
    fflush(stdout);

    // Ouvrir le fichier de statistiques pour lecture
    FILE *fp_read = fopen("statistique.txt", "r");
    FILE *fp_write = fopen("temp.txt", "w");

    if (fp_write == NULL)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier temporaire pour l'écriture.\n");
        if (fp_read != NULL)
            fclose(fp_read);
        return;
    }
    else
    {
        // printf("Fichier temporaire ouvert avec succès.\n");
        fflush(stdout);
    }

    int joueur_trouve = 0;
    char ligne[256];

    if (fp_read != NULL)
    {
        // printf("Fichier statistique.txt ouvert pour lecture.\n");
        fflush(stdout);
        while (fgets(ligne, sizeof(ligne), fp_read))
        {
            // Variables pour stocker les données de chaque ligne
            char nom[100];
            int victoires = 0, defaites = 0, total_points = 0;

            // Parser la ligne
            // Format attendu : NomJoueur,Victoires,Defaites,Points
            char *token = strtok(ligne, ",");
            if (token != NULL)
            {
                strncpy(nom, token, sizeof(nom) - 1);
                nom[sizeof(nom) - 1] = '\0';

                // Nettoyer le nom pour comparaison
                char *start_nom = nom;
                while (isspace((unsigned char)*start_nom))
                    start_nom++;
                memmove(nom, start_nom, strlen(start_nom) + 1);

                // Supprimer les espaces en fin
                char *end_nom = nom + strlen(nom) - 1;
                while (end_nom > nom && isspace((unsigned char)*end_nom))
                    end_nom--;
                *(end_nom + 1) = '\0';

                // Remplacer les espaces internes par des underscores
                for (int i = 0; nom[i] != '\0'; i++)
                {
                    if (isspace((unsigned char)nom[i]))
                    {
                        nom[i] = '_';
                    }
                }

                // Lire les victoires
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    victoires = atoi(token);
                }

                // Lire les défaites
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    defaites = atoi(token);
                }

                // Lire les points
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    total_points = atoi(token);
                }

                // printf("Lecture ligne : %s, Victoires: %d, Defaites: %d, Points: %d\n", nom, victoires, defaites, total_points);
                fflush(stdout);

                // Comparer avec le nom du joueur à mettre à jour
                if (strcmp(nom, nom_joueur) == 0)
                {
                    // printf("Joueur trouvé : %s\n", nom_joueur);
                    fflush(stdout);
                    // Mettre à jour les statistiques
                    if (victory)
                    {
                        victoires += 1;
                    }
                    else
                    {
                        defaites += 1;
                    }
                    total_points += points;
                    joueur_trouve = 1;

                    // Écrire la ligne mise à jour dans le fichier temporaire
                    fprintf(fp_write, "%s,%d,%d,%d\n", nom, victoires, defaites, total_points);
                    // printf("Statistiques mises à jour pour %s.\n", nom);
                    fflush(stdout);
                }
                else
                {
                    // Écrire la ligne inchangée dans le fichier temporaire
                    fprintf(fp_write, "%s,%d,%d,%d\n", nom, victoires, defaites, total_points);
                }
            }
        }
        fclose(fp_read);
    }
    else
    {
        // printf("Fichier statistique.txt non trouvé. Un nouveau fichier sera créé.\n");
        fflush(stdout);
    }

    if (!joueur_trouve)
    {
        // Ajouter le joueur avec les statistiques appropriées
        fprintf(fp_write, "%s,%d,%d,%d\n", nom_joueur, victory ? 1 : 0, victory ? 0 : 1, points);
        // printf("Nouvel enregistrement pour le joueur %s ajouté.\n", nom_joueur);
        fflush(stdout);
    }

    fclose(fp_write);

    // Remplacer le fichier original par le fichier temporaire
    if (remove("statistique.txt") != 0)
    {
        if (errno != ENOENT)
        {
            perror("Erreur : Impossible de supprimer le fichier original des statistiques");
            return;
        }
        // Si errno == ENOENT, le fichier n'existe pas, ce qui est acceptable
    }

    if (rename("temp.txt", "statistique.txt") != 0)
    {
        perror("Erreur : Impossible de renommer le fichier temporaire en fichier des statistiques");
        return;
    }
    else
    {
        // printf("Fichier temporaire renommé en statistique.txt avec succès.\n");
        fflush(stdout);
    }

    // if (joueur_trouve)
    // {
    //     // printf("Statistiques mises à jour pour le joueur %s.\n", nom_joueur);
    // }
    // else
    // {
    //     // printf("Nouvel enregistrement pour le joueur %s ajouté.\n", nom_joueur);
    // }

    // printf("Fin de la mise à jour des statistiques pour %s\n", player_name);
    fflush(stdout);
}

void display_statistics() {
    printf("=== Affichage des Statistiques des Joueurs ===\n");
    fflush(stdout);

    FILE *fp_read = fopen("statistique.txt", "r");
    if (fp_read == NULL) {
        printf("Aucune statistique trouvée. Le fichier 'statistique.txt' n'existe pas.\n");
        return;
    }

    // Tableaux parallèles pour stocker les données
    int capacity = 10; // Capacité initiale
    int count = 0;
    char **names = malloc(capacity * sizeof(char *));
    int *victories = malloc(capacity * sizeof(int));
    int *defeats = malloc(capacity * sizeof(int));
    int *points = malloc(capacity * sizeof(int));

    if (names == NULL || victories == NULL || defeats == NULL || points == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour les données.\n");
        if (names) free(names);
        if (victories) free(victories);
        if (defeats) free(defeats);
        if (points) free(points);
        fclose(fp_read);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fp_read)) {
        char nom[100];
        int victoires_temp, defaites_temp, pts;

        // Parser la ligne
        char *token = strtok(ligne, ",");
        if (token == NULL) continue;
        strncpy(nom, token, sizeof(nom) - 1);
        nom[sizeof(nom) - 1] = '\0';

        // Lire les victoires
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        victoires_temp = atoi(token);

        // Lire les défaites
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        defaites_temp = atoi(token);

        // Lire les points
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        pts = atoi(token);

        // Ajouter au tableau
        if (count >= capacity) {
            capacity *= 2;
            char **temp_names = realloc(names, capacity * sizeof(char *));
            int *temp_victories = realloc(victories, capacity * sizeof(int));
            int *temp_defeats = realloc(defeats, capacity * sizeof(int));
            int *temp_points = realloc(points, capacity * sizeof(int));
            if (temp_names == NULL || temp_victories == NULL || temp_defeats == NULL || temp_points == NULL) {
                fprintf(stderr, "Erreur : Impossible de réallouer la mémoire pour les données.\n");
                // Libérer tout
                for (int i = 0; i < count; i++) {
                    free(names[i]);
                }
                free(names);
                free(victories);
                free(defeats);
                free(points);
                fclose(fp_read);
                return;
            }
            names = temp_names;
            victories = temp_victories;
            defeats = temp_defeats;
            points = temp_points;
        }

        // Remplacer les underscores par des espaces pour l'affichage
        for (int i = 0; nom[i] != '\0'; i++) {
            if (nom[i] == '_') {
                nom[i] = ' ';
            }
        }

        // Allouer et copier le nom
        names[count] = malloc(strlen(nom) + 1);
        if (names[count] == NULL) {
            fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour le nom.\n");
            // Libérer tout
            for (int i = 0; i < count; i++) {
                free(names[i]);
            }
            free(names);
            free(victories);
            free(defeats);
            free(points);
            fclose(fp_read);
            return;
        }
        strcpy(names[count], nom);
        victories[count] = victoires_temp;
        defeats[count] = defaites_temp;
        points[count] = pts;
        count++;
    }

    fclose(fp_read);

    if (count == 0) {
        printf("Aucune statistique trouvée dans le fichier 'statistique.txt'.\n");
        free(names);
        free(victories);
        free(defeats);
        free(points);
        return;
    }

    // Trier les joueurs par points décroissants (Bubble sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (points[j] < points[j + 1]) {
                // Échanger points
                int temp_pt = points[j];
                points[j] = points[j + 1];
                points[j + 1] = temp_pt;

                // Échanger victoires
                int temp_vic = victories[j];
                victories[j] = victories[j + 1];
                victories[j + 1] = temp_vic;

                // Échanger défaites
                int temp_def = defeats[j];
                defeats[j] = defeats[j + 1];
                defeats[j + 1] = temp_def;

                // Échanger noms
                char *temp_nom = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp_nom;
            }
        }
    }

    // Afficher le classement
    printf("%-5s %-25s %-10s %-10s %-10s\n", "Rang", "Nom du Joueur", "Victoires", "Defaites", "Points");
    printf("------------------------------------------------------------------\n");

    int rank = 1;
    for (int i = 0; i < count; i++) {
        if (i > 0 && points[i] < points[i - 1]) {
            rank = i + 1;
        }
        printf("%-5d %-25s %-10d %-10d %-10d\n", rank, names[i], victories[i], defeats[i], points[i]);
    }

    // Libérer la mémoire
    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);
    free(victories);
    free(defeats);
    free(points);

    fflush(stdout);


    if (continuerJeu("Retourner sur le menu? Saisir 1 Pour Oui 0 Pour quitter le jeu....") == 49)
    {
        effacer_ecran();
        lancer_jeu();
    }
    else
    {
        quitterJeu();
    }
}