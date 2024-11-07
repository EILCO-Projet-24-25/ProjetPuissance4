#include "puissance.h"


void initialisionGrille(int rows, int cols) {
    grid = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        grid[i] = (char *)malloc(cols * sizeof(char));
        for (int j = 0; j < cols; j++) {
            grid[i][j] = ' ';
        }
    }
}

void affichageGrille() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("| %c ", grid[i][j]);
        }
        printf("|\n");
    }
    for (int j = 1; j <= cols; j++) {
        printf("  %d ", j);
    }
    printf("\n");
}

void libererGrille() {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

int deposerPignon(int col, char disc) {
    if (col < 0 || col >= cols) return 0;

    for (int i = rows - 1; i >= 0; i--) {
        if (grid[i][col] == ' ') {
            grid[i][col] = disc;
            return 1;
        }
    }
    return 0; // Colonne pleine
}

int checkWin(char disc) {
    // Vérifications pour détecter 4 pions alignés dans les directions horizontale, verticale, et diagonales
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols - 3; j++) {
            if (grid[i][j] == disc && grid[i][j+1] == disc && grid[i][j+2] == disc && grid[i][j+3] == disc)
                return 1;
        }
    }
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == disc && grid[i+1][j] == disc && grid[i+2][j] == disc && grid[i+3][j] == disc)
                return 1;
        }
    }
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < cols - 3; j++) {
            if (grid[i][j] == disc && grid[i+1][j+1] == disc && grid[i+2][j+2] == disc && grid[i+3][j+3] == disc)
                return 1;
        }
    }
    for (int i = 3; i < rows; i++) {
        for (int j = 0; j < cols - 3; j++) {
            if (grid[i][j] == disc && grid[i-1][j+1] == disc && grid[i-2][j+2] == disc && grid[i-3][j+3] == disc)
                return 1;
        }
    }
    return 0;
}

int choisirModeJeu() {
    int choix;
    printf("Choisissez le mode de jeu :\n");
    printf("1. Jouer contre un autre joueur\n");
    printf("2. Jouer contre le bot\n");
    printf("Entrez votre choix (1 ou 2) : ");
    scanf("%d", &choix);

    if (choix == 1) {
        printf("Mode sélectionné : Joueur contre Joueur.\n");
        return 1; 
    } else if (choix == 2) {
        printf("Mode sélectionné : Joueur contre Bot.\n");
        return 2;  
    } else {
        printf("Choix invalide. Par défaut, mode Joueur contre Joueur.\n");
        return 1; 
    }
}

char choisirPion(char defaultPion) {
    char pion;
    printf("Choisissez votre pion (ex: X, O) ou appuyez sur Entrée pour garder '%c' : ", defaultPion);
    scanf(" %c", &pion);
    return (pion != '\n') ? pion : defaultPion;
}

