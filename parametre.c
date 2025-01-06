#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include "menu.h"
#include "jeu.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <unistd.h>
#endif

void quitterJeu()
{
    printf("Aurevoir....... :)\n");
    exit(1);
}

void effacer_ecran()
{
    printf("Effacer Ecran\n");
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void afficher_erreur(const char *message)
{
    printf("\033[1;31m"); // Code ANSI pour texte rouge
    printf("%s\n", message);
    printf("\033[0m"); // Reinitialisation de la couleur
}

void afficher_succes(const char *message)
{
    printf("\033[1;32m"); // Code ANSI pour texte vert
    printf("%s\n", message);
    printf("\033[0m"); // Reinitialisation de la couleur
}

int continuerJeu(char *message)
{
    char choix;
    printf("%s\n", message);
    while (getchar() != '\n')
        ;
    scanf("%c", &choix);
    if (isdigit(choix))
    {
        switch (choix)
        {
        case '1':
            break;
        case '0':
            break;
        default:
            afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 et 0.\n");
            continuerJeu(message);
            break;
        }
    }
    else
    {
        afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 et 0.\n");
        return continuerJeu(message);
    }
    return choix;
}

void afficerAide()
{
    effacer_ecran();
    printf("\n=== Aide au jeu Puissance 4 ===\n");
    printf("\nObjectif :\n");
    printf("Le but du jeu est d'aligner 4 de vos pions horizontalement, verticalement ou diagonalement avant votre adversaire.\n");

    printf("\nComment jouer :\n");
    printf("1. Vous jouez a tour de role avec votre adversaire (un autre joueur ou l'ordinateur).\n");
    printf("2. a chaque tour, choisissez une colonne pour y deposer votre pion.\n");
    printf("3. Le pion tombera dans la position la plus basse disponible de cette colonne.\n");

    printf("\nNiveaux de difficulte :\n");
    printf("1. Niveau Facile :\n");
    printf("   - L'ordinateur joue aleatoirement, sans strategie particuliere. Taille (6x7) temps de jeu 10 sec/tour.\n");
    printf("2. Niveau Moyen :\n");
    printf("   - L'ordinateur tente de bloquer vos coups gagnants Taille .\n");
    printf("3. Niveau Difficile :\n");
    printf("   - L'ordinateur analyse la grille pour faire le meilleur choix possible et essayer de gagner a tout prix.\n");
    printf("   Taille  (14x15) temps de jeu 3 sec/tour\n");

    printf("\nConseils :\n");
    printf("- Essayez de planifier vos coups a l'avance pour creer plusieurs alignements possibles.\n");
    printf("- Bloquez les alignements adverses des que possible.\n");
    printf("- Ne negligez pas les diagonales, elles sont souvent moins visibles !\n");

    printf("\nCommandes :\n");
    printf("- Au menu principal, selectionnez les options a l'aide des chiffres (1 - 6).\n");
    printf("- Pendant la partie, entrez le numero de la colonne ou vous voulez deposer votre pion.\n");

    printf("\nAmusez-vous bien et bonne chance !\n\n");

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

int niveauDificulte()
{
    effacer_ecran();
    char choix;
    printf("\n=== Niveau de difficultes ===\n");
    printf("1. Facile (6x7) temps de jeu 10 sec/tour\n");
    printf("2. Intermediaire (9x10) temps de jeu 6 sec/tour\n");
    printf("3. Difficile (14x15) temps de jeu 3 sec/tour\n");
    printf("4. Retourner au menu principal\n");

    printf("Entrez votre choix : \t");
    while (getchar() != '\n')
        ;
    choix = fgetc(stdin);
    // printf("Choix %c\n", choix);
    if (isdigit(choix))
    {
        switch (choix)
        {
        case '1':
            return choix - '0';
            break;
        case '2':
            return choix - '0';
            break;
        case '3':
            return choix - '0';
            break;
        case '4':
            return choix - '0';
            break;
        default:
            afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 - 3.\n");
            niveauDificulte();
            break;
        }
    }
    else
    {
        afficher_erreur("\nEntree non valide. Veuillez entrer un chiffre entre 1 - 3.\n");
        return niveauDificulte();
    }
    return choix - '0';
}
void vider_tampon()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Ignore les caractères restants dans le tampon
    }
}
void saisieNomJoueur(char *joueur1, char *joueur2, int choix)
{
    // printf("Choix %d\n", choix);
    int err = 0;
    vider_tampon();
    do
    {
        err = 0;
        printf("\nEntrez le nom du joueur 1 : \t");

        if (fgets(joueur1, 12, stdin) != NULL)
        {
            joueur1[strcspn(joueur1, "\n")] = '\0'; // Supprimer le retour à la ligne
        }
        if (strcasecmp(joueur1, "Ordinateur") == 0 || strcasecmp(joueur1, "Ordi") == 0)
        {
            afficher_erreur("Ce nom est reserve pour l'ordinateur.\n");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            err = 1;
            while (getchar() != '\n')
                ;
            effacer_ecran();
        }

        // Verifier si la chaîne est vide ou contient uniquement des espaces
        if (joueur1[0] == '\0' || joueur1[0] == ' ')
        {
            afficher_erreur("Vous n'avez rien saisi ou uniquement des espaces.\n");
            printf("\nAppuyez sur la touche entree pour reessayer...\n");
            while (getchar() != '\n')
                ;
            effacer_ecran();
        }
    } while (joueur1[0] == '\0' || joueur1[0] == ' ' || err == 1);

    if (choix == 1)
    {
        strcpy(joueur2, "Ordinateur");
    }
    else if (choix == 2)
    {
        do
        {
            err = 0;
            printf("\nEntrez le nom du joueur 2 : \t");

            if (fgets(joueur2, 12, stdin) != NULL)
            {
                joueur2[strcspn(joueur2, "\n")] = '\0'; // Supprimer le retour à la ligne
            }

            if (strcasecmp(joueur1, joueur2) == 0)
            {
                afficher_erreur("Ce nom est deja pris par le joueur 1.\n");
                printf("\nAppuyez sur la touche entree pour reessayer...\n");
                err = 1;
                while (getchar() != '\n')
                    ;
                effacer_ecran();
            }

            // Verifier si la chaîne est vide ou contient uniquement des espaces
            if (joueur2[0] == '\0' || joueur2[0] == ' ')
            {
                afficher_erreur("Vous n'avez rien saisi ou uniquement des espaces.\n");
                printf("\nAppuyez sur la touche entree pour reessayer...\n");
                while (getchar() != '\n')
                    ;
                effacer_ecran();
            }
        } while (joueur2[0] == '\0' || joueur2[0] == ' ' || err == 1);
    }
}

// Fonction pour lire et rejouer la partie depuis un fichier de sauvegarde
int replayPartie(const char *nomFichier)
{
    char cheminFichier[300];
    snprintf(cheminFichier, sizeof(cheminFichier), "parties/%s", nomFichier);
    FILE *f = fopen(cheminFichier, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s.\n", cheminFichier);
        return -1;
    }

    char buffer[256];
    int lignes = 0, colonnes = 0;
    char joueur1[100], joueur2[100];
    char pionJoueur1, pionJoueur2;

    // Lire les informations initiales
    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        // Supprimer le caractère de nouvelle ligne '\n' si present
        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strncmp(buffer, "Lignes:", 7) == 0)
        {
            sscanf(buffer, "Lignes: %d", &lignes);
        }
        else if (strncmp(buffer, "Colonnes:", 9) == 0)
        {
            sscanf(buffer, "Colonnes: %d", &colonnes);
        }
        else if (strncmp(buffer, "Joueur1:", 8) == 0)
        {
            sscanf(buffer, "Joueur1: %s", joueur1);
        }
        else if (strncmp(buffer, "Joueur2:", 8) == 0)
        {
            sscanf(buffer, "Joueur2: %s", joueur2);
        }
        else if (strncmp(buffer, "PionJoueur1:", 13) == 0)
        {
            sscanf(buffer, "PionJoueur1: %c", &pionJoueur1);
        }
        else if (strncmp(buffer, "PionJoueur2:", 13) == 0)
        {
            sscanf(buffer, "PionJoueur2: %c", &pionJoueur2);
        }
        else if (strcmp(buffer, "----- Sauvegarde -----") == 0)
        {
            break; // Passer aux sauvegardes
        }
    }

    if (lignes == 0 || colonnes == 0)
    {
        fprintf(stderr, "Erreur : Dimensions de la grille non specifiees.\n");
        fclose(f);
        return -1;
    }

    printf("Rejouer la partie entre %s (%c) et %s (%c)\n", joueur1, pionJoueur1, joueur2, pionJoueur2);

    // Allouer la grille
    char **grille = (char **)malloc(lignes * sizeof(char *));
    if (grille == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de memoire pour la grille.\n");
        fclose(f);
        return -1;
    }
    for (int i = 0; i < lignes; i++)
    {
        grille[i] = (char *)malloc(colonnes * sizeof(char));
        if (grille[i] == NULL)
        {
            fprintf(stderr, "Erreur d'allocation de memoire pour la ligne %d.\n", i);
            for (int k = 0; k < i; k++)
            {
                free(grille[k]);
            }
            free(grille);
            fclose(f);
            return -1;
        }
    }

    // Lire chaque sauvegarde et afficher la grille
    int sauvegardeNumero = 1;
    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        // Supprimer le caractère de nouvelle ligne '\n' si present
        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strcmp(buffer, "----- Sauvegarde -----") == 0)
        {
            // Lire la grille
            for (int i = 0; i < lignes; i++)
            {
                if (fgets(buffer, sizeof(buffer), f) == NULL)
                {
                    fprintf(stderr, "Erreur : Sauvegarde incomplète.\n");
                    // Liberer la grille
                    for (int k = 0; k < lignes; k++)
                    {
                        free(grille[k]);
                    }
                    free(grille);
                    fclose(f);
                    return -1;
                }

                // Parser la ligne
                char *token = strtok(buffer, " ");
                for (int j = 0; j < colonnes && token != NULL; j++)
                {
                    grille[i][j] = (token[0] == '0') ? ' ' : token[0];
                    token = strtok(NULL, " ");
                }
            }

            // Afficher la grille
            printf("----- Sauvegarde %d -----\n", sauvegardeNumero);
            afficherGrille(grille, lignes, colonnes);
            sauvegardeNumero++;

            // Attendre une action utilisateur pour continuer
            printf("Appuyez sur Entree pour continuer...");
            getchar(); // Pour capturer le '\n' restant
            getchar(); // Attendre l'Entree
        }
    }

    // Liberer la memoire allouee pour la grille
    libererGrille(grille, lignes);
    fclose(f);
#ifdef _WIN32
    // Préparer les événements de pression et de relâchement de la touche Entrée
    INPUT inputs[2] = {0};

    // Pression de la touche Entrée
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_RETURN; // Code virtuel pour la touche Entrée
    inputs[0].ki.dwFlags = 0;     // Pression de la touche

    // Relâchement de la touche Entrée
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_RETURN;           // Code virtuel pour la touche Entrée
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // Relâchement de la touche

    // Envoyer les événements
    UINT uSent = SendInput(2, inputs, sizeof(INPUT));
    if (uSent != 2)
    {
        printf("Erreur lors de l'envoi des événements de clavier. Erreur : %lu\n", GetLastError());
        return 1;
    }

#else
    // Code pour Linux (X11)
    Display *display;
    Window root;
    XEvent event;
    int screen;

    // Ouvrir une connexion avec le serveur X
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le serveur X.\n");
        return 1;
    }

    screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    // Fonction pour simuler une pression ou un relâchement de touche
    void send_key_event(int keycode, Bool is_press)
    {
        memset(&event, 0, sizeof(event));
        event.type = is_press ? KeyPress : KeyRelease;
        event.xkey.display = display;
        event.xkey.window = root;
        event.xkey.root = root;
        event.xkey.keycode = keycode;
        event.xkey.state = 0;
        XSendEvent(display, root, True, KeyPressMask | KeyReleaseMask, &event);
        XFlush(display);
    }

    // Obtenir le code de la touche Entrée
    KeySym keysym = XK_Return;
    int keycode = XKeysymToKeycode(display, keysym);

    // Simuler la pression de la touche Entrée
    send_key_event(keycode, True);
    usleep(100000); // Pause de 100ms
    send_key_event(keycode, False);

    // Fermer la connexion avec le serveur X
    XCloseDisplay(display);
#endif

    if (continuerJeu("Retourner sur le menu? Saisir 1 Pour Oui 0 Pour quitter le jeu....") == 49)
    {
        effacer_ecran();
        lancer_jeu();
    }
    else
    {
        quitterJeu();
    }
    return 0;
}
