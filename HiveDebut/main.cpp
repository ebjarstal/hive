#include "partie.h"
#include "plateau.h"
#include "pions.h"
#include "gestionnaireMouvements.h"
#include "gestionnairePions.h"
#include "gestionnaireVoisins.h"
#include "gestionnaireSauvegarde.h"
#include "joueur.h"
#include "mouvement.h"

using namespace std;

int main() {
    unsigned int length = 20;

    Plateau plateau(length, length, 5);
    Partie partie(plateau);
    partie.setup();
    while (partie.partieTerminee() != true) {
        GestionnaireSauvegarde::sauvegarde(partie);
        partie.getPlateau().afficher();
        partie.jouerUnTour(partie.getJoueur1());
        if (partie.partieTerminee() == true) break;
        partie.getPlateau().afficher();
        partie.jouerUnTour(partie.getJoueur2());
    }
    return 0;
}

