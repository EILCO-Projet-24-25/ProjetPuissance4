#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "jeu.h"
#include "parametre.h"
#include "sauvegarde.h"

#define JOUEUR 'X'
#define ORDI 'O'
#define VIDE ' '

int main(int argc, char *argv[])
{
    lancer_jeu();
    return 0;
}
