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
    int choix = 0;
    cout << "Interface console (0) ou interface Qt (1) ?" << endl;
    cin >> choix;
    if (choix == 0) {
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
    }
    else if (choix == 1) {
        cout << "Un peu de patience..." << endl;
    }
    else {
        cout << "Choix invalide" << endl;
    }
    return 0;
}

