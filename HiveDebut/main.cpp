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
    GestionnairePions gP;
    GestionnaireMouvements gM;
    GestionnaireVoisins gV;
    GestionnaireCommand gC;
    unsigned int length = 20;

    Plateau plateau(length, length, 5, gP, gM, gV);
    Partie partie(plateau, gC);
    partie.setup();
    while (partie.partieTerminee() != true) {
        std::cout << "Historique SiAZE 1 : " << gC.historique.size() << "\n";
        partie.sauvegarde();
        std::cout << "Historique SiAZE 2 : " << gC.historique.size() << "\n";
        partie.jouerUnTour(partie.getJoueur1());
        partie.jouerUnTour(partie.getJoueur2());

        partie.getPlateau().afficher();

    }
    return 0;
}

