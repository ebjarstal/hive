#include "partie.h"
#include "plateau.h"
#include "pions.h"
#include "gestionnaireMouvements.h"
#include "gestionnairePions.h"
#include "gestionnaireVoisins.h"
#include "joueur.h"
#include "mouvement.h"

using namespace std;

int main() {
    unsigned int length = 20;

    Plateau plateau(length, length, 5);
    Partie partie(plateau);
    partie.setup();
    while (partie.partieTerminee() != true) {
        partie.sauvegarde();
        partie.getPlateau().afficher();
        partie.jouerUnTour(partie.getJoueur1());
        partie.getPlateau().afficher();
        partie.jouerUnTour(partie.getJoueur2());
    }
    return 0;
}

