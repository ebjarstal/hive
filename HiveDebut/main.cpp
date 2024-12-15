#include "partie.h"
#include "plateau.h"
#include "pions.h"
#include "gestionnaireMouvements.h"
#include "gestionnairePions.h"
#include "gestionnaireVoisins.h"
#include "gestionnaireSauvegarde.h"
#include "joueur.h"
#include "mouvement.h"
#include "Constantes.h"

#include <QApplication>
#include "FenetrePrincipale.h"

using namespace std;

int main(int argc, char* argv[]) {
    int choix = 0;
    cout << "Interface console (0) ou interface Qt (1) ?" << endl;
    cin >> choix;
    if (choix == 0) {

        Plateau plateau(TAILLE_PLATEAU, TAILLE_PLATEAU, 5);
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
        QApplication app(argc, argv);
        FenetrePrincipale fenetre;
        fenetre.show();
        return app.exec();
    }
    else {
        cout << "Choix invalide" << endl;
    }
    return 0;
}
