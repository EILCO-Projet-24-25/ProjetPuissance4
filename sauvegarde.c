#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h> // Pour _mkdir
#define MKDIR(path) _mkdir(path)
#else
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

int verifierFichier(const char *nomFichier)
{
    FILE *fichier = fopen(nomFichier, "r");

    // Verifier si le fichier existe
    if (fichier == NULL)
    {
        // printf("Le fichier \"%s\" n'existe pas.\n", nomFichier);
        return 0; // Le fichier n'existe pas
    }

    // Se deplacer à la fin du fichier pour obtenir sa taille
    fseek(fichier, 0, SEEK_END);
    long taille = ftell(fichier); // Obtenir la position actuelle (taille du fichier)
    fclose(fichier);              // Fermer le fichier après verification

    if (taille > 0)
    {
        // printf("Le fichier \"%s\" existe et contient des donnees.\n", nomFichier);
        return 1; // Le fichier existe et contient des donnees
    }
    else
    {
        // printf("Le fichier \"%s\" existe mais est vide.\n", nomFichier);
        return 2; // Le fichier existe mais est vide
    }
}

void sauvegarderJeu(char **grille, int lignes, int colonnes, const char *nomFichier, char *joueur1, char *joueur2, int difficultes, int modejeu, int choixjeu, char pion1, char pion2, char *nomPartie)
{
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    fprintf(fichier, "%d %d %d %d %d %s %s %c %c %s\n", lignes, colonnes, difficultes, modejeu, choixjeu, joueur1, joueur2, pion1, pion2, nomPartie); // Sauvegarde du nombre de lignes, colones, difficultes, mode de jeu, joueur1, joueur2

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == ' ')
            {
                fprintf(fichier, "%c ", '0');
            }
            else
            {
                fprintf(fichier, "%c ", grille[i][j]);
            }
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}

char **chargerJeu(int *lignes, int *colonnes, int *difficultes, int *modejeu, int *choixjeu, const char *nomFichier, char *joueur1, char *joueur2, char *pion1, char *pion2, char *nomPartie)
{
    printf("Charger une partie");
    char **grille = NULL;
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Lecture de la première ligne pour les metadonnees
    if (fscanf(fichier, "%d %d %d %d %d %s %s %c %c %s\n", lignes, colonnes, difficultes, modejeu, choixjeu, joueur1, joueur2, pion1, pion2, nomPartie) != 10)
    {
        fprintf(stderr, "\nErreur lors de la lecture des metadonnees du fichier.\n");
        fclose(fichier);
        exit(1);
    }

    grille = creationGrille(*lignes, *colonnes);
    // Lecture des lignes restantes pour la grille
    for (int i = 0; i < *lignes; i++)
    {
        for (int j = 0; j < *colonnes; j++)
        {
            char c;
            if (fscanf(fichier, " %c", &c) != 1)
            {
                fprintf(stderr, "Erreur lors de la lecture de la grille.\n");
                // Liberation de la memoire en cas d'erreur
                for (int k = 0; k < *lignes; k++)
                {
                    free(grille[k]);
                }
                free(grille);
                fclose(fichier);
                exit(EXIT_FAILURE);
            }
            // Remplacer '0' par ' ' et stocker dans la grille
            grille[i][j] = (c == '0') ? ' ' : c;
        }
    }

    fclose(fichier);
    return grille;
}

char *creerPartie(const char *joueur1, const char *joueur2)
{
    // Recuperer la date et l'heure actuelles
    time_t timestamp = time(NULL);
    if (timestamp == ((time_t)-1))
    {
        fprintf(stderr, "Erreur lors de la recuperation de l'horodatage.\n");
        return NULL;
    }

    struct tm *localTime = localtime(&timestamp);
    if (localTime == NULL)
    {
        fprintf(stderr, "Erreur lors de la recuperation de l'heure locale.\n");
        return NULL;
    }

    // Formater la date et l'heure en DDMMYYYYHHMM
    char datetime[13]; // 12 caractères + '\0'
    if (strftime(datetime, sizeof(datetime), "%d%m%Y%H%M", localTime) == 0)
    {
        fprintf(stderr, "Erreur lors du formatage de la date et de l'heure.\n");
        return NULL;
    }

    // Sanitiser les noms des joueurs (remplacer les espaces par des underscores)
    char joueur1_sanitized[100];
    char joueur2_sanitized[100];
    int idx = 0;

    // Remplacer les espaces par des underscores pour joueur1
    while (joueur1[idx] != '\0' && idx < (int)(sizeof(joueur1_sanitized) - 1))
    {
        if (joueur1[idx] == ' ')
            joueur1_sanitized[idx] = '_';
        else
            joueur1_sanitized[idx] = joueur1[idx];
        idx++;
    }
    joueur1_sanitized[idx] = '\0';

    idx = 0;
    // Remplacer les espaces par des underscores pour joueur2
    while (joueur2[idx] != '\0' && idx < (int)(sizeof(joueur2_sanitized) - 1))
    {
        if (joueur2[idx] == ' ')
            joueur2_sanitized[idx] = '_';
        else
            joueur2_sanitized[idx] = joueur2[idx];
        idx++;
    }
    joueur2_sanitized[idx] = '\0';

    // Creer le nom du fichier au format "DDMMYYYYHHMM_joueur1_Vs_joueur2.txt"
    char nomFichierGrille[256];
    snprintf(nomFichierGrille, sizeof(nomFichierGrille), "%s_%s_Vs_%s.txt", datetime, joueur1_sanitized, joueur2_sanitized);

// Creer le repertoire "parties" s'il n'existe pas
#ifdef _WIN32
    // Sur Windows, _mkdir ne retourne pas d'erreur si le repertoire existe dejà
    MKDIR("parties");
#else
    struct stat st = {0};
    if (stat("parties", &st) == -1)
    {
        if (MKDIR("parties") != 0)
        {
            perror("Erreur lors de la creation du repertoire 'parties'");
            return NULL;
        }
    }
#endif

    // Creer le fichier dans le repertoire "parties"
    char cheminFichierGrille[300];
    snprintf(cheminFichierGrille, sizeof(cheminFichierGrille), "parties/%s", nomFichierGrille);

    FILE *f = fopen(cheminFichierGrille, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Erreur lors de la creation du fichier %s\n", cheminFichierGrille);
        return NULL;
    }
    fclose(f);

    // Enregistrer uniquement le nom du fichier dans "parties/parties.txt"
    FILE *fichierParties = fopen("parties/parties.txt", "a");
    if (fichierParties == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de parties/parties.txt\n");
        return NULL;
    }

    // ecrire uniquement le nom du fichier dans "parties/parties.txt"
    fprintf(fichierParties, "%s\n", nomFichierGrille);
    fclose(fichierParties);

    // Allouer dynamiquement la chaîne pour le nom du fichier à retourner
    char *nomFichierRetour = malloc(strlen(nomFichierGrille) + 1);
    if (nomFichierRetour == NULL)
    {
        fprintf(stderr, "Erreur d'allocation memoire pour le nom du fichier.\n");
        return NULL;
    }
    strcpy(nomFichierRetour, nomFichierGrille);

    return nomFichierRetour;
}

int sauvegarde(const char *nomFichierGrille, char **grille, int lignes, int colonnes, char *joueur1, char *joueur2, char pionjoueur1, char pionjoueur2)
{
    // 1. Construire le chemin complet du fichier
    char cheminFichier[300];
    snprintf(cheminFichier, sizeof(cheminFichier), "parties/%s", nomFichierGrille);

    // 2. Ouvrir le fichier en mode lecture et ecriture (ajout)
    FILE *f = fopen(cheminFichier, "a+");
    if (f == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour sauvegarde.\n", cheminFichier);
        return -1;
    }

    // 3. Verifier si le fichier est vide (première sauvegarde)
    fseek(f, 0, SEEK_END);
    long tailleFichier = ftell(f);
    int premiereSauvegarde = (tailleFichier == 0) ? 1 : 0;

    if (premiereSauvegarde)
    {
        // 4. Enregistrer les dimensions de la grille
        fprintf(f, "Lignes: %d\n", lignes);
        fprintf(f, "Colonnes: %d\n", colonnes);
        fprintf(f, "Joueur1: %s\n", joueur1);
        fprintf(f, "Joueur2: %s\n", joueur2);
        fprintf(f, "PionJoueur1: %c\n", pionjoueur1);
        fprintf(f, "PionJoueur2: %c\n", pionjoueur2);
        fprintf(f, "----- Sauvegarde -----\n");
    }
    else
    {
        // Ajouter un separateur pour les sauvegardes suivantes
        fprintf(f, "----- Sauvegarde -----\n");
    }

    // 5. Enregistrer l'etat actuel de la grille
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == ' ')
            {
                fprintf(f, "%c ", '0');
            }
            else
            {
                fprintf(f, "%c ", grille[i][j]);
            }
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n"); // Ligne vide pour separer les sauvegardes

    // 6. Fermer le fichier
    fclose(f);

    return 0;
}