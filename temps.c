#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

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
void init_random() {
    srand((unsigned)time(NULL));
}

#ifndef _WIN32
// Fonction pour configurer le terminal en mode non bloquant (sous Linux/Unix)
void set_terminal_mode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        // Sauvegarder les paramètres actuels du terminal
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        // Désactiver le mode canonique et l'écho
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        // Mettre stdin en mode non-bloquant
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        // Restaurer les paramètres du terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}
#endif

// Fonction pour valider que la chaîne saisie représente un entier valide
int validation_saisie(const char *input, int *result) {
    if (input == NULL || *input == '\0') {
        return 0; // Entrée vide
    }

    char *endptr;
    long val = strtol(input, &endptr, 10);

    // Vérifier s'il y a des caractères non numériques
    if (*endptr != '\0') {
        return 0; // Entrée invalide
    }

    // Vérifier que la valeur est dans les limites d'un int
    if (val < INT_MIN || val > INT_MAX) {
        return 0; // Dépassement de capacité
    }

    *result = (int)val;
    return 1; // Entrée valide
}

#ifdef _WIN32
// Fonction pour lire un entier avec un timeout sous Windows
// Retourne 1 si un entier valide est saisi, 0 en cas de timeout ou d'erreur
int saisir_entier_avec_gestion_temps(int timeout_ms, int *result) {
    int elapsed = 0;
    const int step = 100; // Intervalle de vérification en ms
    char buffer[20];
    int index = 0;

    while (elapsed < timeout_ms) {
        if (_kbhit()) {
            int c = _getch();
            if (c == '\r') { // Touche Entrée
                buffer[index] = '\0';
                if (validation_saisie(buffer, result)) {
                    return 1; // Saisie valide
                } else {
                    printf("\nEntrée invalide. Veuillez saisir un entier valide.\n");
                    return 0; // Saisie invalide
                }
            } else if (c == '\b') { // Touche Backspace
                if (index > 0) {
                    index--;
                    printf("\b \b"); // Supprimer le caractère affiché
                }
            }
            // Acceptation des signes '+' et '-' en première position
            else if ((c == '-' || c == '+') && index == 0 && index < (int)(sizeof(buffer) - 1)) {
                buffer[index++] = c;
                printf("%c", c);
            }
            // Acceptation des chiffres
            else if (isdigit(c) && index < (int)(sizeof(buffer) - 1)) {
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
int saisir_entier_avec_gestion_temps(int timeout_ms, int *result) {
    set_terminal_mode(1); // Activer le mode non bloquant
    struct timeval start, current;
    gettimeofday(&start, NULL);
    int time_left = timeout_ms;

    char buffer[20];
    int index = 0;

    while (time_left > 0) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval tv;
        tv.tv_sec = time_left / 1000;
        tv.tv_usec = (time_left % 1000) * 1000;

        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            char c = getchar();
            if (c == '\n') { // Touche Entrée
                buffer[index] = '\0';
                set_terminal_mode(0); // Restaurer le terminal
                if (validation_saisie(buffer, result)) {
                    return 1; // Saisie valide
                } else {
                    printf("\nEntrée invalide. Veuillez saisir un entier valide.\n");
                    return 0; // Saisie invalide
                }
            } else if (c == 127 || c == '\b') { // Touche Backspace
                if (index > 0) {
                    index--;
                    printf("\b \b"); // Supprimer le caractère affiché
                }
            }
            // Acceptation des signes '+' et '-' en première position
            else if ((c == '-' || c == '+') && index == 0 && index < (int)(sizeof(buffer) - 1)) {
                buffer[index++] = c;
                printf("%c", c);
            }
            // Acceptation des chiffres
            else if (isdigit(c) && index < (int)(sizeof(buffer) - 1)) {
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
